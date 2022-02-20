#include "n64_wrapper/n64_heap_alloc.h"
#include "n64_wrapper/alignment.h"
#include "n64_wrapper/n64_string.h"

// This module is not required by every sanitizer, so there's no point in building it by default.
// Also, if it isn't going to be used, then this heap would be dead space in the resulting binary.
#if ENABLE_N64WRAPPER_HEAP_ALLOC

#if TARGET_GAME_OOT

// Arena with arbitrary size that should be enough for most cases
STACK(gN64Wrapper_Heap_Arena, N64WRAPPER_HEAP_ARENASIZE);
// The heap
STACK(gN64Wrapper_Heap_Heap, N64WRAPPER_HEAP_HEAPSIZE);

#ifdef TARGET_GAME_OOT
#define __osGetSizes ArenaImpl_GetSizes
#endif

#define Arena void

GAME_INTERNAL_FUNCTION void __osMallocInit(Arena *arena, void *heap, size_t size);
GAME_INTERNAL_FUNCTION void __osMallocCleanup(Arena *arena);
GAME_INTERNAL_FUNCTION u8 __osMallocIsInitalized(Arena *arena);
GAME_INTERNAL_FUNCTION void *__osMalloc(Arena *arena, size_t size);
GAME_INTERNAL_FUNCTION void *__osMallocR(Arena *arena, size_t size);
GAME_INTERNAL_FUNCTION void __osFree(Arena *arena, void *ptr);
GAME_INTERNAL_FUNCTION void *__osRealloc(Arena *arena, void *ptr, size_t newSize);
GAME_INTERNAL_FUNCTION void __osGetSizes(Arena *arena, size_t *maxFreeBlock, size_t *bytesFree, size_t *bytesAllocated);
GAME_INTERNAL_FUNCTION s32 __osCheckArena(Arena *arena);

void *N64Wrapper_Heap_Malloc(size_t size) {
    return __osMalloc(&gN64Wrapper_Heap_Arena, size);
}

void *N64Wrapper_Heap_Realloc(void *oldPtr, size_t newSize) {
    return __osRealloc(&gN64Wrapper_Heap_Arena, oldPtr, newSize);
}

void N64Wrapper_Heap_Free(void *ptr) {
    __osFree(&gN64Wrapper_Heap_Arena, ptr);
}

void *N64Wrapper_Heap_Calloc(u32 elements, size_t size) {
    void *ptr;
    size_t totalSize = elements * size;

    ptr = __osMalloc(&gN64Wrapper_Heap_Arena, totalSize);
    if (ptr != NULL) {
        N64Wrapper_Bzero(ptr, totalSize);
    }
    return ptr;
}

void N64Wrapper_Heap_GetSizes(size_t *maxFreeBlock, size_t *bytesFree, size_t *bytesAllocated) {
    __osGetSizes(&gN64Wrapper_Heap_Arena, maxFreeBlock, bytesFree, bytesAllocated);
}

u32 N64Wrapper_Heap_CheckArena(void) {
    return __osCheckArena(&gN64Wrapper_Heap_Arena);
}

void N64Wrapper_Heap_InitArena() {
    __osMallocInit(&gN64Wrapper_Heap_Arena, gN64Wrapper_Heap_Heap, sizeof(gN64Wrapper_Heap_Heap));
}

void N64Wrapper_Heap_Cleanup(void) {
    __osMallocCleanup(&gN64Wrapper_Heap_Arena);
}

bool N64Wrapper_Heap_IsInitialized(void) {
    return __osMallocIsInitalized(&gN64Wrapper_Heap_Arena);
}

#endif

#endif /* ENABLE_N64WRAPPER_HEAP_ALLOC */
