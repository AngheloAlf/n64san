#include "global_headers/libc.h"
#include "global_headers/ultra64.h"

#include "sanitizer_common/sanitizer_stacktrace.h"
#include "asan/asan_report.h"


#define ASAN_REPORT_ERROR(type, is_write, size)                     \
NOINLINE INTERFACE_ATTRIBUTE                             \
void __asan_report_ ## type ## size(uintptr_t addr) {                    \
  GET_CALLER_PC_BP_SP;                                              \
  ReportGenericError(pc, bp, sp, addr, is_write, size, 0, true);    \
}                                                                   \
NOINLINE INTERFACE_ATTRIBUTE                             \
void __asan_report_exp_ ## type ## size(uintptr_t addr, u32 exp) {       \
  GET_CALLER_PC_BP_SP;                                              \
  ReportGenericError(pc, bp, sp, addr, is_write, size, exp, true);  \
}                                                                   \
NOINLINE INTERFACE_ATTRIBUTE                             \
void __asan_report_ ## type ## size ## _noabort(uintptr_t addr) {        \
  GET_CALLER_PC_BP_SP;                                              \
  ReportGenericError(pc, bp, sp, addr, is_write, size, 0, false);   \
}                                                                   \

ASAN_REPORT_ERROR(load, false, 1)
ASAN_REPORT_ERROR(load, false, 2)
ASAN_REPORT_ERROR(load, false, 4)
ASAN_REPORT_ERROR(load, false, 8)
ASAN_REPORT_ERROR(load, false, 16)
ASAN_REPORT_ERROR(store, true, 1)
ASAN_REPORT_ERROR(store, true, 2)
ASAN_REPORT_ERROR(store, true, 4)
ASAN_REPORT_ERROR(store, true, 8)
ASAN_REPORT_ERROR(store, true, 16)
