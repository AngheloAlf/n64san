#ifndef UBSAN_DIAG_H
#define UBSAN_DIAG_H
/**
 * Diagnostics emission for Clang's undefined behavior sanitizer.
 */


#include "ubsan_value.h"
#include "sanitizer_common/sanitizer_stacktrace.h"
//#include "sanitizer_common/sanitizer_symbolizer.h"



/// A location of some data within the program's address space.
typedef uintptr_t MemoryLocation;



typedef enum Ubsan_LocationKind {
    LK_Null, LK_Source, LK_Memory, LK_Symbolized 
} Ubsan_LocationKind;

typedef struct Ubsan_Location {
    Ubsan_LocationKind Kind;
    union {
        SourceLocation SourceLoc;
        MemoryLocation MemoryLoc;
        //const SymbolizedStack *SymbolizedLoc;  // Not owned.
    };
} Ubsan_Location;



/// A diagnostic severity level.
typedef enum DiagLevel {
  DL_Error, ///< An error.
  DL_Note   ///< A note, attached to a prior diagnostic.
} DiagLevel;



typedef enum ErrorType {
#define UBSAN_CHECK(Name, SummaryKind, FSanitizeFlagName) ErrorType_ ## Name,
#include "ubsan_checks.inc"
#undef UBSAN_CHECK
} ErrorType;


void Ubsan_Diag(Ubsan_Location *loc, DiagLevel Level, ErrorType ET, const char *fmt, ...);


typedef struct ReportOptions {
  bool FromUnrecoverableHandler;
  uptr pc;
  uptr bp;
} ReportOptions;


#define GET_REPORT_OPTIONS(unrecoverable_handler) \
    GET_CALLER_PC_BP; \
    ReportOptions Opts = {unrecoverable_handler, pc, bp}


#endif
