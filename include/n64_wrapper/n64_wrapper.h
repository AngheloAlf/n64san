#ifndef N64_WRAPPER_H
#define N64_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif


#ifndef N64WRAPPER_DONT_INCLUDE_BUILTIN_ULTRA64
#include "global_headers/libc.h"
#include "global_headers/ultra64.h"
#endif


#ifndef NORETURN
# define NORETURN  __attribute__((noreturn))
#endif


void N64Wrapper_Printf(const char *fmt, ...);
// Like Printf, but prints the current PID before the output string.
void N64Wrapper_Report(const char *fmt, ...);

s32 N64Wrapper_Vsprintf(char* dst, const char* fmt, va_list args);


void NORETURN N64Wrapper_Abort(const char* file, s32 line);
void NORETURN N64Wrapper_Die(const char* file, s32 line);


/*
#define Printf  N64Wrapper_Printf
#define Report  N64Wrapper_Report


#define Abort()  N64Wrapper_Abort(__FILE__, __LINE__)
#define Die()    N64Wrapper_Die(__FILE__, __LINE__)
*/


#ifdef __cplusplus
}
#endif

#endif
