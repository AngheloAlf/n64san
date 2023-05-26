#include "n64_wrapper/n64_termination.h"

#if TARGET_GAME_OOT

GAME_INTERNAL_FUNCTION void NORETURN __assert(const char *exp, const char *file, s32 line);

#define ASSERT_FUNC(exp, file, line) __assert(exp, file, line)

#endif

#if TARGET_GAME_MM

#include "n64_wrapper/n64_io.h"

GAME_INTERNAL_FUNCTION void NORETURN Fault_AddHungupAndCrash(const char* filename, s32 line);

#define ASSERT_FUNC(exp, file, line) \
    do { \
        N64Wrapper_Printf("Assertion failed: %s, file %s, line %d\n", exp, file, line); \
        Fault_AddHungupAndCrash(file, line); \
    } while (0)

#endif

void NORETURN N64Wrapper_Assert(const char *exp, const char *file, s32 line) {
    ASSERT_FUNC(exp, file, line);
}
