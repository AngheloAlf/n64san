/**
 * Based on llvm-project
 * https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/asan/asan_rtl.cpp
 */

#include "global_headers/libc.h"
#include "global_headers/ultra64.h"

#include "sanitizer_common/sanitizer_stacktrace.h"
#include "asan/asan_report.h"
#include "asan/asan_interface_internal.h"

uptr __asan_shadow_memory_dynamic_address;               // Global interface symbol.
int __asan_option_detect_stack_use_after_return = false; // Global interface symbol.
uptr *__asan_test_only_reported_buggy_pointer;           // Used only for testing asan.

#define ASAN_REPORT_ERROR(type, is_write, size)                                                 \
    NOINLINE INTERFACE_ATTRIBUTE void __asan_report_##type##size(uintptr_t addr) {              \
        GET_CALLER_PC_BP_SP;                                                                    \
        ReportGenericError(pc, bp, sp, addr, is_write, size, 0, true);                          \
    }                                                                                           \
    NOINLINE INTERFACE_ATTRIBUTE void __asan_report_exp_##type##size(uintptr_t addr, u32 exp) { \
        GET_CALLER_PC_BP_SP;                                                                    \
        ReportGenericError(pc, bp, sp, addr, is_write, size, exp, true);                        \
    }                                                                                           \
    NOINLINE INTERFACE_ATTRIBUTE void __asan_report_##type##size##_noabort(uintptr_t addr) {    \
        GET_CALLER_PC_BP_SP;                                                                    \
        ReportGenericError(pc, bp, sp, addr, is_write, size, 0, false);                         \
    }

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

// Crappy copy-paste of the above block
#define ASAN_ERROR(type, is_write, size)                                                 \
    NOINLINE INTERFACE_ATTRIBUTE void __asan_##type##size(uintptr_t addr) {              \
        GET_CALLER_PC_BP_SP;                                                             \
        ReportGenericError(pc, bp, sp, addr, is_write, size, 0, true);                   \
    }                                                                                    \
    NOINLINE INTERFACE_ATTRIBUTE void __asan_exp_##type##size(uintptr_t addr, u32 exp) { \
        GET_CALLER_PC_BP_SP;                                                             \
        ReportGenericError(pc, bp, sp, addr, is_write, size, exp, true);                 \
    }                                                                                    \
    NOINLINE INTERFACE_ATTRIBUTE void __asan_##type##size##_noabort(uintptr_t addr) {    \
        GET_CALLER_PC_BP_SP;                                                             \
        ReportGenericError(pc, bp, sp, addr, is_write, size, 0, false);                  \
    }

ASAN_ERROR(load, false, 1)
ASAN_ERROR(load, false, 2)
ASAN_ERROR(load, false, 4)
ASAN_ERROR(load, false, 8)
ASAN_ERROR(load, false, 16)
ASAN_ERROR(store, true, 1)
ASAN_ERROR(store, true, 2)
ASAN_ERROR(store, true, 4)
ASAN_ERROR(store, true, 8)
ASAN_ERROR(store, true, 16)

NOINLINE INTERFACE_ATTRIBUTE void __asan_loadN(uptr addr, uptr size) {
    if (__asan_region_is_poisoned(addr, size)) {
        GET_CALLER_PC_BP_SP;
        ReportGenericError(pc, bp, sp, addr, false, size, 0, true);
    }
}

NOINLINE INTERFACE_ATTRIBUTE void __asan_exp_loadN(uptr addr, uptr size, u32 exp) {
    if (__asan_region_is_poisoned(addr, size)) {
        GET_CALLER_PC_BP_SP;
        ReportGenericError(pc, bp, sp, addr, false, size, exp, true);
    }
}

NOINLINE INTERFACE_ATTRIBUTE void __asan_loadN_noabort(uptr addr, uptr size) {
    if (__asan_region_is_poisoned(addr, size)) {
        GET_CALLER_PC_BP_SP;
        ReportGenericError(pc, bp, sp, addr, false, size, 0, false);
    }
}

NOINLINE INTERFACE_ATTRIBUTE void __asan_storeN(uptr addr, uptr size) {
    if (__asan_region_is_poisoned(addr, size)) {
        GET_CALLER_PC_BP_SP;
        ReportGenericError(pc, bp, sp, addr, true, size, 0, true);
    }
}

NOINLINE INTERFACE_ATTRIBUTE void __asan_exp_storeN(uptr addr, uptr size, u32 exp) {
    if (__asan_region_is_poisoned(addr, size)) {
        GET_CALLER_PC_BP_SP;
        ReportGenericError(pc, bp, sp, addr, true, size, exp, true);
    }
}

NOINLINE INTERFACE_ATTRIBUTE void __asan_storeN_noabort(uptr addr, uptr size) {
    if (__asan_region_is_poisoned(addr, size)) {
        GET_CALLER_PC_BP_SP;
        ReportGenericError(pc, bp, sp, addr, true, size, 0, false);
    }
}
