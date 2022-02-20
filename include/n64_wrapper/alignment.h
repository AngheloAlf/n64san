#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "n64_utils.h"

#define ALIGN8(val) (((val) + 7) & ~7)
#define ALIGN16(val) (((val) + 0xF) & ~0xF)
#define ALIGN32(val) (((val) + 0x1F) & ~0x1F)
#define ALIGN64(val) (((val) + 0x3F) & ~0x3F)
#define ALIGN256(val) (((val) + 0xFF) & ~0xFF)

#define ALIGNED8 __attribute__((aligned(8)))

#define STACK(stack, size) u64 stack[ALIGN8(size) / sizeof(u64)]

#define STACK_TOP(stack) ((u8 *)(stack) + sizeof(stack))

#endif
