/* picoc main program - this varies depending on your operating system and
 * how you're using picoc */
 
/* include only picoc.h here - should be able to use it with only the external interfaces, no internals from interpreter.h */
#include "picoc.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PICOC_STACK_SIZE (128*1024)              /* space for the the stack */

int8_t output[36] = {0};

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) void gpci_Load()
	{
		PicocInitialise(PICOC_STACK_SIZE);
		PicocIncludeAllSystemHeaders();
	}

	__declspec(dllexport) void gpci_Unload()
	{
		PicocCleanup();
	}

	__declspec(dllexport) int gpci_Parse(char *FileName)
	{
		PicocPlatformScanFile(FileName);
		return 1;
	}

	__declspec(dllexport) int gpci_Execute(char *FileName, int8_t *input)
	{
		int8_t i;

		for(i=0; i<36; i++)
		{
			output[i] = input[i];
		}

		PicocCallMain(0, &FileName);

		for(i=0; i<36; i++)
		{
			input[i] = output[i];
		}

		return 0;
	}


#ifdef __cplusplus
}
#endif
