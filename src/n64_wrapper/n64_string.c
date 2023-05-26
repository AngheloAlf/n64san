#include "n64_wrapper/n64_string.h"

#if TARGET_GAME_OOT || TARGET_GAME_MM

GAME_INTERNAL_FUNCTION void *memcpy(void *dst, const void *src, size_t size);
GAME_INTERNAL_FUNCTION void bzero(void *begin, size_t length);

#define MEMCPY_FUNC(dst, src, size) memcpy(dst, src, size)
#define BZERO_FUNC(begin, length) bzero(begin, length)

#endif

void *N64Wrapper_Memcpy(void *dst, const void *src, size_t n) {
    return MEMCPY_FUNC(dst, src, n);
}

void N64Wrapper_Bzero(void *dst, size_t length) {
    BZERO_FUNC(dst, length);
}
