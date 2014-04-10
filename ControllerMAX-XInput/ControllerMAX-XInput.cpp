// ControllerMAX-XInput.cpp : main project file.

#include "stdafx.h"

#include <ctime>
#include <math.h>
#include <iostream>
#include <windows.h>

/*  Define the API model (PLUGIN or DIRECT) before including the 
 *  gcapi.h header file.
 */
#define GCAPI_DIRECT
#include "gcapi.h"

/*  Direct API exported functions. Check gcapi.h for a more detailed 
 *  description.
 */
GCDAPI_Load gcdapi_Load = NULL;
GCDAPI_Unload gcdapi_Unload = NULL;
GCAPI_IsConnected gcapi_IsConnected = NULL;
GCAPI_GetFWVer gcapi_GetFWVer = NULL;
GCAPI_Read gcapi_Read = NULL;
GCAPI_Write gcapi_Write = NULL;
GCAPI_GetTimeVal gcapi_GetTimeVal = NULL;
GCAPI_CalcPressTime gcapi_CalcPressTime = NULL;

HINSTANCE hInsGPP = NULL;
GCAPI_REPORT report = {0};

int8_t xinputInput[GCAPI_OUTPUT_TOTAL] = {0};

// report.input and XInput merged as per config
int8_t mergedInput[GCAPI_OUTPUT_TOTAL] = {0}; 

int8_t output[GCAPI_OUTPUT_TOTAL] = {0};

/*  Configuration
 */
wchar_t iniFilePath[100] = L"./ControllerMAX-XInput.ini\0";


namespace ControllerMAX_XInput {

	using namespace System;
	using namespace System::ComponentModel;

	int iround(double num) {
		return (num > 0.0) ? (int)floor(num + 0.5) : (int)ceil(num - 0.5);
	}

	public value class FORWARDER_STATE
	{
	public:
		System::String^ errorMessage;
		static bool controllerConnected;
		static bool deviceConnected;
		static array<int^> ^input = gcnew array<int^>(21);
		static array<int^> ^output = gcnew array<int^>(21);
	};

	/* GPC Language I/O Functions
	 */

	// Returns the current value of a controller entry
	int get_val( int button )
	{
		return mergedInput[button];
	}

	// Overwrites the current value of a controller entry
	void set_val( int button, int value )
	{
		output[button] = value;
	}

	// Returns the previous value of a controller entry
	int get_lval( int button )
	{
		// TODO: XInput controller input
		return report.input[button].prev_value;
	}

	// Returns the elapsed activation time (pressing) of a controller entry
	int get_ptime( int button )
	{
		// TODO: XInput controller input
		return gcapi_CalcPressTime(report.input[button].press_tv);
	}

	// Returns TRUE when detected activation (pressing) of a controller entry
	bool event_press( int button )
	{
		return get_val(button) > 0 && get_lval(button) == 0 ? true : false;
	}

	// Returns TRUE when detected deactivation (releasing) of a controller entry
	bool event_release( int button )
	{
		return get_val(button) == 0 && get_lval(button) > 0 ? true : false;
	}

	// Swap the values between two controller entries
	void swap( int button1, int button2 )
	{
		output[button1] = mergedInput[button2];
		output[button2] = mergedInput[button1];
	}

	// Blocks forwarding of a controller entry for a short period of time
	void block( int button, int time )
	{
		// TODO: Flag as blocked for <time>, starting at <now>, check this in main loop
		output[button] = 0;
	}

	// TODO: sensitivity   Adjust the sensitivity of a controller entry
	// TODO: deadzone      Remove the deadzone of a pair of entries, usually of analog sticks
	// TODO: stickize      Transform the values of mouse input (or Wiimote IR) to analog stick

	void XInputForwarder(int controllerNum, BackgroundWorker^ worker, DoWorkEventArgs ^ e )
	{

		bool cancellationPending = worker->CancellationPending;
		ULONGLONG reportTimer = GetTickCount64();

		FORWARDER_STATE forwarderState;

		HINSTANCE hInsGPP = NULL;
		GCAPI_REPORT report = {0};

		// Load configuration
		bool passthruInput = GetPrivateProfileInt(L"Options", L"PassthruInput", 0, iniFilePath) ? true : false;
		bool applySteeringCorrection = GetPrivateProfileInt(L"Options", L"SteeringCorrection", 0, iniFilePath) ? true : false;

		// Load the Direct API Library
		hInsGPP = LoadLibrary(TEXT("gcdapi.dll"));
		if(hInsGPP == NULL)
		{
			if(!cancellationPending)
			{
				forwarderState.errorMessage = "Error on loading gcdapi.dll";
				worker->ReportProgress(0, forwarderState);
				cancellationPending = true;
			}
		}

		// Set up the pointers to DLL exported functions
		gcdapi_Load = (GCDAPI_Load) GetProcAddress(hInsGPP, "gcdapi_Load");
		gcdapi_Unload = (GCDAPI_Unload) GetProcAddress(hInsGPP, "gcdapi_Unload");
		gcapi_IsConnected = (GCAPI_IsConnected) GetProcAddress(hInsGPP, "gcapi_IsConnected");
		gcapi_GetFWVer = (GCAPI_GetFWVer) GetProcAddress(hInsGPP, "gcapi_GetFWVer");
		gcapi_Read = (GCAPI_Read) GetProcAddress(hInsGPP, "gcapi_Read");
		gcapi_Write = (GCAPI_Write) GetProcAddress(hInsGPP, "gcapi_Write");
		gcapi_GetTimeVal = (GCAPI_GetTimeVal) GetProcAddress(hInsGPP, "gcapi_GetTimeVal");
		gcapi_CalcPressTime = (GCAPI_CalcPressTime) GetProcAddress(hInsGPP, "gcapi_CalcPressTime");

		if(gcdapi_Load == NULL || gcdapi_Unload == NULL || gcapi_IsConnected == NULL || gcapi_GetFWVer == NULL || 
			gcapi_Read == NULL || gcapi_Write == NULL || gcapi_GetTimeVal == NULL || gcapi_CalcPressTime == NULL)
		{
				FreeLibrary(hInsGPP);
				if(!cancellationPending)
				{
					forwarderState.errorMessage = "Error on gcdapi.dll";
					worker->ReportProgress(0, forwarderState);
					cancellationPending = true;
				}
				
		}

		// Allocate resources and initialize the Direct API.
		if(hInsGPP != NULL)
		{
			if(!gcdapi_Load())
			{
				FreeLibrary(hInsGPP);
				if(!cancellationPending)
				{
					forwarderState.errorMessage = "Unable to initiate the Direct API";
					worker->ReportProgress(0, forwarderState);
					cancellationPending = true;
				}
			}
		}

		//
		// XInput Structures
		//
		struct XInputStateEx
		{
			unsigned long eventCount;  // increases with every controller event, but not by one.
			unsigned short up:1, down:1, left:1, right:1, start:1, back:1, leftThumb:1, 
				rightThumb:1, leftShoulder:1, rightShoulder:1, guideButton:1, unknown:1, 
				aButton:1, bButton:1, xButton:1, yButton:1;
			unsigned char leftTrigger;
			unsigned char rightTrigger;
			short thumbLX;
			short thumbLY;
			short thumbRX;
			short thumbRY;
		};

		struct XInputVibration
		{
			WORD wLeftMotorSpeed;
			WORD wRightMotorSpeed;
		};

		// Create hInstance of xinput1_3
		HINSTANCE hInsXInput1_3 = LoadLibrary(L"xinput1_3.dll");
		if(hInsXInput1_3 == NULL)
		{
			if(!cancellationPending)
			{
				forwarderState.errorMessage = "Error on loading xinput1_3.dll";
				worker->ReportProgress(0, forwarderState);
				cancellationPending = true;
			}
		}

		// Alternative to XInputGetState
		// https://github.com/DieKatzchen/GuideButtonPoller
		// Details on unnamed ordinals:
		// https://code.google.com/p/x360ce/issues/detail?id=417

		// Get the address of ordinal 100 (unnamed: XInputGetStateEx) - exposes Guide button
		FARPROC pointerToDLLFunction1 = GetProcAddress(HMODULE(hInsXInput1_3), (LPCSTR)100);

		// typedef the function. It takes an int and a pointer to an XInputStateEx and returns an error code
		// as an int. It's 0 for no error and 1167 for "controller not present". Presumably there are others
		// but I never saw them. It won't cause a crash on error, it just won't update the data.
		typedef int(__stdcall * pICFUNC1)(int, XInputStateEx &);

		// Assign XInputGetStateEx for easier use
		pICFUNC1 XInputGetStateEx;
		XInputGetStateEx = pICFUNC1(pointerToDLLFunction1);

		// Get the pointer to XInputSetState (ordinal 3)
		FARPROC pointerToDLLFunction2 = GetProcAddress(HMODULE(hInsXInput1_3), "XInputSetState");

		typedef int(__stdcall * pICFUNC2)(int, XInputVibration &);

		// Assign XInputSetState for easier use
		pICFUNC2 XInputSetState;
		XInputSetState = pICFUNC2(pointerToDLLFunction2);

		// Create a Controller State
		XInputStateEx controllerState;

		// Create a Vibration State
		XInputVibration vibration;

		while ( !cancellationPending )
		{
			DWORD result = XInputGetStateEx(controllerNum, controllerState);

			forwarderState.controllerConnected = result == ERROR_SUCCESS ? true : false;
			forwarderState.deviceConnected = gcapi_IsConnected() ? true : false;
			
			if(forwarderState.controllerConnected)
			{
				// Left Thumb
				float LX = controllerState.thumbLX;
				float LY = controllerState.thumbLY;

				int8_t percentageLX = (int8_t)iround((LX / 32767) * 100);
				int8_t percentageLY = (int8_t)iround((LY / 32767) * 100);

				// CM expects Y-axis -100 up, 100 down
				percentageLY *= -1;

				// Right Thumb
				float RX = controllerState.thumbRX;
				float RY = controllerState.thumbRY;

				int8_t percentageRX = (int8_t)iround((RX / 32767) * 100);
				int8_t percentageRY = (int8_t)iround((RY / 32767) * 100);

				// CM expects Y-axis -100 up, 100 down
				percentageRY *= -1;

				// Left Trigger
				float LT = (float)controllerState.leftTrigger;
				int8_t percentageLT = (int8_t)iround((LT / 255) * 100);

				// Right Trigger
				float RT = (float)controllerState.rightTrigger;
				int8_t percentageRT = (int8_t)iround((RT / 255) * 100);

				// XInput controller input
				xinputInput[0] = controllerState.guideButton ? 100 : 0; // Guide
				xinputInput[1] = controllerState.back ? 100 : 0; // Back
				xinputInput[2] = controllerState.start ? 100 : 0; // Start
				xinputInput[3] = controllerState.rightShoulder ? 100 : 0; // Right Shoulder
				xinputInput[4] = percentageRT; // Right Trigger [0 ~ 100] %
				xinputInput[5] = controllerState.rightThumb ? 100 : 0; // Right Analog Stick (Pressed)
				xinputInput[6] = controllerState.leftShoulder ? 100 : 0; // Left Shoulder
				xinputInput[7] = percentageLT; // Left Trigger [0 ~ 100] %
				xinputInput[8] = controllerState.leftThumb ? 100 : 0; // Left Analog Stick (Pressed)
				xinputInput[9] = percentageRX; // Right Analog Stick X-axis [-100 ~ 100] %
				xinputInput[10] = percentageRY; // Right Analog Stick Y-axis [-100 ~ 100] %
				xinputInput[11] = percentageLX; // Left Analog Stick X-axis [-100 ~ 100] %
				xinputInput[12] = percentageLY; // Left Analog Stick Y-axis [-100 ~ 100] %
				xinputInput[13] = controllerState.up ? 100 : 0; // DPad Up
				xinputInput[14] = controllerState.down ? 100 : 0; // DPad Down
				xinputInput[15] = controllerState.left ? 100 : 0; // DPad Left
				xinputInput[16] = controllerState.right ? 100 : 0; // DPad Right
				xinputInput[17] = controllerState.yButton ? 100 : 0; // Y
				xinputInput[18] = controllerState.bButton ? 100 : 0; // B
				xinputInput[19] = controllerState.aButton ? 100 : 0; // A
				xinputInput[20] = controllerState.xButton ? 100 : 0; // X

				if(forwarderState.deviceConnected)
				{
					gcapi_Read(&report);
				}

				// Merge or disregard CM input data (auth controller)
				if(passthruInput && forwarderState.deviceConnected)
				{
					for(uint8_t i=0; i<GCAPI_INPUT_TOTAL; i++)
					{
						mergedInput[i] = xinputInput[i] ? xinputInput[i] : report.input[i].value;
					}
				}
				else
				{
					for(uint8_t i=0; i<GCAPI_INPUT_TOTAL; i++)
					{
						mergedInput[i] = xinputInput[i];
					}
				}

				// Input to report to UI
				for(uint8_t i=0; i<=20; i++)
				{
					forwarderState.input[i] = Convert::ToInt32(mergedInput[i]);
				}
				
				// Output to console and XInput controller
				if(forwarderState.deviceConnected)
				{
					// Vibrate XInput controller
					// reported as [0 ~ 100] %, XInput range [0 ~ 65535]
					vibration.wRightMotorSpeed = iround(655.35 * (float)report.rumble[0]);
					vibration.wLeftMotorSpeed = iround(655.35 * (float)report.rumble[1]);
					XInputSetState(controllerNum, vibration);

					// Set output
					for(uint8_t i=0; i<GCAPI_INPUT_TOTAL; i++)
					{
						output[i] = mergedInput[i];
					}
					
					// Execute scripts

					// Steering Correction
					if(applySteeringCorrection)
					{
						if(get_val(XB1_LX) < 0)
						{
							set_val(XB1_LX, ((get_val(XB1_LX) * 7) / 9) - 23);
						}
						if(get_val(XB1_LX) > 0)
						{
							set_val(XB1_LX, ((get_val(XB1_LX) * 7) / 9) + 23);
						}
					}

					gcapi_Write(output);
				}

			}
			else if(forwarderState.deviceConnected)
			{
				// Passthrough mode
				// Input from controller to console
				gcapi_Read(&report);
				for(uint8_t i=0; i<GCAPI_INPUT_TOTAL; i++)
				{
					output[i] = report.input[i].value;
				}
				gcapi_Write(output);
			}

			// Output to report to UI
			for(uint8_t i=0; i<=20; i++)
			{
				forwarderState.output[i] = Convert::ToInt32(output[i]);
			}

			// Wait at least 100ms between reports to UI
			if( (GetTickCount64() - reportTimer) > 100 )
			{
				worker->ReportProgress(0, forwarderState);
				reportTimer = GetTickCount64();
			}

			Sleep(1);
		}


		// Free API resources and unload libraries
		if(hInsGPP != NULL)
		{
			gcdapi_Unload();	
		}
		FreeLibrary(hInsGPP);
		FreeLibrary(hInsXInput1_3);

		e->Cancel = true;
	}

}

#include "Form1.h"

using namespace ControllerMAX_XInput;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
