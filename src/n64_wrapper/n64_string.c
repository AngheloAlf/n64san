#include "n64_wrapper/n64_string.h"

#if TARGET_GAME_OOT

GAME_INTERNAL_FUNCTION void* memcpy(void* dst, const void* src, size_t size);
GAME_INTERNAL_FUNCTION void bzero(void* begin, size_t length);


void *N64Wrapper_Memcpy(void *dst, const void *src, size_t n) {
    return memcpy(dst, src, n);
}

void N64Wrapper_Bzero(void *dst, size_t length) {
    bzero(dst, length);
}

#endif
