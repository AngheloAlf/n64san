#include "n64_wrapper/n64_termination.h"


void NORETURN __assert(const char* exp, const char* file, s32 line);


void NORETURN N64Wrapper_Abort(const char* file, s32 line) {
    __assert("ASAN Abort", file, line);
}
