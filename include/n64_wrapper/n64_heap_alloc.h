#ifndef N64_HEAP_ALLOC_H
#define N64_HEAP_ALLOC_H

#include "n64_utils.h"

void* N64Wrapper_Heap_Malloc(size_t size);
void* N64Wrapper_Heap_Realloc(void* oldPtr, size_t newSize);
void N64Wrapper_Heap_Free(void* ptr);
void* N64Wrapper_Heap_Calloc(u32 elements, size_t size);
// void N64Wrapper_Heap_GetSizes(size_t* maxFreeBlock, size_t* bytesFree, size_t* bytesAllocated);
// u32 N64Wrapper_Heap_CheckArena(void);
void N64Wrapper_Heap_InitArena();
// void N64Wrapper_Heap_Cleanup(void);
bool N64Wrapper_Heap_IsInitialized(void);

#endif
