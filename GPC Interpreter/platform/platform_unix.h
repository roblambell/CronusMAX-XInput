
#ifndef PLATFORM_UNIX_H
#define PLATFORM_UNIX_H

# define USE_MALLOC_STACK                   /* stack is allocated using malloc() */
# define USE_MALLOC_HEAP                    /* heap is allocated using malloc() */
# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
# include <assert.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdarg.h>
# include <setjmp.h>
# ifndef NO_FP
#  include <math.h>
#  define PICOC_MATH_LIBRARY
#  define USE_READLINE
#  undef BIG_ENDIAN
#  if defined(__powerpc__) || defined(__hppa__) || defined(__sparc__)
#   define BIG_ENDIAN
#  endif
# endif

extern jmp_buf ExitBuf;
#endif /* PLATFORM_UNIX_H */