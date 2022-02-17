/**
 * Diagnostic reporting for the UBSan runtime.
 */

#include "ubsan/ubsan_diag.h"
#include "sanitizer_common/sanitizer_stacktrace.h"

#include "global_headers/vt.h"

#include "n64_wrapper/n64_wrapper.h"



void Ubsan_PrintSourceLocation(const char *file, int line, int column, bool vs_style) {
    N64Wrapper_Printf("%s", file);

    if (line <= 0) {
        return;
    }

    if (vs_style) {
        N64Wrapper_Printf("(%d", line);
        if (column > 0) {
            N64Wrapper_Printf(",%d", column);
        }
        N64Wrapper_Printf(")");
    } else {
        N64Wrapper_Printf(":%d", line);
        if (column > 0) {
            N64Wrapper_Printf(":%d", column);
        }
    }
}



void Ubsan_Location_Print(Ubsan_Location *loc) {
    switch (loc->Kind) {
    case LK_Source: {
        SourceLocation* SLoc = &loc->SourceLoc;
        if (SLoc->Filename == NULL) {
            N64Wrapper_Printf("<unknown>");
        } else {
            Ubsan_PrintSourceLocation(SLoc->Filename, SLoc->Line, SLoc->Column, false);
        }
        break;
    }

    case LK_Memory:
        N64Wrapper_Printf("%p", loc->MemoryLoc);
        break;

// TODO
#if 0
    case LK_Symbolized: {
        const AddressInfo &Info = Loc.getSymbolizedStack()->info;
        if (Info.file)
        RenderSourceLocation(Buffer, Info.file, Info.line, Info.column,
                            common_flags()->symbolize_vs_style,
                            common_flags()->strip_path_prefix);
        else if (Info.module)
        RenderModuleLocation(Buffer, Info.module, Info.module_offset,
                            Info.module_arch, common_flags()->strip_path_prefix);
        else
        Buffer->append("%p", reinterpret_cast<void *>(Info.address));
        break;
    }
#endif

    case LK_Null:
        N64Wrapper_Printf("<unknown>");
        break;

    default:
        UNREACHABLE("Ubsan_Location_Print(): Something went wrong");
    }
}



void Ubsan_DiagImpl(Ubsan_Location *loc, DiagLevel level, ErrorType et, const char* fmt, va_list args) {
#if 0
    Decorator Decor;
    InternalScopedString Buffer;
#endif

    // TODO: print et
    (void)et;


#if 0
    Buffer.append("%s", Decor.Bold());
    RenderLocation(&Buffer, loc);
    Buffer.append(":");
#endif
    N64Wrapper_Printf(VT_BOLD());
    Ubsan_Location_Print(loc);
    N64Wrapper_Printf(":");

    switch (level) {
    case DL_Error:
        // TODO: use preprocessor's string concat
        N64Wrapper_Printf("%s runtime error: %s%s", VT_BOLD_FGCOL(RED), VT_RST, VT_BOLD());
        #if 0
        Buffer.append("%s runtime error: %s%s", Decor.Warning(), Decor.Default(),
                    Decor.Bold());
        #endif
        break;

    case DL_Note:
        N64Wrapper_Printf("%s note: %s", VT_BOLD_FGCOL(BLACK), VT_RST);
        #if 0
        Buffer.append("%s note: %s", Decor.Note(), Decor.Default());
        #endif
        break;
    }

#if 0
    RenderText(&Buffer, fmt, Args);

    Buffer.append("%s\n", Decor.Default());
    Printf("%s", Buffer.data());
#else
    N64Wrapper_Vprintf(fmt, args);
    N64Wrapper_Printf("%s\n", VT_RST);
#endif

#if 0
    if (Loc.isMemoryLocation())
        PrintMemorySnippet(Decor, Loc.getMemoryLocation(), Ranges, NumRanges, Args);
#endif
}

void Ubsan_Diag(Ubsan_Location *loc, DiagLevel level, ErrorType et, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    Ubsan_DiagImpl(loc, level, et, fmt, args);

    va_end(args);
}
