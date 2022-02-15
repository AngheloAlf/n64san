#ifndef ALLOCA_H
#define ALLOCA_H

#include "ultra64/types.h"

void* alloca(u32);
#define alloca  __builtin_alloca

#endif
