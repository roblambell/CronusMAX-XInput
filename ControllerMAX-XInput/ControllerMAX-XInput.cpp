// ControllerMAX-XInput.cpp : main project file.

#include "stdafx.h"

#include <math.h>
#include <iostream>
#include <ctime>

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


	bool XInputIsConnected(int controllerNum)
	{
		XINPUT_STATE controllerState;

		// Get the state
		DWORD Result = XInputGetState(controllerNum, &controllerState);

		if(Result == ERROR_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
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
		clock_t reportTimer = clock();

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

		// Guide button is not exposed via XInput API
		// https://github.com/DieKatzchen/GuideButtonPoller
		struct XINPUT_SECRET_STATE
		{
			unsigned long eventCount;  // event counter, increases with every controller event,
			// but for some reason not by one.
			unsigned short up:1, down:1, left:1, right:1, start:1, back:1, l3:1, r3:1,
				lButton:1, rButton:1, guideButton:1, unknown:1, aButton:1,
				bButton:1, xButton:1, yButton:1; // button state bitfield
			unsigned char lTrigger;  // Left Trigger
			unsigned char rTrigger;  // Right Trigger
			short lJoyY;  // Left Joystick Y
			short lJoyx;  // Left Joystick X
			short rJoyY;  // Right Joystick Y 
			short rJoyX;  // Right Joystick X
		};
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
		typedef int(__stdcall * pICFUNC)(int, XINPUT_SECRET_STATE &);
		pICFUNC XInputGetSecretState = pICFUNC(GetProcAddress(HMODULE(hGetProcIDDLL), (LPCSTR)100));

		XINPUT_STATE controllerState;
		XINPUT_SECRET_STATE guideButtonState;


		while ( !cancellationPending )
		{
			forwarderState.controllerConnected = XInputIsConnected(controllerNum);
			forwarderState.deviceConnected = gcapi_IsConnected() ? true : false;

			if(forwarderState.controllerConnected)
			{
				XInputGetState(controllerNum, &controllerState);
				XInputGetSecretState(controllerNum, guideButtonState);

				// Dead zones & normalized values 
				// http://msdn.microsoft.com/en-us/library/windows/desktop/ee417001(v=vs.85).aspx

				// Left Thumb
				float LX = controllerState.Gamepad.sThumbLX;
				float LY = controllerState.Gamepad.sThumbLY;

				// determine how far the controller is pushed	
				float magnitudeL = sqrt(LX*LX + LY*LY); 

				// determine the direction the controller is pushed	
				float normalizedLX = LX / magnitudeL; 
				float normalizedLY = LY / magnitudeL;

				float normalizedMagnitudeL = 0;

				// check if the controller is outside a circular dead zone
				if (magnitudeL > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					// clip the magnitude at its expected maximum value
					if (magnitudeL > 32767) magnitudeL = 32767;

					// adjust magnitude relative to the end of the dead zone
					magnitudeL -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

					// optionally normalize the magnitude with respect to its expected range
					// giving a magnitude value of 0.0 to 1.0
					normalizedMagnitudeL = magnitudeL / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
				}
				else // if the controller is in the dead zone zero out the magnitude
				{
					magnitudeL = 0.0;
					normalizedMagnitudeL = 0.0;
				}

				int8_t percentageLX = int8_t((normalizedLX * normalizedMagnitudeL) * 100);
				int8_t percentageLY = int8_t((normalizedLY * normalizedMagnitudeL) * 100);

				// Right Thumb
				float RX = controllerState.Gamepad.sThumbRX;
				float RY = controllerState.Gamepad.sThumbRY;

				// determine how far the controller is pushed
				float magnitudeR = sqrt(RX*RX + RY*RY);

				// determine the direction the controller is pushed
				float normalizedRX = RX / magnitudeR;
				float normalizedRY = RY / magnitudeR;

				float normalizedMagnitudeR = 0;

				// check if the controller is outside a circular dead zone
				if (magnitudeR > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				{
					// clip the magnitude at its expected maximum value
					if (magnitudeR > 32767) magnitudeR = 32767;

					// adjust magnitude relative to the end of the dead zone
					magnitudeR -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

					// optionally normalize the magnitude with respect to its expected range
					// giving a magnitude value of 0.0 to 1.0
					normalizedMagnitudeR = magnitudeR / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
				}
				else // if the controller is in the dead zone zero out the magnitude
				{
					magnitudeR = 0.0;
					normalizedMagnitudeR = 0.0;
				}
				int8_t percentageRX = int8_t((normalizedRX * normalizedMagnitudeR) * 100);
				int8_t percentageRY = int8_t((normalizedRY * normalizedMagnitudeR) * 100);

				// Left Trigger
				float LT = (float)controllerState.Gamepad.bLeftTrigger;
				int8_t percentageLT = 0;
				if(LT > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				{
					float normalizedLT = (LT - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / (255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
					percentageLT = (int8_t)(normalizedLT * 100);
				}

				// Right Trigger
				float RT = (float)controllerState.Gamepad.bRightTrigger;
				int8_t percentageRT = 0;
				if(RT > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				{
					float normalizedRT = (RT - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / (255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
					percentageRT = (int8_t)(normalizedRT * 100);
				}

				forwarderState.buttonActivity[0] = guideButtonState.guideButton > 0 ? "Guide" : "";
				forwarderState.buttonActivity[1] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_BACK ? "Back" : "";
				forwarderState.buttonActivity[2] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_START ? "Start" : "";
				forwarderState.buttonActivity[3] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_RIGHT_SHOULDER ? "Right Shoulder" : "";
				forwarderState.buttonActivity[4] = percentageRT != 0 ? "Right Trigger, " + percentageRT: "";
				forwarderState.buttonActivity[5] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_RIGHT_THUMB ? "Right Analog Stick (Pressed)" : "";
				forwarderState.buttonActivity[6] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_LEFT_SHOULDER ? "Left Shoulder" : "";
				forwarderState.buttonActivity[7] = percentageLT != 0 ? "Left Trigger, " + percentageLT: "";
				forwarderState.buttonActivity[8] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_LEFT_THUMB ? "Left Analog Stick (Pressed)" : "";
				forwarderState.buttonActivity[9] = percentageRX != 0 ? "Right Analog Stick X-axis, " + percentageRX: "";
				forwarderState.buttonActivity[10] = percentageRY != 0 ? "Right Analog Stick Y-axis, " + percentageRY: "";
				forwarderState.buttonActivity[11] = percentageLX != 0 ? "Left Analog Stick X-axis, " + percentageLX: "";
				forwarderState.buttonActivity[12] = percentageLY != 0 ? "Left Analog Stick Y-axis, " + percentageLY: "";
				forwarderState.buttonActivity[13] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_UP ? "DPad Up" : "";
				forwarderState.buttonActivity[14] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_DOWN ? "DPad Down" : "";
				forwarderState.buttonActivity[15] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_LEFT ? "DPad Left" : "";
				forwarderState.buttonActivity[16] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_RIGHT ? "DPad Right" : "";
				forwarderState.buttonActivity[17] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_A ? "A" : "";
				forwarderState.buttonActivity[18] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_B ? "B" : "";
				forwarderState.buttonActivity[19] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_X ? "X" : "";
				forwarderState.buttonActivity[20] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_Y ? "Y" : "";

				// Output to console and XInput controller
				if(forwarderState.deviceConnected)
				{
					gcapi_Read(&report);

					// Vibrate XInput controller
					// reported as [0 ~ 100] %, XInput range [0 ~ 65535]
					int rumbleR = (int)(65535 * ((float)report.rumble[0] / 100));
					int rumbleL = (int)(65535 * ((float)report.rumble[1] / 100));
					XInputVibrate(controllerNum, rumbleR, rumbleL);

					// Output to console
					int8_t output[GCAPI_OUTPUT_TOTAL] = {0};
					output[0] = guideButtonState.guideButton > 0 ? 100 : 0; // Guide
					output[1] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_BACK ? 100 : 0; // Back
					output[2] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_START ? 100 : 0; // Start
					output[3] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_RIGHT_SHOULDER ? 100 : 0; // Right Shoulder
					output[4] = percentageRT; // Right Trigger [0 ~ 100] %
					output[5] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_RIGHT_THUMB ? 100 : 0; // Right Analog Stick (Pressed)
					output[6] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_LEFT_SHOULDER ? 100 : 0; // Left Shoulder
					output[7] = percentageLT; // Left Trigger [0 ~ 100] %
					output[8] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_LEFT_THUMB ? 100 : 0; // Left Analog Stick (Pressed)
					output[9] = percentageRX; // Right Analog Stick X-axis [-100 ~ 100] %
					output[10] = percentageRY; // Right Analog Stick Y-axis [-100 ~ 100] %
					output[11] = percentageLX; // Left Analog Stick X-axis [-100 ~ 100] %
					output[12] = percentageLY; // Left Analog Stick Y-axis [-100 ~ 100] %
					output[13] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_UP ? 100 : 0; // DPad Up
					output[14] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_DOWN ? 100 : 0; // DPad Down
					output[15] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_LEFT ? 100 : 0; // DPad Left
					output[16] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_DPAD_RIGHT ? 100 : 0; // DPad Right
					output[17] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_Y ? 100 : 0; // Y
					output[18] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_B ? 100 : 0; // B
					output[19] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_X ? 100 : 0; // A
					output[20] = controllerState.Gamepad.wButtons &XINPUT_GAMEPAD_X ? 100 : 0; // X
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

			// Report to UI every 200ms
			if( (clock() - reportTimer) > 10 )
			{
				worker->ReportProgress(0, forwarderState);
				reportTimer = clock();
			}

			Sleep(1);
		}

		// Free API resources and unload libraries.
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
