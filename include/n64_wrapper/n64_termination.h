#ifndef N64_TERMINATION_H
#define N64_TERMINATION_H

#include "n64_utils.h"

void NORETURN N64Wrapper_Assert(const char* exp, const char* file, s32 line);


#ifdef NDEBUG
#define ASSERT(cond, msg, file, line) ((void)0)
#else
#define ASSERT(cond, msg, file, line) ((cond) ? ((void)0) : N64Wrapper_Assert(#cond, __FILE__, __LINE__))
#endif

#endif
