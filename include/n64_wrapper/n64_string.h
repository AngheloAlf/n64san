#ifndef N64_STRING_H
#define N64_STRING_H

#include "n64_utils.h"

/**
 * Copies `n` bytes pointed by `src` to the buffer pointed by `dst`.
 * Returns `dst`.
 */
void *N64Wrapper_Memcpy(void *dst, const void *src, size_t n);

/**
 * Sets `length` bytes of `dst` to zero.
 */
void N64Wrapper_Bzero(void *dst, size_t length);

#endif
