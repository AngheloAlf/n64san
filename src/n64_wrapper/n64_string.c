#include "n64_wrapper/n64_string.h"

GAME_INTERNAL_FUNCTION void* memcpy(void* dst, const void* src, size_t size);

void *N64Wrapper_Memcpy(void *dst, const void *src, size_t n) {
    return memcpy(dst, src, n);
}
