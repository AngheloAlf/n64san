#ifndef ASAN_REPORT_H
#define ASAN_REPORT_H

/**
 * Based on llvm-project
 * https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/asan/asan_report.h
 */

#include "sanitizer_common/sanitizer_internal_defs.h"

// Different kinds of error reports.
void ReportGenericError(uintptr_t pc, uintptr_t bp, uintptr_t sp, uintptr_t addr, bool is_write, uintptr_t access_size,
                        u32 exp, bool fatal);

#endif
