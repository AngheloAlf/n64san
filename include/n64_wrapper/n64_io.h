#ifndef N64_IO_H
#define N64_IO_H

#include "n64_utils.h"

/**
 * Takes a null-terminated C string pointed by `fmt` and prints it.
 * `fmt` may contain format specifiers, in which case the following arguments will be inserted in the resulting string.
 * The format specifiers should be, at minimum, C89 compatible.
 *
 * Where the resulting string is printed to is game dependent.
 */
int N64Wrapper_Vprintf(const char *fmt, va_list args);
int N64Wrapper_Printf(const char *fmt, ...);

/**
 * Takes a null-terminated C string pointed by `fmt` and and writes the results to `dst`.
 * `fmt` may contain format specifiers, in which case the following arguments will be inserted in the resulting string.
 * The format specifiers should be, at minimum, C89 compatible.
 */
s32 N64Wrapper_Vsprintf(char *dst, const char *fmt, va_list args);
s32 N64Wrapper_Sprintf(char *dst, const char *fmt, ...);

#if SANITIZER_DEBUG
#define N64Wrapper_DebugPrintf(...) N64Wrapper_Printf("\t N64San DEBUG: " __VA_ARGS__)
#else
#define N64Wrapper_DebugPrintf(...)
#endif

#endif
