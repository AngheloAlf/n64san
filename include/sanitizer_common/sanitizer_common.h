#ifndef SANITIZER_COMMON_H
#define SANITIZER_COMMON_H

/**
 * Based on llvm-project
 * https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_common.h
 */

#include "global_headers/libc.h"
#include "sanitizer_internal_defs.h"

extern const char *SanitizerToolName; // Can be changed by the tool.

#endif
