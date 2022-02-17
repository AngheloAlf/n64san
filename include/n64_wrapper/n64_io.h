#ifndef N64_IO_H
#define N64_IO_H

#include "n64_utils.h"


void N64Wrapper_Vprintf(const char* fmt, va_list args);

void N64Wrapper_Printf(const char *fmt, ...);
// Like Printf, but prints the current PID before the output string.
void N64Wrapper_Report(const char *fmt, ...);

s32 N64Wrapper_Vsprintf(char* dst, const char* fmt, va_list args);


#if SANITIZER_DEBUG
#define N64Wrapper_DebugPrintf(...) N64Wrapper_Printf("\t N64San DEBUG: " __VA_ARGS__)
#else
#define N64Wrapper_DebugPrintf(...)
#endif

#endif
