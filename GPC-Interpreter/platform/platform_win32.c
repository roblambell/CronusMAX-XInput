#include "../picoc.h"
#include "../interpreter.h"

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

/* mark where to end the program for platforms which require this */
jmp_buf PicocExitBuf;

#ifndef NO_DEBUGGER
#include <signal.h>

static void BreakHandler(int Signal)
{
    PlatformPrintf("break\n");
    DebugManualBreak = TRUE;
}

void PlatformInit()
{
    /* capture the break signal and pass it to the debugger */
    signal(SIGINT, BreakHandler);
}
#else
void PlatformInit()
{
}
#endif

void PlatformCleanup()
{
}

/* get a line of interactive input */
char *PlatformGetLine(char *Buf, int MaxLen, const char *Prompt)
{
#ifdef USE_READLINE
    if (Prompt != NULL)
    {
        /* use GNU readline to read the line */
        char *InLine = readline(Prompt);
        if (InLine == NULL)
            return NULL;
    
        Buf[MaxLen] = '\0';
        strncpy(Buf, InLine, MaxLen-1);
        strncat(Buf, "\n", MaxLen-1);
        
        if (InLine[0] != '\0')
            add_history(InLine);
            
        free(InLine);
        return Buf;
    }
#endif

    if (Prompt != NULL)
        printf("%s", Prompt);
        
    fflush(stdout);
    return fgets(Buf, MaxLen, stdin);
}

/* get a character of interactive input */
int PlatformGetCharacter(void)
{
    fflush(stdout);
    return getchar();
}

/* write a character to the console */
void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
    putchar(OutCh);
}

/* read a file into memory */
char *PlatformReadFile(const char *FileName)
{
    struct stat FileInfo;
    char *ReadText;
    FILE *InFile;
    int BytesRead;
    
    if (stat(FileName, &FileInfo))
        ProgramFail(NULL, "can't read file %s\n", FileName);
    
    ReadText = malloc(FileInfo.st_size + 1);
    if (ReadText == NULL)
        ProgramFail(NULL, "out of memory\n");
        
    InFile = fopen(FileName, "r");
    if (InFile == NULL)
        ProgramFail(NULL, "can't read file %s\n", FileName);
    
    BytesRead = fread(ReadText, 1, FileInfo.st_size, InFile);
    if (BytesRead == 0)
        ProgramFail(NULL, "can't read file %s\n", FileName);

    ReadText[BytesRead] = '\0';
    fclose(InFile);
    
    return ReadText;    
}

/* read and scan a file for definitions */
void PicocPlatformScanFile(const char *FileName)
{
    char *SourceStr = PlatformReadFile(FileName);

    PicocParse(FileName, SourceStr, strlen(SourceStr), TRUE, FALSE, TRUE, TRUE);
}

/* exit the program */
void PlatformExit(int RetVal)
{
    PicocExitValue = RetVal;
    longjmp(PicocExitBuf, 1);
}


#include <time.h>
/* simulates gmtime_r under windows */
struct tm *gmtime_r(const time_t *time, struct tm *result)
{
	//The difference is that gmtime_r(3) is a standard POSIX function. The closest
	//you can find to gmtime_r() on a windows environment is gmtime_s(), which has
	//it's arguments reversed:
	//
	//    gmtime_r(const time_t*, struct tm*)
	//    gmtime_s(struct tm*, const time_t*)
	//
	//Basically, they both convert a time value to a tm structure. gmtime_r then
	//return a pointer to this structure (or NULL if failed), whereas gmtime_s 
	//returns 0 if successful, and a errno_t in case of failure.
	//The tm structure has the following body, as can be seen from both docs listed above:
	//
	//struct tm {
	//    int tm_sec;         /* seconds */
	//    int tm_min;         /* minutes */
	//    int tm_hour;        /* hours */
	//    int tm_mday;        /* day of the month */
	//    int tm_mon;         /* month */
	//    int tm_year;        /* year */
	//    int tm_wday;        /* day of the week */
	//    int tm_yday;        /* day in the year */
	//    int tm_isdst;       /* daylight saving time */
	//};
	
	errno_t gmtime_error;
	gmtime_error = gmtime_s(result, time);
	if (gmtime_error==0)
		return result;
	else
		return NULL;
}
