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

/*  XInput API Header & Library.
*/
#include <XInput.h>
#pragma comment(lib, "XInput.lib")


namespace ControllerMAX_XInput {

	using namespace System::ComponentModel;

	int iround(double num) {
		return (num > 0.0) ? (int)floor(num + 0.5) : (int)ceil(num - 0.5);
	}

	void XInputVibrate(int controllerNum, int leftVal, int rightVal)
	{
		// Create a Vibration State
		XINPUT_VIBRATION Vibration;

		// Set the Vibration Values
		Vibration.wLeftMotorSpeed = leftVal;
		Vibration.wRightMotorSpeed = rightVal;

		// Vibrate the controller
		XInputSetState(controllerNum, &Vibration);
	}

	public value class FORWARDER_STATE
	{
	public:
		System::String^ errorMessage;
		static bool controllerConnected;
		static bool deviceConnected;
		static array<System::String^> ^buttonActivity = gcnew array<System::String^>(21);
	};

	void XInputForwarder(int controllerNum, BackgroundWorker^ worker, DoWorkEventArgs ^ e )
	{

		bool cancellationPending = worker->CancellationPending;
		ULONGLONG reportTimer = GetTickCount64();

		FORWARDER_STATE forwarderState;

		HINSTANCE hInsGPP = NULL;
		GCAPI_REPORT report = {0};
		int8_t output[GCAPI_OUTPUT_TOTAL] = {0};

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

		// Alternative to XInput API, includes guide button and works with up x360ce
		// https://github.com/DieKatzchen/GuideButtonPoller
		// More info on unnamed ordinals:
		// https://code.google.com/p/x360ce/issues/detail?id=417
		struct ControllerStruct
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

		//First create an HINSTANCE of the xinput1_3.dll
		HINSTANCE hGetProcIDDLL = LoadLibrary(L"xinput1_3.dll");
		if(hGetProcIDDLL == NULL)
		{
			if(!cancellationPending)
			{
				forwarderState.errorMessage = "Error on loading xinput1_3.dll";
				worker->ReportProgress(0, forwarderState);
				cancellationPending = true;
			}
		}

		//Get the address of ordinal 100.
		FARPROC lpfnGetProcessID = GetProcAddress(HMODULE(hGetProcIDDLL), (LPCSTR)100);

		//typedef the function. It takes an int and a pointer to a ControllerStruct and returns an error code
		//as an int.  it's 0 for no error and 1167 for "controller not present".  presumably there are others
		//but I never saw them.  It won't cause a crash on error, it just won't update the data.
		typedef int(__stdcall * pICFUNC)(int, ControllerStruct &);

		//Assign it to XInputGetStateEx for easier use
		pICFUNC XInputGetStateEx;
		XInputGetStateEx = pICFUNC(lpfnGetProcessID);

		//Create in an instance of the ControllerStruct
		ControllerStruct controllerState;

		while ( !cancellationPending )
		{
			if(XInputGetStateEx(controllerNum, controllerState) == 1167)
			{
				forwarderState.controllerConnected = false;
			}
			else
			{
				forwarderState.controllerConnected = true;
			}

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

				forwarderState.buttonActivity[0] = controllerState.guideButton ? "Guide" : "";
				forwarderState.buttonActivity[1] = controllerState.back ? "Back" : "";
				forwarderState.buttonActivity[2] = controllerState.start ? "Start" : "";
				forwarderState.buttonActivity[3] = controllerState.rightShoulder ? "Right Shoulder" : "";
				forwarderState.buttonActivity[4] = percentageRT != 0 ? "Right Trigger, " + percentageRT: "";
				forwarderState.buttonActivity[5] = controllerState.rightThumb ? "Right Analog Stick (Pressed)" : "";
				forwarderState.buttonActivity[6] = controllerState.leftShoulder ? "Left Shoulder" : "";
				forwarderState.buttonActivity[7] = percentageLT != 0 ? "Left Trigger, " + percentageLT: "";
				forwarderState.buttonActivity[8] = controllerState.leftThumb ? "Left Analog Stick (Pressed)" : "";
				forwarderState.buttonActivity[9] = percentageRX != 0 ? "Right Analog Stick X-axis, " + percentageRX: "";
				forwarderState.buttonActivity[10] = percentageRY != 0 ? "Right Analog Stick Y-axis, " + percentageRY: "";
				forwarderState.buttonActivity[11] = percentageLX != 0 ? "Left Analog Stick X-axis, " + percentageLX: "";
				forwarderState.buttonActivity[12] = percentageLY != 0 ? "Left Analog Stick Y-axis, " + percentageLY: "";
				forwarderState.buttonActivity[13] = controllerState.up ? "DPad Up" : "";
				forwarderState.buttonActivity[14] = controllerState.down ? "DPad Down" : "";
				forwarderState.buttonActivity[15] = controllerState.left ? "DPad Left" : "";
				forwarderState.buttonActivity[16] = controllerState.right ? "DPad Right" : "";
				forwarderState.buttonActivity[17] = controllerState.aButton ? "A" : "";
				forwarderState.buttonActivity[18] = controllerState.bButton ? "B" : "";
				forwarderState.buttonActivity[19] = controllerState.xButton ? "X" : "";
				forwarderState.buttonActivity[20] = controllerState.yButton ? "Y" : "";

				// Output to console and XInput controller
				if(forwarderState.deviceConnected)
				{
					gcapi_Read(&report);

					// Vibrate XInput controller
					// reported as [0 ~ 100] %, XInput range [0 ~ 65535]
					int rumbleR = iround(655.35 * (float)report.rumble[0]);
					int rumbleL = iround(655.35 * (float)report.rumble[1]);
					XInputVibrate(controllerNum, rumbleR, rumbleL);

					// Output to console
					int8_t output[GCAPI_OUTPUT_TOTAL] = {0};
					output[0] = controllerState.guideButton ? 100 : 0; // Guide
					output[1] = controllerState.back ? 100 : 0; // Back
					output[2] = controllerState.start ? 100 : 0; // Start
					output[3] = controllerState.rightShoulder ? 100 : 0; // Right Shoulder
					output[4] = percentageRT; // Right Trigger [0 ~ 100] %
					output[5] = controllerState.rightThumb ? 100 : 0; // Right Analog Stick (Pressed)
					output[6] = controllerState.leftShoulder ? 100 : 0; // Left Shoulder
					output[7] = percentageLT; // Left Trigger [0 ~ 100] %
					output[8] = controllerState.leftThumb ? 100 : 0; // Left Analog Stick (Pressed)
					output[9] = percentageRX; // Right Analog Stick X-axis [-100 ~ 100] %
					output[10] = percentageRY; // Right Analog Stick Y-axis [-100 ~ 100] %
					output[11] = percentageLX; // Left Analog Stick X-axis [-100 ~ 100] %
					output[12] = percentageLY; // Left Analog Stick Y-axis [-100 ~ 100] %
					output[13] = controllerState.up ? 100 : 0; // DPad Up
					output[14] = controllerState.down ? 100 : 0; // DPad Down
					output[15] = controllerState.left ? 100 : 0; // DPad Left
					output[16] = controllerState.right ? 100 : 0; // DPad Right
					output[17] = controllerState.yButton ? 100 : 0; // Y
					output[18] = controllerState.bButton ? 100 : 0; // B
					output[19] = controllerState.aButton ? 100 : 0; // A
					output[20] = controllerState.xButton ? 100 : 0; // X
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

			// Wait at least 200ms between reports to UI
			if( (GetTickCount64() - reportTimer) > 100 )
			{
				// TODO: Clone userState before reporting to UI
				worker->ReportProgress(0, forwarderState);
				reportTimer = GetTickCount64();
			}

			Sleep(1);
		}

		// Don't leave them trembling
		XInputVibrate(controllerNum, 0, 0);

		// Free API resources and unload libraries
		if(hInsGPP != NULL)
		{
			gcdapi_Unload();
			
		}
		FreeLibrary(hInsGPP);
		FreeLibrary(hGetProcIDDLL);

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
