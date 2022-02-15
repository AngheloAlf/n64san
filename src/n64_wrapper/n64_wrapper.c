#include "n64_wrapper/n64_wrapper.h"

void* is_proutSyncPrintf(void* arg, const char* str, u32 count);
s32 _Printf(PrintCallback pfn, void* arg, const char* fmt, va_list ap);

s32 vsprintf(char* dst, const char* fmt, va_list args);

void NORETURN __assert(const char* exp, const char* file, s32 line);


void N64Wrapper_Printf(const char *fmt, ...) {
    // Same definition as osSyncPrintf
    va_list args;
    va_start(args, fmt);

    _Printf(is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}


// Like Printf, but prints the current PID before the output string.
void N64Wrapper_Report(const char *fmt, ...) {
    // This implementation will not print the current PID
    va_list args;
    va_start(args, fmt);

    _Printf(is_proutSyncPrintf, NULL, fmt, args);

    va_end(args);
}

s32 N64Wrapper_Vsprintf(char* dst, const char* fmt, va_list args) {
    return vsprintf(dst, fmt, args);
}



void N64Wrapper_Abort(const char* file, s32 line) {
    __assert("ASAN Abort", file, line);
}

void N64Wrapper_Die(const char* file, s32 line) {
    __assert("ASAN Die", file, line);
}
