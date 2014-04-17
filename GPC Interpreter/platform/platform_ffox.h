
#ifndef PLATFORM_FFOX_H
#define PLATFORM_FFOX_H

#  define HEAP_SIZE (16*1024)               /* space for the heap and the stack */
#  define NO_HASH_INCLUDE
#  include <stdlib.h>
#  include <ctype.h>
#  include <string.h>
#  include <sys/types.h>
#  include <stdarg.h>
#  include <setjmp.h>
#  include <math.h>
#  define assert(x)
#  define BUILTIN_MINI_STDLIB
#  undef BIG_ENDIAN

#endif PLATFORM_FFOX_H