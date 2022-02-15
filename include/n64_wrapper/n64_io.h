#ifndef N64_IO_H
#define N64_IO_H

#include "n64_utils.h"


void N64Wrapper_Printf(const char *fmt, ...);
// Like Printf, but prints the current PID before the output string.
void N64Wrapper_Report(const char *fmt, ...);

s32 N64Wrapper_Vsprintf(char* dst, const char* fmt, va_list args);


void NORETURN N64Wrapper_Abort(const char* file, s32 line);
void NORETURN N64Wrapper_Die(const char* file, s32 line);


#endif
