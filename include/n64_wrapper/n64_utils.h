#ifndef N64_UTILS_H
#define N64_UTILS_H

#ifndef N64WRAPPER_DONT_INCLUDE_BUILTIN_ULTRA64
#include "global_headers/libc.h"
#include "global_headers/ultra64.h"
#endif

#ifndef NORETURN
#define NORETURN  __attribute__((noreturn))
#endif

#define GAME_INTERNAL_FUNCTION

#endif
