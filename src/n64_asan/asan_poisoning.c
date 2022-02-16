#include "global_headers/libc.h"
#include "global_headers/ultra64.h"

#include "asan/asan_interface_internal.h"

uintptr_t __asan_region_is_poisoned(uintptr_t beg, size_t size) {
    (void)beg;
    (void)size;

    return 0;
}
