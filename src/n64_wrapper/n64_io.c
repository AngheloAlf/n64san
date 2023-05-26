#include "n64_wrapper/n64_io.h"

#if TARGET_GAME_OOT

GAME_INTERNAL_FUNCTION void *is_proutSyncPrintf(void *arg, const char *str, u32 count);
GAME_INTERNAL_FUNCTION s32 _Printf(PrintCallback pfn, void *arg, const char *fmt, va_list ap);

GAME_INTERNAL_FUNCTION s32 vsprintf(char *dst, const char *fmt, va_list args);

#define VPRINTF_FUNC(fmt, args) _Printf(is_proutSyncPrintf, NULL, fmt, args)
#define VSPRINTF_FUNC(dst, fmt, args) vsprintf(dst, fmt, args)

// OoT does the initialization in the game code
#define INITIALIZATION_FUNC()

#endif

#if TARGET_GAME_MM

GAME_INTERNAL_FUNCTION s32 _Printf(PrintCallback pfn, void *arg, const char *fmt, va_list ap);

GAME_INTERNAL_FUNCTION s32 vsprintf(char *dst, const char *fmt, va_list args);

GAME_INTERNAL_FUNCTION OSPiHandle* osCartRomInit(void);
GAME_INTERNAL_FUNCTION s32 osEPiWriteIo(OSPiHandle* handle, uintptr_t devAddr, u32 data);
GAME_INTERNAL_FUNCTION s32 osEPiReadIo(OSPiHandle* handle, uintptr_t devAddr, u32* data);

OSPiHandle* sISVHandle; // official name : is_Handle

typedef struct {
    /* 0x00 */ u32 magic; // IS64
    /* 0x04 */ u32 get;
    /* 0x08 */ u8 unk_08[0x14-0x08];
    /* 0x14 */ u32 put;
    /* 0x18 */ u8 unk_18[0x20-0x18];
    /* 0x20 */ u8 data[0x10000-0x20];
} ISVDbg;

#define gISVDbgPrnAdrs ((ISVDbg*)0xB3FF0000)
#define ASCII_TO_U32(a, b, c, d) ((u32)((a << 24) | (b << 16) | (c << 8) | (d << 0)))

void isPrintfInit(void) {
    sISVHandle = osCartRomInit();
    osEPiWriteIo(sISVHandle, (u32)&gISVDbgPrnAdrs->put, 0);
    osEPiWriteIo(sISVHandle, (u32)&gISVDbgPrnAdrs->get, 0);
    osEPiWriteIo(sISVHandle, (u32)&gISVDbgPrnAdrs->magic, ASCII_TO_U32('I', 'S', '6', '4'));
}

void* is_proutSyncPrintf(void* arg UNUSED, const char* str, u32 count) {
    u32 data;
    s32 pos;
    s32 start;
    s32 end;

    osEPiReadIo(sISVHandle, (u32)&gISVDbgPrnAdrs->magic, &data);
    if (data != ASCII_TO_U32('I', 'S', '6', '4')) {
        return (void*)1;
    }
    osEPiReadIo(sISVHandle, (u32)&gISVDbgPrnAdrs->get, &data);
    pos = data;
    osEPiReadIo(sISVHandle, (u32)&gISVDbgPrnAdrs->put, &data);
    start = data;
    end = start + count;
    if (end >= 0xFFE0) {
        end -= 0xFFE0;
        if (pos < end || start < pos) {
            return (void*)1;
        }
    } else {
        if (start < pos && pos < end) {
            return (void*)1;
        }
    }
    while (count) {
        u32 addr = (u32)&gISVDbgPrnAdrs->data + (start & 0xFFFFFFC);
        s32 shift = ((3 - (start & 3)) * 8);

        if (*str) {
            osEPiReadIo(sISVHandle, addr, &data);
            osEPiWriteIo(sISVHandle, addr, (*str << shift) | (data & ~(0xFF << shift)));

            start++;
            if (start >= 0xFFE0) {
                start -= 0xFFE0;
            }
        }
        count--;
        str++;
    }
    osEPiWriteIo(sISVHandle, (u32)&gISVDbgPrnAdrs->put, start);

    return (void*)1;
}

#define VPRINTF_FUNC(fmt, args) _Printf(is_proutSyncPrintf, NULL, fmt, args)
#define VSPRINTF_FUNC(dst, fmt, args) vsprintf(dst, fmt, args)

#define INITIALIZATION_FUNC() isPrintfInit()

#endif

static bool isInitialized = false;

#define INITIALIZE() \
    do { \
        if (!isInitialized) { \
            INITIALIZATION_FUNC(); \
            isInitialized = true; \
        } \
    } while (0)


int N64Wrapper_Vprintf(const char *fmt, va_list args) {
    INITIALIZE();

    return VPRINTF_FUNC(fmt, args);
}

int N64Wrapper_Printf(const char *fmt, ...) {
    int ret;

    va_list args;
    va_start(args, fmt);

    INITIALIZE();

    ret = N64Wrapper_Vprintf(fmt, args);

    va_end(args);

    return ret;
}

s32 N64Wrapper_Vsprintf(char *dst, const char *fmt, va_list args) {
    INITIALIZE();

    return VSPRINTF_FUNC(dst, fmt, args);
}

s32 N64Wrapper_Sprintf(char *dst, const char *fmt, ...) {
    s32 ret;
    va_list args;
    va_start(args, fmt);

    INITIALIZE();

    ret = N64Wrapper_Vsprintf(dst, fmt, args);

    va_end(args);

    return ret;
}
