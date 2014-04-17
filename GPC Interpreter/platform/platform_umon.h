
#ifndef PLATFORM_UMON_H
#define PLATFORM_UMON_H

#    define HEAP_SIZE (128*1024)               /* space for the heap and the stack */
#    define NO_FP
#    define BUILTIN_MINI_STDLIB
#    include <stdlib.h>
#    include <string.h>
#    include <ctype.h>
#    include <sys/types.h>
#    include <stdarg.h>
#    include <math.h>
#    include "monlib.h"
#    define assert(x)
#    define malloc mon_malloc
#    define calloc(a,b) mon_malloc(a*b)
#    define realloc mon_realloc
#    define free mon_free

#endif PLATFORM_UMON_H