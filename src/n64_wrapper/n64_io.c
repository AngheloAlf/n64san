#include "n64_wrapper/n64_io.h"

void* is_proutSyncPrintf(void* arg, const char* str, u32 count);
s32 _Printf(PrintCallback pfn, void* arg, const char* fmt, va_list ap);

s32 vsprintf(char* dst, const char* fmt, va_list args);

void N64Wrapper_Vprintf(const char* fmt, va_list args) {
    _Printf(is_proutSyncPrintf, NULL, fmt, args);
}

void N64Wrapper_Printf(const char *fmt, ...) {
    // Same definition as osSyncPrintf
    va_list args;
    va_start(args, fmt);

    N64Wrapper_Vprintf(fmt, args);

    va_end(args);
}


// Like Printf, but prints the current PID before the output string.
void N64Wrapper_Report(const char *fmt, ...) {
    // This implementation will not print the current PID
    va_list args;
    va_start(args, fmt);

    N64Wrapper_Vprintf(fmt, args);

    va_end(args);
}

s32 N64Wrapper_Vsprintf(char* dst, const char* fmt, va_list args) {
    return vsprintf(dst, fmt, args);
}
