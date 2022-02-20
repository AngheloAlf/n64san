#ifndef UBSAN_HANDLERS_H
#define UBSAN_HANDLERS_H

/**
 * Based on llvm-project
 * https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/ubsan/ubsan_handlers.h
 * 
 * Entry points to the runtime library for Clang's undefined behavior sanitizer.
 */

#include "ubsan_value.h"

typedef struct TypeMismatchData {
    SourceLocation Loc;
    const TypeDescriptor *Type;
    unsigned char LogAlignment;
    unsigned char TypeCheckKind;
} TypeMismatchData;

#if 0
$Lubsan_type1:
	.half	-1 // TypeKind
	.half	0 // TypeInfo
	.ascii	"'struct GlobalContext'\000" // TypeName
	.rdata
	.align	2
$LC0:
	.ascii	"src/code/z_game_over.c\000"
	.data
	.align	2
	.type	$Lubsan_data4, @object
	.size	$Lubsan_data4, 20



$Lubsan_data4:
	.word	$LC0 // Filename
	.word	4    // Line
	.word	34   // Column
	.word	$Lubsan_type1 //
	.byte	3 // LogAlignment
	.byte	3 // TypeCheckKind

#endif

#define UNRECOVERABLE(checkname, ...) \
    SANITIZER_INTERFACE_ATTRIBUTE NORETURN void __ubsan_handle_##checkname(__VA_ARGS__);

#define RECOVERABLE(checkname, ...)               \
    SANITIZER_INTERFACE_ATTRIBUTE                 \
    void __ubsan_handle_##checkname(__VA_ARGS__); \
    SANITIZER_INTERFACE_ATTRIBUTE NORETURN void __ubsan_handle_##checkname##_abort(__VA_ARGS__);

/// \brief Handle a runtime type check failure, caused by either a misaligned
/// pointer, a null pointer, or a pointer to insufficient storage for the
/// type.
RECOVERABLE(type_mismatch_v1, TypeMismatchData *Data, ValueHandle Pointer)

typedef struct AlignmentAssumptionData {
    SourceLocation Loc;
    SourceLocation AssumptionLoc;
    const TypeDescriptor *Type;
} AlignmentAssumptionData;

/// \brief Handle a runtime alignment assumption check failure,
/// caused by a misaligned pointer.
RECOVERABLE(alignment_assumption, AlignmentAssumptionData *Data, ValueHandle Pointer, ValueHandle Alignment,
            ValueHandle Offset)

typedef struct OverflowData {
    SourceLocation Loc;
    const TypeDescriptor *Type;
} OverflowData;

/// \brief Handle an integer addition overflow.
RECOVERABLE(add_overflow, OverflowData *Data, ValueHandle LHS, ValueHandle RHS)

/// \brief Handle an integer subtraction overflow.
RECOVERABLE(sub_overflow, OverflowData *Data, ValueHandle LHS, ValueHandle RHS)

/// \brief Handle an integer multiplication overflow.
RECOVERABLE(mul_overflow, OverflowData *Data, ValueHandle LHS, ValueHandle RHS)

/// \brief Handle a signed integer overflow for a unary negate operator.
RECOVERABLE(negate_overflow, OverflowData *Data, ValueHandle OldVal)

/// \brief Handle an INT_MIN/-1 overflow or division by zero.
RECOVERABLE(divrem_overflow, OverflowData *Data, ValueHandle LHS, ValueHandle RHS)

typedef struct ShiftOutOfBoundsData {
    SourceLocation Loc;
    const TypeDescriptor *LHSType;
    const TypeDescriptor *RHSType;
} ShiftOutOfBoundsData;

/// \brief Handle a shift where the RHS is out of bounds or a left shift where
/// the LHS is negative or overflows.
RECOVERABLE(shift_out_of_bounds, ShiftOutOfBoundsData *Data, ValueHandle LHS, ValueHandle RHS)

typedef struct OutOfBoundsData {
    SourceLocation Loc;
    const TypeDescriptor *ArrayType;
    const TypeDescriptor *IndexType;
} OutOfBoundsData;

/// \brief Handle an array index out of bounds error.
RECOVERABLE(out_of_bounds, OutOfBoundsData *Data, ValueHandle Index)

typedef struct UnreachableData {
    SourceLocation Loc;
} UnreachableData;

/// \brief Handle a __builtin_unreachable which is reached.
UNRECOVERABLE(builtin_unreachable, UnreachableData *Data)
/// \brief Handle reaching the end of a value-returning function.
UNRECOVERABLE(missing_return, UnreachableData *Data)

typedef struct VLABoundData {
    SourceLocation Loc;
    const TypeDescriptor *Type;
} VLABoundData;

/// \brief Handle a VLA with a non-positive bound.
RECOVERABLE(vla_bound_not_positive, VLABoundData *Data, ValueHandle Bound)

// Keeping this around for binary compatibility with (sanitized) programs
// compiled with older compilers.
typedef struct FloatCastOverflowData {
    const TypeDescriptor *FromType;
    const TypeDescriptor *ToType;
} FloatCastOverflowData;

typedef struct FloatCastOverflowDataV2 {
    SourceLocation Loc;
    const TypeDescriptor *FromType;
    const TypeDescriptor *ToType;
} FloatCastOverflowDataV2;

/// Handle overflow in a conversion to or from a floating-point type.
/// void *Data is one of FloatCastOverflowData* or FloatCastOverflowDataV2*
RECOVERABLE(float_cast_overflow, void *Data, ValueHandle From)

typedef struct InvalidValueData {
    SourceLocation Loc;
    const TypeDescriptor *Type;
} InvalidValueData;

/// \brief Handle a load of an invalid value for the type.
RECOVERABLE(load_invalid_value, InvalidValueData *Data, ValueHandle Val)

/// Known implicit conversion check kinds.
/// Keep in sync with the enum of the same name in CGExprScalar.cpp
typedef enum ImplicitConversionCheckKind {
    ICCK_IntegerTruncation = 0, // Legacy, was only used by clang 7.
    ICCK_UnsignedIntegerTruncation = 1,
    ICCK_SignedIntegerTruncation = 2,
    ICCK_IntegerSignChange = 3,
    ICCK_SignedIntegerTruncationOrSignChange = 4,
} ImplicitConversionCheckKind;

typedef struct ImplicitConversionData {
    SourceLocation Loc;
    const TypeDescriptor *FromType;
    const TypeDescriptor *ToType;
    /* ImplicitConversionCheckKind */ unsigned char Kind;
} ImplicitConversionData;

/// \brief Implict conversion that changed the value.
RECOVERABLE(implicit_conversion, ImplicitConversionData *Data, ValueHandle Src, ValueHandle Dst)

/// Known builtin check kinds.
/// Keep in sync with the enum of the same name in CodeGenFunction.h
typedef enum BuiltinCheckKind {
    BCK_CTZPassedZero,
    BCK_CLZPassedZero,
} BuiltinCheckKind;

typedef struct InvalidBuiltinData {
    SourceLocation Loc;
    unsigned char Kind;
} InvalidBuiltinData;

/// Handle a builtin called in an invalid way.
RECOVERABLE(invalid_builtin, InvalidBuiltinData *Data)

typedef struct InvalidObjCCast {
    SourceLocation Loc;
    const TypeDescriptor *ExpectedType;
} InvalidObjCCast;

/// Handle an invalid ObjC cast.
RECOVERABLE(invalid_objc_cast, InvalidObjCCast *Data, ValueHandle Pointer)

typedef struct NonNullReturnData {
    SourceLocation AttrLoc;
} NonNullReturnData;

/// \brief Handle returning null from function with the returns_nonnull
/// attribute, or a return type annotated with _Nonnull.
RECOVERABLE(nonnull_return_v1, NonNullReturnData *Data, SourceLocation *Loc)
RECOVERABLE(nullability_return_v1, NonNullReturnData *Data, SourceLocation *Loc)

typedef struct NonNullArgData {
    SourceLocation Loc;
    SourceLocation AttrLoc;
    int ArgIndex;
} NonNullArgData;

/// \brief Handle passing null pointer to a function parameter with the nonnull
/// attribute, or a _Nonnull type annotation.
RECOVERABLE(nonnull_arg, NonNullArgData *Data)
RECOVERABLE(nullability_arg, NonNullArgData *Data)

typedef struct PointerOverflowData {
    SourceLocation Loc;
} PointerOverflowData;

RECOVERABLE(pointer_overflow, PointerOverflowData *Data, ValueHandle Base, ValueHandle Result)

/// \brief Known CFI check kinds.
/// Keep in sync with the enum of the same name in CodeGenFunction.h
typedef enum CFITypeCheckKind {
    CFITCK_VCall,
    CFITCK_NVCall,
    CFITCK_DerivedCast,
    CFITCK_UnrelatedCast,
    CFITCK_ICall,
    CFITCK_NVMFCall,
    CFITCK_VMFCall,
} CFITypeCheckKind;

typedef struct CFICheckFailData {
    CFITypeCheckKind CheckKind;
    SourceLocation Loc;
    const TypeDescriptor *Type;
} CFICheckFailData;

/// \brief Handle control flow integrity failures.
RECOVERABLE(cfi_check_fail, CFICheckFailData *Data, ValueHandle Function, uptr VtableIsValid)

#if 0
typedef struct ReportOptions ReportOptions;

SANITIZER_INTERFACE_ATTRIBUTE void __ubsan_handle_cfi_bad_type(
    CFICheckFailData *Data, ValueHandle Vtable, bool ValidVtable,
    ReportOptions Opts);
#endif

#endif // UBSAN_HANDLERS_H
