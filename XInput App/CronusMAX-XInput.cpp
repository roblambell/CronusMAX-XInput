// CronusMAX-XInput.cpp : main project file.

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

HINSTANCE hInsDeviceAPI = NULL;
GCAPI_REPORT report = {0};

/*  GPC Interpreter type definitions / exported functions.
 *  TODO: Communicate ParseState and Message + amend/catch interpreter error handling
 */
typedef     void (__stdcall *GPCI_Load)();
typedef     void (__stdcall *GPCI_Unload)();
typedef  uint8_t (__stdcall *GPCI_Parse)(char *);
typedef  uint8_t (__stdcall *GPCI_Execute)(char *, int8_t (*)[GCAPI_OUTPUT_TOTAL], int8_t (*)[4]);

GPCI_Load gpci_Load = NULL;
GPCI_Unload gpci_Unload = NULL;
GPCI_Parse gpci_Parse = NULL;
GPCI_Execute gpci_Execute = NULL;

HINSTANCE hInsGPCInterpreter = NULL;

// report.input and XInput get merged as per config
int8_t xinputInput[GCAPI_OUTPUT_TOTAL] = {0};
int8_t mergedInput[GCAPI_OUTPUT_TOTAL] = {0};
int8_t output[GCAPI_OUTPUT_TOTAL] = {0};

int8_t rumble[4] = {0};

/*  Configuration
 */
wchar_t cfgFilePath[100] = L"./XInput.cfg\0";
char *gpcFileName = "XInput.gpc";

namespace CronusMAX_XInput {

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
		static bool gpcScriptLoaded;
		static array<int^> ^input = gcnew array<int^>(21);
		static array<int^> ^output = gcnew array<int^>(21);
		static array<int^> ^rumble_in = gcnew array<int^>(4);
		static array<int^> ^rumble_out = gcnew array<int^>(4);
	};

	void XInputForwarder(int controllerNum, BackgroundWorker^ worker, DoWorkEventArgs ^ e )
	{

		bool cancellationPending = worker->CancellationPending;
		ULONGLONG reportTimer = GetTickCount64();

		FORWARDER_STATE forwarderState;
		
		// Load configuration
		bool passthruInput = GetPrivateProfileInt(L"Options", L"PassthruInput", 0, cfgFilePath) ? true : false;

		// Load the Direct API Library
		hInsDeviceAPI = LoadLibrary(TEXT("gcdapi.dll"));
		if(hInsDeviceAPI == NULL)
		{
			if(!cancellationPending)
			{
				forwarderState.errorMessage = "Error on loading gcdapi.dll";
				worker->ReportProgress(0, forwarderState);
				cancellationPending = true;
			}
		}

		// Set up the pointers to DLL exported functions
		gcdapi_Load = (GCDAPI_Load) GetProcAddress(hInsDeviceAPI, "gcdapi_Load");
		gcdapi_Unload = (GCDAPI_Unload) GetProcAddress(hInsDeviceAPI, "gcdapi_Unload");
		gcapi_IsConnected = (GCAPI_IsConnected) GetProcAddress(hInsDeviceAPI, "gcapi_IsConnected");
		gcapi_GetFWVer = (GCAPI_GetFWVer) GetProcAddress(hInsDeviceAPI, "gcapi_GetFWVer");
		gcapi_Read = (GCAPI_Read) GetProcAddress(hInsDeviceAPI, "gcapi_Read");
		gcapi_Write = (GCAPI_Write) GetProcAddress(hInsDeviceAPI, "gcapi_Write");
		gcapi_GetTimeVal = (GCAPI_GetTimeVal) GetProcAddress(hInsDeviceAPI, "gcapi_GetTimeVal");
		gcapi_CalcPressTime = (GCAPI_CalcPressTime) GetProcAddress(hInsDeviceAPI, "gcapi_CalcPressTime");

		if(gcdapi_Load == NULL || gcdapi_Unload == NULL || gcapi_IsConnected == NULL || gcapi_GetFWVer == NULL || 
			gcapi_Read == NULL || gcapi_Write == NULL || gcapi_GetTimeVal == NULL || gcapi_CalcPressTime == NULL)
		{
				FreeLibrary(hInsDeviceAPI);
				if(!cancellationPending)
				{
					forwarderState.errorMessage = "Error on gcdapi.dll";
					worker->ReportProgress(0, forwarderState);
					cancellationPending = true;
				}
				
		}

		// Allocate resources and initialize the Direct API.
		if(hInsDeviceAPI != NULL)
		{
			if(!gcdapi_Load())
			{
				FreeLibrary(hInsDeviceAPI);
				if(!cancellationPending)
				{
					forwarderState.errorMessage = "Unable to initiate the Direct API";
					worker->ReportProgress(0, forwarderState);
					cancellationPending = true;
				}
			}
		}

		// Load the GPC Interpreter
		hInsGPCInterpreter = LoadLibrary(TEXT("gpci.dll"));
		if(hInsGPCInterpreter == NULL)
		{
			if(!cancellationPending)
			{
				forwarderState.errorMessage = "Error on loading gpci.dll";
				worker->ReportProgress(0, forwarderState);
				cancellationPending = true;
			}
		}
		else
		{
			// Set up the pointers to DLL exported functions
			gpci_Load = (GPCI_Load) GetProcAddress(hInsGPCInterpreter, "gpci_Load");
			gpci_Unload = (GPCI_Unload) GetProcAddress(hInsGPCInterpreter, "gpci_Unload");
			gpci_Parse = (GPCI_Parse) GetProcAddress(hInsGPCInterpreter, "gpci_Parse");
			gpci_Execute = (GPCI_Execute) GetProcAddress(hInsGPCInterpreter, "gpci_Execute");

			gpci_Load();

			if(gpcFileName != "")
			{
				forwarderState.gpcScriptLoaded = gpci_Parse(gpcFileName) ? true : false;
				if(!forwarderState.gpcScriptLoaded && !cancellationPending)
				{
					forwarderState.errorMessage = "Error on loading " + gcnew String(gpcFileName);
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
			
			// Process input
			if(forwarderState.controllerConnected)
			{
				// Left Thumb
				float LX = controllerState.thumbLX;
				float LY = controllerState.thumbLY;
				int8_t percentageLX = (int8_t)iround((LX / 32767) * 100);
				int8_t percentageLY = (int8_t)iround((LY / 32767) * 100);
				percentageLY *= -1; // CM expects Y-axis -100 up, 100 down

				// Right Thumb
				float RX = controllerState.thumbRX;
				float RY = controllerState.thumbRY;
				int8_t percentageRX = (int8_t)iround((RX / 32767) * 100);
				int8_t percentageRY = (int8_t)iround((RY / 32767) * 100);
				percentageRY *= -1; // CM expects Y-axis -100 up, 100 down

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
								
				// Set output
				for(uint8_t i=0; i<GCAPI_INPUT_TOTAL; i++)
				{
					output[i] = mergedInput[i];
				}
			}
			else if(forwarderState.deviceConnected)
			{
				gcapi_Read(&report);

				// Passthru mode

				// Don't need to report input to UI, it's 
				// not shown unless a controller is connected

				// Set output
				for(uint8_t i=0; i<GCAPI_INPUT_TOTAL; i++)
				{
					output[i] = report.input[i].value;
				}
			}

			// Rumble from console
			if(forwarderState.deviceConnected)
			{
				rumble[0] = report.rumble[0];
				rumble[1] = report.rumble[1];
				rumble[2] = 0; //report.rumble[2];
				rumble[3] = 0; //report.rumble[3];

				// Rumble to report to UI
				forwarderState.rumble_in[0] = Convert::ToInt32(rumble[0]);
				forwarderState.rumble_in[1] = Convert::ToInt32(rumble[1]);
				forwarderState.rumble_in[2] = Convert::ToInt32(rumble[2]);
				forwarderState.rumble_in[3] = Convert::ToInt32(rumble[3]);
			}
			
			// GPC interpreter
			if(forwarderState.gpcScriptLoaded)
			{
				gpci_Execute(gpcFileName, &output, &rumble);
			}
			
			// Rumble to controller
			if(forwarderState.controllerConnected)
			{
				// Vibrate XInput controller
				// reported as [0 ~ 100] %, XInput range [0 ~ 65535]
				vibration.wRightMotorSpeed = iround(655.35 * (float)rumble[0]);
				vibration.wLeftMotorSpeed = iround(655.35 * (float)rumble[1]);
				XInputSetState(controllerNum, vibration);

				// Rumble to report to UI
				forwarderState.rumble_out[0] = Convert::ToInt32(rumble[0]);
				forwarderState.rumble_out[1] = Convert::ToInt32(rumble[1]);
				forwarderState.rumble_out[2] = Convert::ToInt32(0);
				forwarderState.rumble_out[3] = Convert::ToInt32(0);
			}

			// Output to console
			if(forwarderState.deviceConnected)
			{
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
		if(hInsDeviceAPI != NULL)
		{
			gcdapi_Unload();
		}
		if(hInsGPCInterpreter != NULL)
		{
			gpci_Unload();
		}
		FreeLibrary(hInsDeviceAPI);
		FreeLibrary(hInsGPCInterpreter);
		FreeLibrary(hInsXInput1_3);

		e->Cancel = true;
	}

}

#include "Form1.h"

using namespace CronusMAX_XInput;

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
