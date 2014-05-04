#include "../interpreter.h"
#include <stdint.h>

void Win32SetupFunc(void)
{    
}

int8_t output[21];
int8_t rumble_out[4];

int iround(double num) {
	return (num > 0.0) ? (int)floor(num + 0.5) : (int)ceil(num - 0.5);
}

/* GPC I/O Functions 
 *
 * get_val       Returns the current value of a controller entry
 * set_val       Overwrites the current value of a controller entry
 * get_lval      Returns the previous value of a controller entry
 * get_ptime     Returns the elapsed activation time (pressing) of a controller entry
 * event_press   Returns TRUE when detected activation (pressing) of a controller entry
 * event_release Returns TRUE when detected deactivation (releasing) of a controller entry
 * swap          Swap the values between two controller entries
 * block         Blocks forwarding of a controller entry for a short period of time
 * sensitivity   Adjust the sensitivity of a controller entry
 * deadzone      Remove the deadzone of a pair of entries, usually of analog sticks
 * stickize      Transform the values of mouse input (or Wiimote IR) to analog stick
 *
 * Rumble
 *
 * get_rumble    Returns the current value of a rumble
 * set_rumble    Set the value of a rumble (engine speed)
 * block_rumble  Blocks any rumble action sent by the console
 * reset_rumble  Unblocks the rumble (if it is blocked)
 *
 */

// GPC I/O Functions

// int get_val ( <identifier> )
// Returns the current value of a controller entry
void Cget_val (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = output[Param[0]->Val->Integer];
}

// set_val ( <identifier>, <value> )
// Overwrites the current value of a controller entry
void Cset_val (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	int button = Param[0]->Val->Integer;
	int value = Param[1]->Val->Integer;
	output[button] = value;
}

// TODO: int get_lval ( <identifier> )
// Returns the previous value of a controller entry
void Cget_lval (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = 0;
}

// TODO: int get_ptime ( <identifier> )
// Returns the elapsed activation time (pressing) of a controller entry
void Cget_ptime (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = 0;
}

// TODO: int event_press ( <identifier> )
// Returns TRUE when detected activation (pressing) of a controller entry
void Cevent_press (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = 0;
}

// TODO: int event_release ( <identifier> )
// Returns TRUE when detected deactivation (releasing) of a controller entry
void Cevent_release (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = 0;
}

// swap ( <identifier1>, <identifier2> )
// Swap the values between two controller entries
void Cswap (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	int button1 = Param[0]->Val->Integer;
	int button2 = Param[1]->Val->Integer;

	int button1_val = output[button1];
	int button2_val = output[button2];

	output[button1] = button2_val;
	output[button2] = button1_val;
}

// TODO: block ( <identifier>, <time> )
// Blocks forwarding of a controller entry for a short period of time
void Cblock (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{

}

// TODO: sensitivity ( <identifier>, <midpoint>, <sensitivity> )
// Adjust the sensitivity of a controller entry
void Csensitivity (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	int button = Param[0]->Val->Integer;
	int input_val = output[button];
	int midpoint = Param[1]->Val->Integer;
	int sensitivity = Param[1]->Val->Integer;

	int total;
	int output_val;

	if(input_val > midpoint)
	{
		total = (100 - midpoint) * 2;
		output_val = ((100 - midpoint) + (input_val - midpoint)) / total;
	}
	else
	{
		total = midpoint * 2;
		output_val = input_val / total;
	}
	
	output[button] = iround(output_val * sensitivity);
}

// TODO: deadzone ( <identifierX>, <identifierY>, <xdz_cir>, <ydz_rad> )
// Remove the deadzone of a pair of entries, usually of analog sticks
void Cdeadzone (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	
}

// TODO: stickize ( <identifierX>, <identifierY>, <radius> )
// Transform the values of mouse input (or Wiimote IR) to analog stick
void Cstickize (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	
}

// Rumble

// int get_rumble ( <identifier> )
// Returns the current value of a rumble
void Cget_rumble (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = rumble_out[Param[0]->Val->Integer];
}

// set_rumble ( <identifier>, <value> )
// Set the value of a rumble (engine speed)
void Cset_rumble (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	int rumble_ident = Param[0]->Val->Integer;
	int value = Param[1]->Val->Integer;
	rumble_out[rumble_ident] = value;
}

/* list of all library functions and their prototypes */
struct LibraryFunction Win32Functions[] =
{
	// GPC I/O Functions
	{ Cget_val,       "int get_val(int);" },                  // int get_val ( <identifier> )
	{ Cset_val,       "void set_val(int, int);" },            // set_val ( <identifier>, <value> )
	{ Cget_lval,      "int get_lval(int);" },                 // int get_lval ( <identifier> )
	{ Cget_ptime,     "int get_ptime(int);" },                // int get_ptime ( <identifier> )
	{ Cevent_press,   "int event_press(int);" },              // int event_press ( <identifier> )
	{ Cevent_release, "int event_release(int);" },            // int event_release ( <identifier> )
	{ Cswap,          "void swap(int, int);" },               // swap ( <identifier1>, <identifier2> )
	{ Cblock,         "void block(int, int);" },              // block ( <identifier>, <time> )
	{ Csensitivity,   "void sensitivity(int, int, int);" },   // sensitivity ( <identifier>, <midpoint>, <sensitivity> )
	{ Cdeadzone,      "void deadzone(int, int, int, int);" }, // deadzone ( <identifierX>, <identifierY>, <xdz_cir>, <ydz_rad> )
	{ Cstickize,      "void stickize(int, int, int);" },      // stickize ( <identifierX>, <identifierY>, <radius> )
	// Rumble
	{ Cget_rumble,    "int get_rumble(int);" },               // int get_rumble ( <rumble_ident> )
	{ Cset_rumble,    "void set_rumble(int, int);" },         // set_rumble ( <rumble_ident>, <value> )
    { NULL,           NULL }
};

/* Controller Input/Output Indexes */
// PS4 Input/Output Indexes
int PS4_PS =        0;
int PS4_SHARE =     1;
int PS4_OPTIONS =   2;
int PS4_R1 =        3;
int PS4_R2 =        4;
int PS4_R3 =        5;
int PS4_L1 =        6;
int PS4_L2 =        7;
int PS4_L3 =        8;
int PS4_RX =        9;
int PS4_RY =       10;
int PS4_LX =       11;
int PS4_LY =       12;
int PS4_UP =       13;
int PS4_DOWN =     14;
int PS4_LEFT =     15;
int PS4_RIGHT =    16;
int PS4_TRIANGLE = 17;
int PS4_CIRCLE =   18;
int PS4_CROSS =    19;
int PS4_SQUARE =   20;
int PS4_ACCX =     21;
int PS4_ACCY =     22;
int PS4_ACCZ =     23;
int PS4_GYROX =    24;
int PS4_GYROY =    25;
int PS4_GYROZ =    26;
int PS4_TOUCH =    27;
int PS4_TOUCHX =   28;
int PS4_TOUCHY =   29;

// PS3 Input/Output Indexes
int PS3_PS =        0;
int PS3_SELECT =    1;
int PS3_START =     2;
int PS3_R1 =        3;
int PS3_R2 =        4;
int PS3_R3 =        5;
int PS3_L1 =        6;
int PS3_L2 =        7;
int PS3_L3 =        8;
int PS3_RX =        9;
int PS3_RY =       10;
int PS3_LX =       11;
int PS3_LY =       12;
int PS3_UP =       13;
int PS3_DOWN =     14;
int PS3_LEFT =     15;
int PS3_RIGHT =    16;
int PS3_TRIANGLE = 17;
int PS3_CIRCLE =   18;
int PS3_CROSS =    19;
int PS3_SQUARE =   20;
int PS3_ACCX =     21;
int PS3_ACCY =     22;
int PS3_ACCZ =     23;
int PS3_GYRO =     24;

// XBox One Input/Output Indexes
int XB1_XBOX =      0;
int XB1_VIEW =      1;
int XB1_MENU =      2;
int XB1_RB =        3;
int XB1_RT =        4;
int XB1_RS =        5;
int XB1_LB =        6;
int XB1_LT =        7;
int XB1_LS =        8;
int XB1_RX =        9;
int XB1_RY =       10;
int XB1_LX =       11;
int XB1_LY =       12;
int XB1_UP =       13;
int XB1_DOWN =     14;
int XB1_LEFT =     15;
int XB1_RIGHT =    16;
int XB1_Y =        17;
int XB1_B =        18;
int XB1_A =        19;
int XB1_X =        20;

// XBox 360 Input/Output Indexes
int XB360_XBOX =    0;
int XB360_BACK =    1;
int XB360_START =   2;
int XB360_RB =      3;
int XB360_RT =      4;
int XB360_RS =      5;
int XB360_LB =      6;
int XB360_LT =      7;
int XB360_LS =      8;
int XB360_RX =      9;
int XB360_RY =     10;
int XB360_LX =     11;
int XB360_LY =     12;
int XB360_UP =     13;
int XB360_DOWN =   14;
int XB360_LEFT =   15;
int XB360_RIGHT =  16;
int XB360_Y =      17;
int XB360_B =      18;
int XB360_A =      19;
int XB360_X =      20;

// Wiimote Input Indexes
int WII_HOME =      0;
int WII_MINUS =     1;
int WII_PLUS =      2;
int WII_ONE =       5;
int WII_TWO =       8;
int WII_UP =       13;
int WII_DOWN =     14;
int WII_LEFT =     15;
int WII_RIGHT =    16;
int WII_B =        18;
int WII_A =        19;
int WII_ACCX =     21;
int WII_ACCY =     22;
int WII_ACCZ =     23;
int WII_IRX =      28;
int WII_IRY =      29;

// Nunchuk Input Indexes
int WII_C =         6;
int WII_Z =         7;
int WII_NX =       11;
int WII_NY =       12;
int WII_ACCNX =    25;
int WII_ACCNY =    26;
int WII_ACCNZ =    27;

// Classic Controller [PRO] Input Indexes
int WII_RT =        3;
int WII_ZR =        4;
int WII_LT =        6;
int WII_ZL =        7;
int WII_RX =        9;
int WII_RY =       10;
int WII_LX =       11;
int WII_LY =       12;
int WII_X =        17;
int WII_Y =        20;

// Rumble
int RUMBLE_A =     0;
int RUMBLE_B =     1;
int RUMBLE_LT =    2;
int RUMBLE_RT =    3;

// GPC constants
int DZ_CIRCLE =    101;

void PlatformLibraryInit(void)
{
	IncludeRegister("picoc_unix.h", &Win32SetupFunc, &Win32Functions[0], NULL);

	VariableDefinePlatformVar(NULL, "PS4_PS", &IntType, (union AnyValue *)&PS4_PS, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_SHARE", &IntType, (union AnyValue *)&PS4_SHARE, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_OPTIONS", &IntType, (union AnyValue *)&PS4_OPTIONS, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_R1", &IntType, (union AnyValue *)&PS4_R1, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_R2", &IntType, (union AnyValue *)&PS4_R2, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_R3", &IntType, (union AnyValue *)&PS4_R3, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_L1", &IntType, (union AnyValue *)&PS4_L1, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_L2", &IntType, (union AnyValue *)&PS4_L2, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_L3", &IntType, (union AnyValue *)&PS4_L3, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_RX", &IntType, (union AnyValue *)&PS4_RX, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_RY", &IntType, (union AnyValue *)&PS4_RY, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_LX", &IntType, (union AnyValue *)&PS4_LX, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_LY", &IntType, (union AnyValue *)&PS4_LY, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_UP", &IntType, (union AnyValue *)&PS4_UP, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_DOWN", &IntType, (union AnyValue *)&PS4_DOWN, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_LEFT", &IntType, (union AnyValue *)&PS4_LEFT, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_RIGHT", &IntType, (union AnyValue *)&PS4_RIGHT, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_TRIANGLE", &IntType, (union AnyValue *)&PS4_TRIANGLE, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_CIRCLE", &IntType, (union AnyValue *)&PS4_CIRCLE, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_CROSS", &IntType, (union AnyValue *)&PS4_CROSS, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_SQUARE", &IntType, (union AnyValue *)&PS4_SQUARE, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_ACCX", &IntType, (union AnyValue *)&PS4_ACCX, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_ACCY", &IntType, (union AnyValue *)&PS4_ACCY, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_ACCZ", &IntType, (union AnyValue *)&PS4_ACCZ, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_GYROX", &IntType, (union AnyValue *)&PS4_GYROX, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_GYROY", &IntType, (union AnyValue *)&PS4_GYROY, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_GYROZ", &IntType, (union AnyValue *)&PS4_GYROZ, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_TOUCH", &IntType, (union AnyValue *)&PS4_TOUCH, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_TOUCHX", &IntType, (union AnyValue *)&PS4_TOUCHX, TRUE);
	VariableDefinePlatformVar(NULL, "PS4_TOUCHY", &IntType, (union AnyValue *)&PS4_TOUCHY, TRUE);

	VariableDefinePlatformVar(NULL, "PS3_PS", &IntType, (union AnyValue *)&PS3_PS, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_SELECT", &IntType, (union AnyValue *)&PS3_SELECT, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_START", &IntType, (union AnyValue *)&PS3_START, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_R1", &IntType, (union AnyValue *)&PS3_R1, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_R2", &IntType, (union AnyValue *)&PS3_R2, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_R3", &IntType, (union AnyValue *)&PS3_R3, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_L1", &IntType, (union AnyValue *)&PS3_L1, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_L2", &IntType, (union AnyValue *)&PS3_L2, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_L3", &IntType, (union AnyValue *)&PS3_L3, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_RX", &IntType, (union AnyValue *)&PS3_RX, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_RY", &IntType, (union AnyValue *)&PS3_RY, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_LX", &IntType, (union AnyValue *)&PS3_LX, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_LY", &IntType, (union AnyValue *)&PS3_LY, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_UP", &IntType, (union AnyValue *)&PS3_UP, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_DOWN", &IntType, (union AnyValue *)&PS3_DOWN, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_LEFT", &IntType, (union AnyValue *)&PS3_LEFT, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_RIGHT", &IntType, (union AnyValue *)&PS3_RIGHT, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_TRIANGLE", &IntType, (union AnyValue *)&PS3_TRIANGLE, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_CIRCLE", &IntType, (union AnyValue *)&PS3_CIRCLE, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_CROSS", &IntType, (union AnyValue *)&PS3_CROSS, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_SQUARE", &IntType, (union AnyValue *)&PS3_SQUARE, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_ACCX", &IntType, (union AnyValue *)&PS3_ACCX, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_ACCY", &IntType, (union AnyValue *)&PS3_ACCY, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_ACCZ", &IntType, (union AnyValue *)&PS3_ACCZ, TRUE);
	VariableDefinePlatformVar(NULL, "PS3_GYRO", &IntType, (union AnyValue *)&PS3_GYRO, TRUE);

	VariableDefinePlatformVar(NULL, "XB1_XBOX", &IntType, (union AnyValue *)&XB1_XBOX, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_VIEW", &IntType, (union AnyValue *)&XB1_VIEW, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_MENU", &IntType, (union AnyValue *)&XB1_MENU, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_RB", &IntType, (union AnyValue *)&XB1_RB, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_RT", &IntType, (union AnyValue *)&XB1_RT, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_RS", &IntType, (union AnyValue *)&XB1_RS, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_LB", &IntType, (union AnyValue *)&XB1_LB, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_LT", &IntType, (union AnyValue *)&XB1_LT, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_LS", &IntType, (union AnyValue *)&XB1_LS, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_RX", &IntType, (union AnyValue *)&XB1_RX, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_RY", &IntType, (union AnyValue *)&XB1_RY, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_LX", &IntType, (union AnyValue *)&XB1_LX, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_LY", &IntType, (union AnyValue *)&XB1_LY, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_UP", &IntType, (union AnyValue *)&XB1_UP, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_DOWN", &IntType, (union AnyValue *)&XB1_DOWN, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_LEFT", &IntType, (union AnyValue *)&XB1_LEFT, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_RIGHT", &IntType, (union AnyValue *)&XB1_RIGHT, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_Y", &IntType, (union AnyValue *)&XB1_Y, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_B", &IntType, (union AnyValue *)&XB1_B, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_A", &IntType, (union AnyValue *)&XB1_A, TRUE);
	VariableDefinePlatformVar(NULL, "XB1_X", &IntType, (union AnyValue *)&XB1_X, TRUE);

	VariableDefinePlatformVar(NULL, "XB360_XBOX", &IntType, (union AnyValue *)&XB360_XBOX, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_BACK", &IntType, (union AnyValue *)&XB360_BACK, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_START", &IntType, (union AnyValue *)&XB360_START, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_RB", &IntType, (union AnyValue *)&XB360_RB, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_RT", &IntType, (union AnyValue *)&XB360_RT, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_RS", &IntType, (union AnyValue *)&XB360_RS, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_LB", &IntType, (union AnyValue *)&XB360_LB, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_LT", &IntType, (union AnyValue *)&XB360_LT, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_LS", &IntType, (union AnyValue *)&XB360_LS, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_RX", &IntType, (union AnyValue *)&XB360_RX, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_RY", &IntType, (union AnyValue *)&XB360_RY, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_LX", &IntType, (union AnyValue *)&XB360_LX, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_LY", &IntType, (union AnyValue *)&XB360_LY, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_UP", &IntType, (union AnyValue *)&XB360_UP, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_DOWN", &IntType, (union AnyValue *)&XB360_DOWN, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_LEFT", &IntType, (union AnyValue *)&XB360_LEFT, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_RIGHT", &IntType, (union AnyValue *)&XB360_RIGHT, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_Y", &IntType, (union AnyValue *)&XB360_Y, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_B", &IntType, (union AnyValue *)&XB360_B, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_A", &IntType, (union AnyValue *)&XB360_A, TRUE);
	VariableDefinePlatformVar(NULL, "XB360_X", &IntType, (union AnyValue *)&XB360_X, TRUE);

	VariableDefinePlatformVar(NULL, "WII_HOME", &IntType, (union AnyValue *)&WII_HOME, TRUE);
	VariableDefinePlatformVar(NULL, "WII_MINUS", &IntType, (union AnyValue *)&WII_MINUS, TRUE);
	VariableDefinePlatformVar(NULL, "WII_PLUS", &IntType, (union AnyValue *)&WII_PLUS, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ONE", &IntType, (union AnyValue *)&WII_ONE, TRUE);
	VariableDefinePlatformVar(NULL, "WII_TWO", &IntType, (union AnyValue *)&WII_TWO, TRUE);
	VariableDefinePlatformVar(NULL, "WII_UP", &IntType, (union AnyValue *)&WII_UP, TRUE);
	VariableDefinePlatformVar(NULL, "WII_DOWN", &IntType, (union AnyValue *)&WII_DOWN, TRUE);
	VariableDefinePlatformVar(NULL, "WII_LEFT", &IntType, (union AnyValue *)&WII_LEFT, TRUE);
	VariableDefinePlatformVar(NULL, "WII_RIGHT", &IntType, (union AnyValue *)&WII_RIGHT, TRUE);
	VariableDefinePlatformVar(NULL, "WII_B", &IntType, (union AnyValue *)&WII_B, TRUE);
	VariableDefinePlatformVar(NULL, "WII_A", &IntType, (union AnyValue *)&WII_A, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ACCX", &IntType, (union AnyValue *)&WII_ACCX, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ACCY", &IntType, (union AnyValue *)&WII_ACCY, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ACCZ", &IntType, (union AnyValue *)&WII_ACCZ, TRUE);
	VariableDefinePlatformVar(NULL, "WII_IRX", &IntType, (union AnyValue *)&WII_IRX, TRUE);
	VariableDefinePlatformVar(NULL, "WII_IRY", &IntType, (union AnyValue *)&WII_IRY, TRUE);

	VariableDefinePlatformVar(NULL, "WII_C", &IntType, (union AnyValue *)&WII_C, TRUE);
	VariableDefinePlatformVar(NULL, "WII_Z", &IntType, (union AnyValue *)&WII_Z, TRUE);
	VariableDefinePlatformVar(NULL, "WII_NX", &IntType, (union AnyValue *)&WII_NX, TRUE);
	VariableDefinePlatformVar(NULL, "WII_NY", &IntType, (union AnyValue *)&WII_NY, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ACCNX", &IntType, (union AnyValue *)&WII_ACCNX, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ACCNY", &IntType, (union AnyValue *)&WII_ACCNY, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ACCNZ", &IntType, (union AnyValue *)&WII_ACCNZ, TRUE);

	VariableDefinePlatformVar(NULL, "WII_RT", &IntType, (union AnyValue *)&WII_RT, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ZR", &IntType, (union AnyValue *)&WII_ZR, TRUE);
	VariableDefinePlatformVar(NULL, "WII_LT", &IntType, (union AnyValue *)&WII_LT, TRUE);
	VariableDefinePlatformVar(NULL, "WII_ZL", &IntType, (union AnyValue *)&WII_ZL, TRUE);
	VariableDefinePlatformVar(NULL, "WII_RX", &IntType, (union AnyValue *)&WII_RX, TRUE);
	VariableDefinePlatformVar(NULL, "WII_RY", &IntType, (union AnyValue *)&WII_RY, TRUE);
	VariableDefinePlatformVar(NULL, "WII_LX", &IntType, (union AnyValue *)&WII_LX, TRUE);
	VariableDefinePlatformVar(NULL, "WII_LY", &IntType, (union AnyValue *)&WII_LY, TRUE);
	VariableDefinePlatformVar(NULL, "WII_X", &IntType, (union AnyValue *)&WII_X, TRUE);
	VariableDefinePlatformVar(NULL, "WII_Y", &IntType, (union AnyValue *)&WII_Y, TRUE);

	VariableDefinePlatformVar(NULL, "RUMBLE_A", &IntType, (union AnyValue *)&RUMBLE_A, TRUE);
	VariableDefinePlatformVar(NULL, "RUMBLE_B", &IntType, (union AnyValue *)&RUMBLE_B, TRUE);
	VariableDefinePlatformVar(NULL, "RUMBLE_LT", &IntType, (union AnyValue *)&RUMBLE_LT, TRUE);
	VariableDefinePlatformVar(NULL, "RUMBLE_RT", &IntType, (union AnyValue *)&RUMBLE_RT, TRUE);

	VariableDefinePlatformVar(NULL, "DZ_CIRCLE", &IntType, (union AnyValue *)&DZ_CIRCLE, TRUE);
}
