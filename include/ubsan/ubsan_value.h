#ifndef UBSAN_VALUE_H
#define UBSAN_VALUE_H
/**
 * Representation of data which is passed from the compiler-generated calls into
 * the ubsan runtime.
 */

#include "sanitizer_common/sanitizer_common.h"
#include "sanitizer_common/sanitizer_internal_defs.h"
#include "global_headers/unk.h"


typedef struct SourceLocation {
    const char *Filename;
    u32 Line;
    u32 Column;
} SourceLocation;


typedef struct TypeDescriptor {
    u16 TypeKind;
    u16 TypeInfo;
    char TypeName[UNK_SIZE]; // A string. The size is arbitrary
} TypeDescriptor;


typedef uintptr_t ValueHandle;


#endif
