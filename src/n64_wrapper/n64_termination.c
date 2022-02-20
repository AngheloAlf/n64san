#include "n64_wrapper/n64_termination.h"

#if TARGET_GAME_OOT

GAME_INTERNAL_FUNCTION void NORETURN __assert(const char *exp, const char *file, s32 line);

void NORETURN N64Wrapper_Assert(const char *exp, const char *file, s32 line) {
    __assert(exp, file, line);
}

#endif
