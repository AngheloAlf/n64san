#include "asan/asan_report.h"
#include "n64_wrapper/n64_wrapper.h"


void ReportGenericError(uintptr_t pc, uintptr_t bp, uintptr_t sp, uintptr_t addr, bool is_write,
                        uintptr_t access_size, u32 exp, bool fatal) {
    // TODO: improve

    N64Wrapper_Printf("ReportGenericError()\n");

    N64Wrapper_Printf("\t pc: %X\n", pc);
    N64Wrapper_Printf("\t bp: %X\n", bp);
    N64Wrapper_Printf("\t sp: %X\n", sp);
    N64Wrapper_Printf("\t addr: %X\n", addr);
    N64Wrapper_Printf("\t is_write,: %s\n", is_write ? "true" : "false");
    N64Wrapper_Printf("\t access_size: %X\n", access_size);
    N64Wrapper_Printf("\t exp: %X\n", exp);
    N64Wrapper_Printf("\t fatal: %s\n", fatal ? "true" : "false");

    N64Wrapper_Printf("\n");
}

