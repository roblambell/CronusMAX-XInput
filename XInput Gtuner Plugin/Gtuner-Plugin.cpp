/*  ControllerMAX-XInput
 *
 *  Plugin for Gtuner
 *  http://www.controllermax.com
 */

#include <math.h>

/*  Define the API model (PLUGIN or DIRECT) before including the 
 *  gcapi.h header file.
 */
#define GPPAPI_PLUGIN
#include "gcapi.h"


/*  Plugin API callback functions. Check gppapi.h for a more detailed 
 *  description.
 */
GCAPI_IsConnected gcapi_IsConnected = NULL;
GCAPI_GetFWVer gcapi_GetFWVer = NULL;
GCAPI_Read gcapi_Read = NULL;
GCAPI_Write gcapi_Write = NULL;
GCAPI_GetTimeVal gcapi_GetTimeVal = NULL;
GCAPI_CalcPressTime gcapi_CalcPressTime = NULL;
GPPAPI_Exit gpp_Exit = NULL;
GPPAPI_Proc gpp_Proc = NULL;


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
	WORD                                wLeftMotorSpeed;
	WORD                                wRightMotorSpeed;
};

int controllerNum = 0;

HINSTANCE hInsXInput1_3 = NULL;

// typedef the function. It takes an int and a pointer to an XInputStateEx and returns an error code
// as an int. It's 0 for no error and 1167 for "controller not present". Presumably there are others
// but I never saw them. It won't cause a crash on error, it just won't update the data.
typedef int(__stdcall * pICFUNC1)(int, XInputStateEx &);
pICFUNC1 XInputGetStateEx;

typedef int(__stdcall * pICFUNC2)(int, XInputVibration &);
pICFUNC2 XInputSetState;

// Create a Controller State
XInputStateEx controllerState;

// Create a Vibration State
XInputVibration vibration;

int iround(double num) {
	return (num > 0.0) ? (int)floor(num + 0.5) : (int)ceil(num - 0.5);
}


/* gpp_PluginName [mandatory]
 *  Provides to the GPP SW the name of your plugin.
 */
const char * _stdcall gpp_PluginName() {
	return("XInput");
}


/* gpp_Load [mandatory]
 *  The GPP software call the gpp_Load when the plugin dll is loaded.
 *  Parameters: the hmodule of DLL
 *              the version of software
 *              pointers to callback functions
 *  Return 1 on success, or return 0 on failure. 
 */
uint8_t _stdcall gpp_Load(HMODULE hmodule, uint16_t sw_version, GPPAPI_CALLBACKS *callbacks) {
	if(sw_version < 0x0131) {
		// Check SW compatibility
		return(0);
	}
	// Set up the callback functions
	gcapi_IsConnected = callbacks->IsConnected;
	gcapi_GetFWVer = callbacks->GetFWVer;
	gcapi_Read = callbacks->Read;
	gcapi_Write = callbacks->Write;
	gcapi_GetTimeVal = callbacks->GetTimeVal;
	gcapi_CalcPressTime = callbacks->CalcPressTime;
	gpp_Exit = callbacks->Exit;
	gpp_Proc = callbacks->Proc;


	// Create hInstance of xinput1_3
	HINSTANCE hInsXInput1_3 = LoadLibrary(L"xinput1_3.dll");
	if(hInsXInput1_3 == NULL)
	{
		return(0);
	}

	// Alternative to XInputGetState
	// https://github.com/DieKatzchen/GuideButtonPoller
	// Details on unnamed ordinals:
	// https://code.google.com/p/x360ce/issues/detail?id=417

	// Get the address of ordinal 100 (unnamed: XInputGetStateEx) - exposes Guide button
	FARPROC pointerToDLLFunction1 = GetProcAddress(HMODULE(hInsXInput1_3), (LPCSTR)100);

	// Assign XInputGetStateEx for easier use
	XInputGetStateEx = pICFUNC1(pointerToDLLFunction1);

	// Get the pointer to XInputSetState (ordinal 3)
	FARPROC pointerToDLLFunction2 = GetProcAddress(HMODULE(hInsXInput1_3), "XInputSetState");

	// Assign XInputSetState for easier use
	XInputSetState = pICFUNC2(pointerToDLLFunction2);
	
	return(1);
}


/* gpp_Unload [optional]
 *  The GPP software call the gpp_Unload (if it exist) when is need to 
 *  immediately quit the plugin, for example, in case the user has closed 
 *  the GPP application. Use this function to free the memory and resources. 
 */
void _stdcall gpp_Unload() {

	FreeLibrary(hInsXInput1_3);

	return;
}


/* gpp_ReportUpdate [optional]
 *  The GPP software call the gpp_ReportUpdate function whenever it receives 
 *  a new set of data from GPP device. You can use this function to read the 
 *  input data (from controller) and/or to set the output data (which will be 
 *  sent to the console). 
 */
void _stdcall gpp_ReportUpdate() {
	GCAPI_REPORT report;
	int8_t output[GCAPI_OUTPUT_TOTAL] = {0};

	DWORD result = XInputGetStateEx(controllerNum, controllerState);

	bool controllerConnected = (result == ERROR_SUCCESS ? true : false);

	if(gcapi_Read)
	{
		gcapi_Read(&report);
		
		if(controllerConnected)
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

			// Vibrate XInput controller
			// reported as [0 ~ 100] %, XInput range [0 ~ 65535]
			vibration.wRightMotorSpeed = iround(655.35 * (float)report.rumble[0]);
			vibration.wLeftMotorSpeed = iround(655.35 * (float)report.rumble[1]);
			XInputSetState(controllerNum, vibration);

			// Output to console
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
		}
		else
		{
			// Passthrough mode
			// Input from controller to console
			for(uint8_t i=0; i<GCAPI_INPUT_TOTAL; i++) {
				output[i] = report.input[i].value;
			}
		}

		if(gcapi_Write) {
			gcapi_Write(output);
		}

	}

	return;
}
