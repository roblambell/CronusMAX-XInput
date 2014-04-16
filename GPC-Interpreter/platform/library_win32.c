#include "../interpreter.h"
#include <stdint.h>

void Win32SetupFunc(void)
{    
}

int8_t output[36];

void Cget_val (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    ReturnValue->Val->Integer = output[Param[0]->Val->Integer];
}

void Cset_val (struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
	int button = Param[0]->Val->Integer;
	int value = Param[1]->Val->Integer;
	output[button] = value;
}

/* list of all library functions and their prototypes */
struct LibraryFunction Win32Functions[] =
{
	{ Cget_val,     "int get_val(int);" },
	{ Cset_val,     "int set_val(int, int);" },
    { NULL,         NULL }
};

/* PS4 Input/Output Indexes
 *  The GCAPI implements a generic structure to accommodate all the entries 
 *  of a given system/controller/protocol, below are defined the position 
 *  of the entries for the PS4.
 */
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


/* PS3 Input/Output Indexes
 *  Defines the position of the entries for the PS3.
 */
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


/* XBox One Input/Output Indexes
 *  Defines the position of the entries for the XBox One.
 */
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


/* XBox 360 Input/Output Indexes
 *  Defines the position of the entries for the XBox 360.
 */
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


/* Wiimote Input Indexes
 *  Defines the position of the entries for the Wiimote.
 */
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

/* Nunchuk Input Indexes
 */
int WII_C =         6;
int WII_Z =         7;
int WII_NX =       11;
int WII_NY =       12;
int WII_ACCNX =    25;
int WII_ACCNY =    26;
int WII_ACCNZ =    27;

/* Classic Controller [PRO] Input Indexes
 */
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


}
