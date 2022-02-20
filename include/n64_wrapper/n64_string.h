#ifndef N64_STRING_H
#define N64_STRING_H

#include "n64_utils.h"

void *N64Wrapper_Memcpy(void *dst, const void *src, size_t n);

void N64Wrapper_Bzero(void *dst, size_t length);

#endif
