/* picoc main program - this varies depending on your operating system and
 * how you're using picoc */
 
/* include only picoc.h here - should be able to use it with only the external interfaces, no internals from interpreter.h */
#include "picoc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PICOC_STACK_SIZE (128*1024)              /* space for the the stack */

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) int gpci_Load()
	{
		PicocInitialise(PICOC_STACK_SIZE);
		PicocIncludeAllSystemHeaders();
		return 0;
	}

	__declspec(dllexport) void gpci_Unload()
	{
		PicocCleanup();
	}

	__declspec(dllexport) int gpci_Parse(char *FileName)
	{
		PicocPlatformScanFile(FileName);
		return 0;
	}

	__declspec(dllexport) void gpci_Execute(char *FileName)
	{
		PicocCallMain(0, &FileName);
	}


#ifdef __cplusplus
}
#endif
