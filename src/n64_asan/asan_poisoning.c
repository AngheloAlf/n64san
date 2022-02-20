/**
 * Based on llvm-project
 * https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/asan/asan_poisoning.cpp
 * 
 * Shadow memory poisoning by ASan RTL and by user application.
 */

#include "global_headers/libc.h"
#include "global_headers/ultra64.h"

#include "asan/asan_interface_internal.h"

uintptr_t __asan_region_is_poisoned(uintptr_t beg, size_t size) {
    (void)beg;
    (void)size;

    return 0;
}
