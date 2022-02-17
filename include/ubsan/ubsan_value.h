#ifndef UBSAN_VALUE_H
#define UBSAN_VALUE_H
/**
 * Representation of data which is passed from the compiler-generated calls into
 * the ubsan runtime.
 */

#include "sanitizer_common/sanitizer_common.h"
#include "sanitizer_common/sanitizer_internal_defs.h"
#include "global_headers/unk.h"


typedef s64 SIntMax;
typedef u64 UIntMax;

typedef f64 FloatMax;

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

typedef enum TypeDescriptor_Kind {
    /// An integer type. Lowest bit is 1 for a signed value, 0 for an unsigned
    /// value. Remaining bits are log_2(bit width). The value representation is
    /// the integer itself if it fits into a ValueHandle, and a pointer to the
    /// integer otherwise.
    TK_Integer = 0x0000,
    /// A floating-point type. Low 16 bits are bit width. The value
    /// representation is that of bitcasting the floating-point value to an
    /// integer type.
    TK_Float = 0x0001,
    /// Any other type. The value representation is unspecified.
    TK_Unknown = 0xffff
} TypeDescriptor_Kind;

bool TypeDescriptor_isIntegerTy(const TypeDescriptor* typeDescr);
bool TypeDescriptor_isSignedIntegerTy(const TypeDescriptor* typeDescr);
bool TypeDescriptor_isUnsignedIntegerTy(const TypeDescriptor* typeDescr);
unsigned TypeDescriptor_getIntegerBitWidth(const TypeDescriptor* typeDescr);
bool TypeDescriptor_isFloatTy(const TypeDescriptor* typeDescr);
unsigned TypeDescriptor_getFloatBitWidth(const TypeDescriptor* typeDescr);


typedef uintptr_t ValueHandle;

/// \brief Get this value as a signed integer.
SIntMax Ubsan_Value_getSIntValue(const TypeDescriptor *Type, ValueHandle Val);

/// \brief Get this value as an unsigned integer.
UIntMax Ubsan_Value_getUIntValue(const TypeDescriptor *Type, ValueHandle Val);

/// \brief Decode this value, which must be a positive or unsigned integer.
UIntMax Ubsan_Value_getPositiveIntValue(const TypeDescriptor *Type, ValueHandle Val);

/// Is this an integer with value -1?
bool Ubsan_Value_isMinusOne(const TypeDescriptor *Type, ValueHandle Val);

/// Is this a negative integer?
bool Ubsan_Value_isNegative(const TypeDescriptor *Type, ValueHandle Val);

/// \brief Get this value as a floating-point quantity.
FloatMax Ubsan_Value_getFloatValue(const TypeDescriptor *Type, ValueHandle Val);


#endif
