#include "global_headers/libc.h"
#include "global_headers/ultra64.h"

#include "sanitizer_common/sanitizer_stacktrace.h"
#include "asan/asan_report.h"

#include "n64_wrapper/n64_wrapper.h"

// TODO: Improve. Maybe add some extra data at the beginning of each pointer to track stuff (?)

static ALWAYS_INLINE void *OnMalloc(u32 class_id, size_t size) {
    void *ptr;

    N64Wrapper_DebugPrintf("OnMalloc(%i, 0x%X)\n", class_id, size);

    if (!N64Wrapper_Heap_IsInitialized()) {
        N64Wrapper_DebugPrintf("OnMalloc(): Initializing heap\n");
        N64Wrapper_Heap_InitArena();
    }

    ptr = N64Wrapper_Heap_Malloc(size);
    N64Wrapper_DebugPrintf("OnMalloc(): ptr: %X\n", ptr);

    (void)class_id;

    N64Wrapper_DebugPrintf("\n");

    return ptr;
}

static ALWAYS_INLINE void *OnMallocAlways(u32 class_id, size_t size) {
    void *ptr;

    N64Wrapper_DebugPrintf("OnMallocAlways(%i, 0x%X)\n", class_id, size);

    if (!N64Wrapper_Heap_IsInitialized()) {
        N64Wrapper_DebugPrintf("OnMallocAlways(): Initializing heap\n");
        N64Wrapper_Heap_InitArena();
    }

    ptr = N64Wrapper_Heap_Malloc(size);
    N64Wrapper_DebugPrintf("OnMallocAlways(): ptr: %X\n", ptr);

    (void)class_id;

    N64Wrapper_DebugPrintf("\n");

    return ptr;
}

static ALWAYS_INLINE void OnFree(void *ptr, u32 class_id, size_t size) {
    N64Wrapper_DebugPrintf("OnFree(0x%X, %i, 0x%X)\n", ptr, class_id, size);

    if (!N64Wrapper_Heap_IsInitialized()) {
        N64Wrapper_DebugPrintf("OnFree(): Initializing heap\n");
        N64Wrapper_Heap_InitArena();
    }

    (void)class_id;
    (void)size;

    N64Wrapper_Heap_Free(ptr);

    N64Wrapper_DebugPrintf("\n");
}

#define DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(class_id)                                      \
    SANITIZER_INTERFACE_ATTRIBUTE void *__asan_stack_malloc_##class_id(size_t size) {         \
        return OnMalloc(class_id, size);                                                      \
    }                                                                                         \
    SANITIZER_INTERFACE_ATTRIBUTE void *__asan_stack_malloc_always_##class_id(size_t size) {  \
        return OnMallocAlways(class_id, size);                                                \
    }                                                                                         \
    SANITIZER_INTERFACE_ATTRIBUTE void __asan_stack_free_##class_id(void *ptr, size_t size) { \
        OnFree(ptr, class_id, size);                                                          \
    }

DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(0)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(1)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(2)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(3)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(4)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(5)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(6)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(7)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(8)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(9)
DEFINE_STACK_MALLOC_FREE_WITH_CLASS_ID(10)
