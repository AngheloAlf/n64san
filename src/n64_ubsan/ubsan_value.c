/**
 * Based on llvm-project
 * https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/ubsan/ubsan_value.cpp
 * 
 * Representation of data which is passed from the compiler-generated calls into
 * the ubsan runtime.
 */

#include "ubsan/ubsan_value.h"
#include "sanitizer_common/asm.h"

#include "n64_wrapper/n64_wrapper.h"

bool TypeDescriptor_isIntegerTy(const TypeDescriptor *typeDescr) {
    return typeDescr->TypeKind == TK_Integer;
}

bool TypeDescriptor_isSignedIntegerTy(const TypeDescriptor *typeDescr) {
    return TypeDescriptor_isIntegerTy(typeDescr) && (typeDescr->TypeInfo & 1);
}

bool TypeDescriptor_isUnsignedIntegerTy(const TypeDescriptor *typeDescr) {
    return TypeDescriptor_isIntegerTy(typeDescr) && !(typeDescr->TypeInfo & 1);
}

unsigned TypeDescriptor_getIntegerBitWidth(const TypeDescriptor *typeDescr) {
    CHECK(TypeDescriptor_isIntegerTy(typeDescr));
    return 1 << (typeDescr->TypeInfo >> 1);
}

bool TypeDescriptor_isFloatTy(const TypeDescriptor *typeDescr) {
    return typeDescr->TypeKind == TK_Float;
}

unsigned TypeDescriptor_getFloatBitWidth(const TypeDescriptor *typeDescr) {
    CHECK(TypeDescriptor_isFloatTy(typeDescr));
    return typeDescr->TypeInfo;
}

/// Is \c Val a (zero-extended) integer?
static inline bool Ubsan_Value_isInlineInt(const TypeDescriptor *Type) {
    CHECK(TypeDescriptor_isIntegerTy(Type));
    const unsigned InlineBits = sizeof(ValueHandle) * 8;
    const unsigned Bits = TypeDescriptor_getIntegerBitWidth(Type);
    return Bits <= InlineBits;
}

/// Is \c Val a (zero-extended) integer representation of a float?
static inline bool Ubsan_Value_isInlineFloat(const TypeDescriptor *Type) {
    CHECK(TypeDescriptor_isFloatTy(Type));
    const unsigned InlineBits = sizeof(ValueHandle) * 8;
    const unsigned Bits = TypeDescriptor_getFloatBitWidth(Type);
    return Bits <= InlineBits;
}

SIntMax Ubsan_Value_getSIntValue(const TypeDescriptor *Type, ValueHandle Val) {
    CHECK(TypeDescriptor_isSignedIntegerTy(Type));
    if (Ubsan_Value_isInlineInt(Type)) {
        // Val was zero-extended to ValueHandle. Sign-extend from original width
        // to SIntMax.
        const unsigned ExtraBits = sizeof(SIntMax) * 8 - TypeDescriptor_getIntegerBitWidth(Type);
        // GCC emits __ashldi3 and __ashrdi3, so we implement it ourselves
        // return (SIntMax)((UIntMax)(Val) << ExtraBits) >> ExtraBits;
        return U64_RightShift((SIntMax)U64_LeftShift((UIntMax)(Val), ExtraBits), ExtraBits);
    }

    if (TypeDescriptor_getIntegerBitWidth(Type) == 64) {
        return *(s64 *)(Val);
    }

    UNREACHABLE("unexpected bit width");
}

UIntMax Ubsan_Value_getUIntValue(const TypeDescriptor *Type, ValueHandle Val) {
    CHECK(TypeDescriptor_isUnsignedIntegerTy(Type));
    if (Ubsan_Value_isInlineInt(Type)) {
        return Val;
    }

    if (TypeDescriptor_getIntegerBitWidth(Type) == 64) {
        return *(u64 *)(Val);
    }

    UNREACHABLE("unexpected bit width");
}

UIntMax Ubsan_Value_getPositiveIntValue(const TypeDescriptor *Type, ValueHandle Val) {
    if (TypeDescriptor_isUnsignedIntegerTy(Type)) {
        return Ubsan_Value_getUIntValue(Type, Val);
    }
    SIntMax result = Ubsan_Value_getSIntValue(Type, Val);
    CHECK(result >= 0);
    return result;
}

bool Ubsan_Value_isMinusOne(const TypeDescriptor *Type, ValueHandle Val) {
    return TypeDescriptor_isSignedIntegerTy(Type) && Ubsan_Value_getSIntValue(Type, Val) == -1;
}

bool Ubsan_Value_isNegative(const TypeDescriptor *Type, ValueHandle Val) {
    return TypeDescriptor_isSignedIntegerTy(Type) && Ubsan_Value_getSIntValue(Type, Val) < 0;
}

FloatMax Ubsan_Value_getFloatValue(const TypeDescriptor *Type, ValueHandle Val) {
    CHECK(TypeDescriptor_isFloatTy(Type));

    if (Ubsan_Value_isInlineFloat(Type)) {
        switch (TypeDescriptor_getFloatBitWidth(Type)) {
            case 32: {
                f32 Value;
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                // For big endian the float value is in the last 4 bytes.
                // On some targets we may only have 4 bytes so we count backwards from
                // the end of Val to account for both the 32-bit and 64-bit cases.
                N64Wrapper_Memcpy(&Value, ((const char *)(&Val + 1)) - sizeof(f32), sizeof(f32));
#else
                N64Wrapper_Memcpy(&Value, &Val, sizeof(f32));
#endif
                return Value;
            }
            case 64: {
                f64 Value;
                N64Wrapper_Memcpy(&Value, &Val, sizeof(f64));
                return Value;
            }
        }
    } else {
        switch (TypeDescriptor_getFloatBitWidth(Type)) {
            case 64:
                return *(double *)(Val);
            case 80:
                return *(long double *)(Val);
            case 96:
                return *(long double *)(Val);
            case 128:
                return *(long double *)(Val);
        }
    }

    UNREACHABLE("unexpected floating point bit width");
}
