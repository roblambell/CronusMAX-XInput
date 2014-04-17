/* gcapi.h
 *
 * Copyright (c) 2011-2014 ISystem Ltda
 *
 * This file is part of Gtuner and Gamepad Proxy Projects
 * http://www.gamepadproxy.com
 *
 * GPP and CM are USB a devices which provides an open programming 
 * interface for computer applications receive input data from game 
 * controllers, and send commands to console (PS4, XBox One, PS3 and 
 * XBox360).
 *
 * This header defines the directives used by the plugin API (Gtuner.exe) 
 * and by the GPP/CM direct API (gcdapi.dll).
 *
 * Changes:
 * 11-Mar-2012: Included <windows.h> to use HMODULE and NULL statements.
 * 14-Feb-2012: gppapi.h replaces the gpp_plugin.h and includes the 
 *              directives for the direct API.
 * 11-Nov-2013: Added Dualshock 4 (PS4 controller) directives.
 * 22-Nov-2013: Added Playstation 4 (console) directives.
 * 26-Nov-2013: Added Xbox One controller directives.
 * 04-Dec-2013: Added Xbox One (console) directives.
 */

#ifndef GCAPI_H
#define GCAPI_H

#include <windows.h>

/* Exact-width Integer Types
 */
#include <stdint.h>


/* GPP/CM Input Port State
 *  The four highest bits defines the controller connected into GPP/CM  
 *  device, and the lower bits defines the controller extension, if 
 *  applicable.
 */
#define     CONTROLLER_DISCONNECTED     0x00
#define     CONTROLLER_PS3              0x10
#define     CONTROLLER_XB360            0x20
#define     CONTROLLER_WII              0x30
#define     CONTROLLER_PS4              0x40
#define     CONTROLLER_XB1              0x50

#define     EXTENSION_NUNCHUK           0x01
#define     EXTENSION_CLASSIC           0x02


/* GPP/CM Output Port State
 *  Defines the possible states for the output port of the GPP/CM device.
 *  The state indicates the output protocol in use and not necessarily 
 *  the console model in which the GPP/CM device is plugged.
 */
#define     CONSOLE_DISCONNECTED           0
#define     CONSOLE_PS3                    1
#define     CONSOLE_XB360                  2
#define     CONSOLE_PS4                    3
#define     CONSOLE_XB1                    4


/* Player LED State
 *  Each player LED of the controller can be in one of four states: 
 *  off, on, blinking rapidly or blinking slowly. These states are 
 *  consistent with what is expected for the PS3. The GPP/CM does 
 *  not reproduce all the possible states for XBox 360 system.
 */
#define     LED_OFF                        0
#define     LED_ON                         1
#define     LED_FAST                       2
#define     LED_SLOW                       3


/* PS4 Input/Output Indexes
 *  The GCAPI implements a generic structure to accommodate all the entries 
 *  of a given system/controller/protocol, below are defined the position 
 *  of the entries for the PS4.
 */
#define     PS4_PS                         0
#define     PS4_SHARE                      1
#define     PS4_OPTIONS                    2
#define     PS4_R1                         3
#define     PS4_R2                         4
#define     PS4_R3                         5
#define     PS4_L1                         6
#define     PS4_L2                         7
#define     PS4_L3                         8
#define     PS4_RX                         9
#define     PS4_RY                        10
#define     PS4_LX                        11
#define     PS4_LY                        12
#define     PS4_UP                        13
#define     PS4_DOWN                      14
#define     PS4_LEFT                      15
#define     PS4_RIGHT                     16
#define     PS4_TRIANGLE                  17
#define     PS4_CIRCLE                    18
#define     PS4_CROSS                     19
#define     PS4_SQUARE                    20
#define     PS4_ACCX                      21
#define     PS4_ACCY                      22
#define     PS4_ACCZ                      23
#define     PS4_GYROX                     24
#define     PS4_GYROY                     25
#define     PS4_GYROZ                     26
#define     PS4_TOUCH                     27
#define     PS4_TOUCHX                    28
#define     PS4_TOUCHY                    29


/* PS3 Input/Output Indexes
 *  Defines the position of the entries for the PS3.
 */
#define     PS3_PS                         0
#define     PS3_SELECT                     1
#define     PS3_START                      2
#define     PS3_R1                         3
#define     PS3_R2                         4
#define     PS3_R3                         5
#define     PS3_L1                         6
#define     PS3_L2                         7
#define     PS3_L3                         8
#define     PS3_RX                         9
#define     PS3_RY                        10
#define     PS3_LX                        11
#define     PS3_LY                        12
#define     PS3_UP                        13
#define     PS3_DOWN                      14
#define     PS3_LEFT                      15
#define     PS3_RIGHT                     16
#define     PS3_TRIANGLE                  17
#define     PS3_CIRCLE                    18
#define     PS3_CROSS                     19
#define     PS3_SQUARE                    20
#define     PS3_ACCX                      21
#define     PS3_ACCY                      22
#define     PS3_ACCZ                      23
#define     PS3_GYRO                      24


/* XBox One Input/Output Indexes
 *  Defines the position of the entries for the XBox One.
 */
#define     XB1_XBOX                       0
#define     XB1_VIEW                       1
#define     XB1_MENU                       2
#define     XB1_RB                         3
#define     XB1_RT                         4
#define     XB1_RS                         5
#define     XB1_LB                         6
#define     XB1_LT                         7
#define     XB1_LS                         8
#define     XB1_RX                         9
#define     XB1_RY                        10
#define     XB1_LX                        11
#define     XB1_LY                        12
#define     XB1_UP                        13
#define     XB1_DOWN                      14
#define     XB1_LEFT                      15
#define     XB1_RIGHT                     16
#define     XB1_Y                         17
#define     XB1_B                         18
#define     XB1_A                         19
#define     XB1_X                         20


/* XBox 360 Input/Output Indexes
 *  Defines the position of the entries for the XBox 360.
 */
#define     XB360_XBOX                     0
#define     XB360_BACK                     1
#define     XB360_START                    2
#define     XB360_RB                       3
#define     XB360_RT                       4
#define     XB360_RS                       5
#define     XB360_LB                       6
#define     XB360_LT                       7
#define     XB360_LS                       8
#define     XB360_RX                       9
#define     XB360_RY                      10
#define     XB360_LX                      11
#define     XB360_LY                      12
#define     XB360_UP                      13
#define     XB360_DOWN                    14
#define     XB360_LEFT                    15
#define     XB360_RIGHT                   16
#define     XB360_Y                       17
#define     XB360_B                       18
#define     XB360_A                       19
#define     XB360_X                       20


/* Wiimote Input Indexes
 *  Defines the position of the entries for the Wiimote.
 */
#define     WII_HOME                       0
#define     WII_MINUS                      1
#define     WII_PLUS                       2
#define     WII_ONE                        5
#define     WII_TWO                        8
#define     WII_UP                        13
#define     WII_DOWN                      14
#define     WII_LEFT                      15
#define     WII_RIGHT                     16
#define     WII_B                         18
#define     WII_A                         19
#define     WII_ACCX                      21
#define     WII_ACCY                      22
#define     WII_ACCZ                      23
#define     WII_IRX                       28
#define     WII_IRY                       29

/* Nunchuk Input Indexes
 */
#define     WII_C                          6
#define     WII_Z                          7
#define     WII_NX                        11
#define     WII_NY                        12
#define     WII_ACCNX                     25
#define     WII_ACCNY                     26
#define     WII_ACCNZ                     27

/* Classic Controller [PRO] Input Indexes
 */
#define     WII_RT                         3
#define     WII_ZR                         4
#define     WII_LT                         6
#define     WII_ZL                         7
#define     WII_RX                         9
#define     WII_RY                        10
#define     WII_LX                        11
#define     WII_LY                        12
#define     WII_X                         17
#define     WII_Y                         20


/* Trace Output Indexes
 *  The TRACE entries were created to check variables and to debug 
 *  GPC scripts. If you are programming a plugin you can use this 
 *  feature to check and plot values on the Gtuner Monitor. These 
 *  entries do not have usability in the direct access API.
 */
#define     TRACE_1                       30
#define     TRACE_2                       31
#define     TRACE_3                       32
#define     TRACE_4                       33
#define     TRACE_5                       34
#define     TRACE_6                       35


/* Input/Output Total Indexes
 *  Total number of indexes of the data structure for inputs and outputs.
 */
#define     GCAPI_INPUT_TOTAL             30
#define     GCAPI_OUTPUT_TOTAL			  36


/* GCAPI Report Type
 *  This data structure is intended to inform the plugin or the application 
 *  about the current state of the GPP/CM input and output ports, about  
 *  parameters set by the console (LED, rumble, battery) and, as well as, 
 *  about the current values of the buttons, sticks and sensors.
 *
 *  Read-only data structure.
 */
typedef struct {
    uint8_t console;             // Receives values established by the #defines CONSOLE_*
    uint8_t controller;          // Values from #defines CONTROLLER_* and EXTENSION_*
    uint8_t led[4];              // Four LED - #defines LED_*
    uint8_t rumble[2];           // Two rumbles - Range: [0 ~ 100] %
    uint8_t battery_level;       // Battery level - Range: [0 ~ 10] 0 = empty, 10 = full
    struct {
        int8_t value;            // Current value - Range: [-100 ~ 100] %
        int8_t prev_value;       // Previous value - Range: [-100 ~ 100] %
        uint32_t press_tv;       // Time marker for the button press event
    } input[GCAPI_INPUT_TOTAL];  // Input structure (for controller entries)
} GCAPI_REPORT;


/* Generic API functions
 *  Type definition of generic (plugin and direct access) API functions. 
 *  The _stdcall calling convention is used to call API functions. 
 *
 *  IsConnected   - Check if the GPP/CM device is connected
 *  GetFWVer      - Get device firmware version
 *  Read          - Read GCDAPI_REPORT (states of device and controller)
 *  Write         - Write output[GCDAPI_OUTPUT_TOTAL] (send it to console)
 *  GetTimeVal    - Get current time value
 *  CalcPressTime - Calculates the pressing time of a entry.
 */
typedef  uint8_t (_stdcall *GCAPI_IsConnected)();
typedef uint16_t (_stdcall *GCAPI_GetFWVer)();
typedef  uint8_t (_stdcall *GCAPI_Read)(GCAPI_REPORT *);
typedef  uint8_t (_stdcall *GCAPI_Write)(int8_t *);
typedef uint32_t (_stdcall *GCAPI_GetTimeVal)();
typedef uint32_t (_stdcall *GCAPI_CalcPressTime)(uint32_t);


/* Direct API functions
 *  Type definition of specific direct API functions. These 
 *  functions can be accessed through the gcdapi.dll library. Don't 
 *  forget to define GCAPI_DIRECT before include this header in order 
 *  to use these typedefs.
 *
 *  Load          - Mandatory! Call after load the gcdapi.dll library
 *  Unload        - Mandatory! Call before unload the gcdapi.dll library
 *  ...           - Plus generic API functions.
 */
#ifdef GCAPI_DIRECT
typedef  uint8_t (_stdcall *GCDAPI_Load)();
typedef     void (_stdcall *GCDAPI_Unload)();
#endif /* GCAPI_DIRECT */


/* Gtuner Plugin API functions - Only avaliable for the GPP/CM devices.
 *  Defines the specific directives of plugin API. Don't forget to 
 *  define GPPAPI_PLUGIN before include this header in order to use these 
 *  directives.
 */
#ifdef GPPAPI_PLUGIN

/* Proc Commands
 *  Definition of the commands identifiers processed by the Proc callback 
 *  function.
 */
#define GPPAPI_GET_MAIN_HWND			0x01
#define GPPAPI_OPEN_GPPMONITOR          0x20
#define GPPAPI_CLOSE_GPPMONITOR         0x21
#define GPPAPI_NEW_SCRIPT               0xB0

/* Proc Callback Function - Type Definition
 *  The Proc function is an application-defined function that processes 
 *  generic commands sent from the Plugin to the Gtuner software (Gtuner.exe). 
 *  The second and third parameters of the Proc() callback function are 
 *  dependent on the command identifier (first parameter). For some 
 *  command identifiers these has values and for some others these are 
 *  either NULL or 0. 
 */
typedef  uint8_t (_stdcall *GPPAPI_Proc)(uint8_t, void *, void *);

/* Exit Callback Function - Type Definition
 *  The plugin must call the Exit function if it want to be unloaded.
 */
typedef     void (_stdcall *GPPAPI_Exit)();

/* Callback List Type
 *  List of callback functions used by the plugin to send commands and 
 *  access the data structures related to the Gtuner API. These functions 
 *  have the same structure and purpose of the exported functions in the 
 *  gcdapi.dll library, which makes easy to use the same code to produce 
 *  both an application or a plugin.
 */
typedef struct {
    GCAPI_IsConnected IsConnected;
    GCAPI_GetFWVer GetFWVer;
    GCAPI_Read Read;
    GCAPI_Write Write;
    GCAPI_GetTimeVal GetTimeVal;
    GCAPI_CalcPressTime CalcPressTime;
    GPPAPI_Exit Exit;
    GPPAPI_Proc Proc;
} GPPAPI_CALLBACKS;

/* Plugin Exported Functions
 *  You should implement (or define an empty function body) those 
 *  functions which are called by Gtuner (Gtuner.exe) plugin manager. 
 *  The _stdcall calling convention is used on exported functions of 
 *  plugin DLL. Functions that use this calling convention require a 
 *  function prototype.
 * 
 *  You will need create a module definition (.def) file to provides the 
 *  linker with information about the plugin exported functions. Create a 
 *  new text file, name it <plugin_name>.def and put it into the project 
 *  directory:
 *
 *  LIBRARY    <plugin_name>
 *
 *  EXPORTS
 *          gpp_PluginName   @1
 *          gpp_Load         @2
 *          gpp_Unload       @3
 *          gpp_ReportUpdate @4
 *
 *  In this file specify the library name, and the name of the exported 
 *  function. Now this .def file should be given as an option to the 
 *  linker. Add <plugin_name>.def as a linker option, or provide 
 *  "<plugin_name>.def" in the "Module definition file" field (Input 
 *  category) in the project properties (Linker options).
 *
 *  gpp_PluginName    - Define and return your plugin name.
 *  gpp_PluginVersion - The first 8 bits are the MajorPart number and the last 
 *                      8 bits are the MinorPart number.
 *  gpp_Load          - It will be called after loading the plugin. Initialize 
 *                      your plugin.
 *  gpp_ReportUpdate  - Called whenever there is an update in Device Report.
 *  gpp_Unload        - It will be called before unloading the plugin. Clean up 
 *                      your plugin.
 */
#ifdef _GPPAPISW_
#include "gswapi.h"
#else
extern const char * _stdcall gpp_PluginName();
extern     uint16_t _stdcall gpp_PluginVersion();
extern      uint8_t _stdcall gpp_Load(HMODULE hmodule, uint16_t sw_version, GPPAPI_CALLBACKS *callbacks);
extern         void _stdcall gpp_ReportUpdate();
extern         void _stdcall gpp_Unload();
#endif /* _GPPAPISW_ */

#endif /* GPPAPI_PLUGIN */

#endif /* GCAPI_H */
