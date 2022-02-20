#ifndef SANITIZER_STACKTRACE_H
#define SANITIZER_STACKTRACE_H

/**
 * Based on llvm-project
 * https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_stacktrace.h
 */

#include "sanitizer_common.h"
#include "sanitizer_internal_defs.h"
#include "sanitizer_platform.h"

#define SANITIZER_CAN_FAST_UNWIND 0
#define SANITIZER_CAN_SLOW_UNWIND 0

// Use this macro if you want to print stack trace with the caller
// of the current function in the top frame.
#define GET_CALLER_PC_BP           \
    uptr bp = GET_CURRENT_FRAME(); \
    uptr pc = GET_CALLER_PC();

#define GET_CALLER_PC_BP_SP \
    GET_CALLER_PC_BP;       \
    uptr local_stack;       \
    uptr sp = (uptr)&local_stack

// Use this macro if you want to print stack trace with the current
// function in the top frame.
#define GET_CURRENT_PC_BP          \
    uptr bp = GET_CURRENT_FRAME(); \
    uptr pc = StackTrace::GetCurrentPc()

#define GET_CURRENT_PC_BP_SP \
    GET_CURRENT_PC_BP;       \
    uptr local_stack;        \
    uptr sp = (uptr)&local_stack

#endif // SANITIZER_STACKTRACE_H
