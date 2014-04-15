
#ifndef PLATFORM_SURVEYOR_H
#define PLATFORM_SURVEYOR_H

#   define HEAP_SIZE C_HEAPSIZE
#   define NO_FP
#   define NO_CTYPE
#   define NO_HASH_INCLUDE
#   define NO_MODULUS
#   include <cdefBF537.h>
#   include "../string.h"
#   include "../print.h"
#   include "../srv.h"
#   include "../setjmp.h"
#   include "../stdarg.h"
#   include "../colors.h"
#   include "../neural.h"
#   include "../gps.h"
#   include "../i2c.h"
#   include "../jpeg.h"
#   include "../malloc.h"
#   include "../xmodem.h"
#   define assert(x)
#   undef BIG_ENDIAN
#   define NO_CALLOC
#   define NO_REALLOC
#   define BROKEN_FLOAT_CASTS
#   define BUILTIN_MINI_STDLIB

#endif // PLATFORM_SURVEYOR_H