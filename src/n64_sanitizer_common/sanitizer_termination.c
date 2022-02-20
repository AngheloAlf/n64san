/**
 * Based on llvm-project
 * https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_termination.cpp
 */

#include "global_headers/libc.h"
#include "global_headers/ultra64.h"

#include "sanitizer_common/sanitizer_common.h"

#include "n64_wrapper/n64_wrapper.h"

void NORETURN Die() {
    N64Wrapper_Assert("SAN: Die()\n", __FILE__, __LINE__);
}

// TODO
tid_t GetTid() {
    return 0;
}

void NORETURN CheckFailed(const char *file, int line, const char *cond, u64 v1, u64 v2) {
    u32 tid = GetTid();

    N64Wrapper_Printf("%s: CHECK failed: %s:%d \"%s\" (0x%zx, 0x%zx) (tid=%u)\n", SanitizerToolName, file, line, cond,
                      v1, v2, tid);

    N64Wrapper_Assert(cond, file, line);
}
