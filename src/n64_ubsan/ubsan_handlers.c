/**
 * Error logging entry points for the UBSan runtime.
 */

//#include "ubsan_platform.h"
#include "ubsan/ubsan_handlers.h"
#include "ubsan/ubsan_diag.h"
//#include "ubsan_flags.h"
//#include "ubsan_monitor.h"
#include "ubsan/ubsan_value.h"

#include "sanitizer_common/sanitizer_common.h"



/// Situations in which we might emit a check for the suitability of a
/// pointer or glvalue. Needs to be kept in sync with CodeGenFunction.h in
/// clang.
enum TypeCheckKind {
  /// Checking the operand of a load. Must be suitably sized and aligned.
  TCK_Load,
  /// Checking the destination of a store. Must be suitably sized and aligned.
  TCK_Store,
  /// Checking the bound value in a reference binding. Must be suitably sized
  /// and aligned, but is not required to refer to an object (until the
  /// reference is used), per core issue 453.
  TCK_ReferenceBinding,
  /// Checking the object expression in a non-static data member access. Must
  /// be an object within its lifetime.
  TCK_MemberAccess,
  /// Checking the 'this' pointer for a call to a non-static member function.
  /// Must be an object within its lifetime.
  TCK_MemberCall,
  /// Checking the 'this' pointer for a constructor call.
  TCK_ConstructorCall,
  /// Checking the operand of a static_cast to a derived pointer type. Must be
  /// null or an object within its lifetime.
  TCK_DowncastPointer,
  /// Checking the operand of a static_cast to a derived reference type. Must
  /// be an object within its lifetime.
  TCK_DowncastReference,
  /// Checking the operand of a cast to a base object. Must be suitably sized
  /// and aligned.
  TCK_Upcast,
  /// Checking the operand of a cast to a virtual base object. Must be an
  /// object within its lifetime.
  TCK_UpcastToVirtualBase,
  /// Checking the value assigned to a _Nonnull pointer. Must not be null.
  TCK_NonnullAssign,
  /// Checking the operand of a dynamic_cast or a typeid expression.  Must be
  /// null or an object within its lifetime.
  TCK_DynamicOperation
};

const char *TypeCheckKinds[] = {
    "load of", "store to", "reference binding to", "member access within",
    "member call on", "constructor call on", "downcast of", "downcast of",
    "upcast of", "cast to virtual base of", "_Nonnull binding to",
    "dynamic operation on"
};


static void handleTypeMismatchImpl(TypeMismatchData *Data, ValueHandle Pointer,
                                   ReportOptions Opts) {
    Ubsan_Location Loc = { LK_Source, { Data->Loc } };


  uptr Alignment = (uptr)1 << Data->LogAlignment;
  ErrorType ET;
  if (Pointer == 0)
    ET = (Data->TypeCheckKind == TCK_NonnullAssign)
             ? ErrorType_NullPointerUseWithNullability
             : ErrorType_NullPointerUse;
  else if (Pointer & (Alignment - 1))
    ET = ErrorType_MisalignedPointerUse;
  else
    ET = ErrorType_InsufficientObjectSize;

#if 0
  // Use the SourceLocation from Data to track deduplication, even if it's
  // invalid.
  if (ignoreReport(Loc.getSourceLocation(), Opts, ET))
    return;

  SymbolizedStackHolder FallbackLoc;
  if (Data->Loc.isInvalid()) {
    FallbackLoc.reset(getCallerLocation(Opts.pc));
    Loc = FallbackLoc;
  }

  ScopedReport R(Opts, Loc, ET);
#else
    (void)Opts;
#endif

  switch (ET) {
  case ErrorType_NullPointerUse:
  case ErrorType_NullPointerUseWithNullability:
    Ubsan_Diag(&Loc, DL_Error, ET, "%s null pointer of type %s", TypeCheckKinds[Data->TypeCheckKind], Data->Type->TypeName);
    break;
  case ErrorType_MisalignedPointerUse:
    Ubsan_Diag(&Loc, DL_Error, ET, "%s misaligned address 0x%X for type %s, "
                        "which requires %i byte alignment", TypeCheckKinds[Data->TypeCheckKind], (void *)Pointer, Data->Type->TypeName, Alignment);
    break;
  case ErrorType_InsufficientObjectSize:
    Ubsan_Diag(&Loc, DL_Error, ET, "%s address 0x%X with insufficient space "
                        "for an object of type %s", TypeCheckKinds[Data->TypeCheckKind], (void *)Pointer, Data->Type->TypeName);
    break;
  default:
    UNREACHABLE("unexpected error type!");
  }

#if 0
  if (Pointer)
    Diag(Pointer, DL_Note, ET, "pointer points here");
#endif
}



void __ubsan_handle_type_mismatch_v1(TypeMismatchData *Data,
                                              ValueHandle Pointer) {
  GET_REPORT_OPTIONS(false);
  handleTypeMismatchImpl(Data, Pointer, Opts);
}
void __ubsan_handle_type_mismatch_v1_abort(TypeMismatchData *Data,
                                                    ValueHandle Pointer) {
  GET_REPORT_OPTIONS(true);
  handleTypeMismatchImpl(Data, Pointer, Opts);
  Die();
}


// TODO
#if 0
void __ubsan_handle_alignment_assumption(AlignmentAssumptionData *Data,
                                                  ValueHandle Pointer,
                                                  ValueHandle Alignment,
                                                  ValueHandle Offset) {
  GET_REPORT_OPTIONS(false);
  handleAlignmentAssumptionImpl(Data, Pointer, Alignment, Offset, Opts);
}
void __ubsan_handle_alignment_assumption_abort(
    AlignmentAssumptionData *Data, ValueHandle Pointer, ValueHandle Alignment,
    ValueHandle Offset) {
  GET_REPORT_OPTIONS(true);
  handleAlignmentAssumptionImpl(Data, Pointer, Alignment, Offset, Opts);
  Die();
}
#endif



/// \brief Common diagnostic emission for various forms of integer overflow.
static void handleIntegerOverflowImpl(OverflowData *Data, ValueHandle LHS,
                                      const char *Operator, ValueHandle RHS,
                                      ReportOptions Opts) {
    Ubsan_Location Loc = { LK_Source, { Data->Loc } };
    bool IsSigned = TypeDescriptor_isSignedIntegerTy(Data->Type);
    ErrorType ET = IsSigned ? ErrorType_SignedIntegerOverflow
                            : ErrorType_UnsignedIntegerOverflow;

#if 0
    if (ignoreReport(Loc, Opts, ET))
        return;

    // If this is an unsigned overflow in non-fatal mode, potentially ignore it.
    if (!IsSigned && !Opts.FromUnrecoverableHandler &&
        flags()->silence_unsigned_overflow)
        return;

    ScopedReport R(Opts, Loc, ET);
#else
    (void)Opts;
#endif

    Ubsan_Diag(&Loc, DL_Error, ET, "%s integer overflow: %i %s %i cannot be represented in type %s", (IsSigned ? "signed" : "unsigned"), LHS, Operator, RHS, Data->Type->TypeName);
}


#define UBSAN_OVERFLOW_HANDLER(handler_name, op, unrecoverable)                \
  void handler_name(OverflowData *Data, ValueHandle LHS,              \
                             ValueHandle RHS) {                                \
    GET_REPORT_OPTIONS(unrecoverable);                                         \
    handleIntegerOverflowImpl(Data, LHS, op, RHS, Opts);    \
    if (unrecoverable)                                                         \
      Die();                                                                   \
  }

UBSAN_OVERFLOW_HANDLER(__ubsan_handle_add_overflow, "+", false)
UBSAN_OVERFLOW_HANDLER(__ubsan_handle_add_overflow_abort, "+", true)
UBSAN_OVERFLOW_HANDLER(__ubsan_handle_sub_overflow, "-", false)
UBSAN_OVERFLOW_HANDLER(__ubsan_handle_sub_overflow_abort, "-", true)
UBSAN_OVERFLOW_HANDLER(__ubsan_handle_mul_overflow, "*", false)
UBSAN_OVERFLOW_HANDLER(__ubsan_handle_mul_overflow_abort, "*", true)



#if 0
void __ubsan_handle_negate_overflow(OverflowData *Data,
                                             ValueHandle OldVal) {
  GET_REPORT_OPTIONS(false);
  handleNegateOverflowImpl(Data, OldVal, Opts);
}
void __ubsan_handle_negate_overflow_abort(OverflowData *Data,
                                                    ValueHandle OldVal) {
  GET_REPORT_OPTIONS(true);
  handleNegateOverflowImpl(Data, OldVal, Opts);
  Die();
}
#endif



static void handleDivremOverflowImpl(OverflowData *Data, ValueHandle LHS,
                                     ValueHandle RHS, ReportOptions Opts) {
    Ubsan_Location Loc = { LK_Source, { Data->Loc } };

    ErrorType ET;
    if (Ubsan_Value_isMinusOne(Data->Type, RHS))
        ET = ErrorType_SignedIntegerOverflow;
    else if (TypeDescriptor_isIntegerTy(Data->Type))
        ET = ErrorType_IntegerDivideByZero;
    else
        ET = ErrorType_FloatDivideByZero;

    #if 0
    if (ignoreReport(Loc, Opts, ET))
        return;

    ScopedReport R(Opts, Loc, ET);
    #else
        (void)Opts;
    #endif

    switch (ET) {
    case ErrorType_SignedIntegerOverflow:
        Ubsan_Diag(&Loc, DL_Error, ET, "division of %i by -1 cannot be represented in type %s", LHS, Data->Type->TypeName);
        break;

    case ErrorType_IntegerDivideByZero:
    case ErrorType_FloatDivideByZero:
        Ubsan_Diag(&Loc, DL_Error, ET, "division by zero");
        break;

    default:
        UNREACHABLE("unexpected error type!");
        break;
    }
}

void __ubsan_handle_divrem_overflow(OverflowData *Data,
                                             ValueHandle LHS, ValueHandle RHS) {
  GET_REPORT_OPTIONS(false);
  handleDivremOverflowImpl(Data, LHS, RHS, Opts);
}
void __ubsan_handle_divrem_overflow_abort(OverflowData *Data,
                                                    ValueHandle LHS,
                                                    ValueHandle RHS) {
  GET_REPORT_OPTIONS(true);
  handleDivremOverflowImpl(Data, LHS, RHS, Opts);
  Die();
}



static void handleShiftOutOfBoundsImpl(ShiftOutOfBoundsData *Data,
                                       ValueHandle LHS, ValueHandle RHS,
                                       ReportOptions Opts) {
    Ubsan_Location Loc = { LK_Source, { Data->Loc } };

    ErrorType ET;
    if (Ubsan_Value_isNegative(Data->RHSType, RHS) ||
        Ubsan_Value_getPositiveIntValue(Data->RHSType, RHS) >= TypeDescriptor_getIntegerBitWidth(Data->LHSType))
        ET = ErrorType_InvalidShiftExponent;
    else
        ET = ErrorType_InvalidShiftBase;

#if 0
    if (ignoreReport(Loc, Opts, ET))
        return;

    ScopedReport R(Opts, Loc, ET);
#else
    (void)Opts;
#endif

    if (ET == ErrorType_InvalidShiftExponent) {
        if (Ubsan_Value_isNegative(Data->RHSType, RHS)) {
            Ubsan_Diag(&Loc, DL_Error, ET, "shift exponent %i is negative", RHS);
        } else {
            Ubsan_Diag(&Loc, DL_Error, ET, "shift exponent %i is too large for %i-bit type %s", RHS, TypeDescriptor_getIntegerBitWidth(Data->LHSType), Data->LHSType->TypeName);
        }
    } else {
        if (Ubsan_Value_isNegative(Data->LHSType, LHS)) {
            Ubsan_Diag(&Loc, DL_Error, ET, "left shift of negative value %i", LHS);
        } else {
            Ubsan_Diag(&Loc, DL_Error, ET,
                "left shift of %i by %i places cannot be represented in type %s", LHS, RHS, Data->LHSType->TypeName);
        }
    }
}


void __ubsan_handle_shift_out_of_bounds(ShiftOutOfBoundsData *Data,
                                                 ValueHandle LHS,
                                                 ValueHandle RHS) {
  GET_REPORT_OPTIONS(false);
  handleShiftOutOfBoundsImpl(Data, LHS, RHS, Opts);
}
void __ubsan_handle_shift_out_of_bounds_abort(
                                                     ShiftOutOfBoundsData *Data,
                                                     ValueHandle LHS,
                                                     ValueHandle RHS) {
  GET_REPORT_OPTIONS(true);
  handleShiftOutOfBoundsImpl(Data, LHS, RHS, Opts);
  Die();
}


static void handleOutOfBoundsImpl(OutOfBoundsData *Data, ValueHandle Index,
                                  ReportOptions Opts) {
    Ubsan_Location Loc = { LK_Source, { Data->Loc } };

    ErrorType ET = ErrorType_OutOfBoundsIndex;

#if 0
    if (ignoreReport(Loc, Opts, ET))
        return;

    ScopedReport R(Opts, Loc, ET);
#else
    (void)Opts;
#endif

    Ubsan_Diag(&Loc, DL_Error, ET, "index %i out of bounds for type %s", Index, Data->ArrayType->TypeName);
}

void __ubsan_handle_out_of_bounds(OutOfBoundsData *Data,
                                           ValueHandle Index) {
    GET_REPORT_OPTIONS(false);
    handleOutOfBoundsImpl(Data, Index, Opts);
}
void __ubsan_handle_out_of_bounds_abort(OutOfBoundsData *Data,
                                                 ValueHandle Index) {
    GET_REPORT_OPTIONS(true);
    handleOutOfBoundsImpl(Data, Index, Opts);
    Die();
}



#if 0
void __ubsan_handle_builtin_unreachable(UnreachableData *Data) {
  GET_REPORT_OPTIONS(true);
  handleBuiltinUnreachableImpl(Data, Opts);
  Die();
}

#if 0
// C++ only
void __ubsan_handle_missing_return(UnreachableData *Data) {
  GET_REPORT_OPTIONS(true);
  handleMissingReturnImpl(Data, Opts);
  Die();
}
#endif


void __ubsan_handle_vla_bound_not_positive(VLABoundData *Data,
                                                    ValueHandle Bound) {
  GET_REPORT_OPTIONS(false);
  handleVLABoundNotPositive(Data, Bound, Opts);
}
void __ubsan_handle_vla_bound_not_positive_abort(VLABoundData *Data,
                                                          ValueHandle Bound) {
  GET_REPORT_OPTIONS(true);
  handleVLABoundNotPositive(Data, Bound, Opts);
  Die();
}



void __ubsan_handle_float_cast_overflow(void *Data, ValueHandle From) {
  GET_REPORT_OPTIONS(false);
  handleFloatCastOverflow(Data, From, Opts);
}
void __ubsan_handle_float_cast_overflow_abort(void *Data,
                                                       ValueHandle From) {
  GET_REPORT_OPTIONS(true);
  handleFloatCastOverflow(Data, From, Opts);
  Die();
}


void __ubsan_handle_load_invalid_value(InvalidValueData *Data,
                                                ValueHandle Val) {
  GET_REPORT_OPTIONS(false);
  handleLoadInvalidValue(Data, Val, Opts);
}
void __ubsan_handle_load_invalid_value_abort(InvalidValueData *Data,
                                                      ValueHandle Val) {
  GET_REPORT_OPTIONS(true);
  handleLoadInvalidValue(Data, Val, Opts);
  Die();
}


void __ubsan_handle_implicit_conversion(ImplicitConversionData *Data,
                                                 ValueHandle Src,
                                                 ValueHandle Dst) {
  GET_REPORT_OPTIONS(false);
  handleImplicitConversion(Data, Opts, Src, Dst);
}
void __ubsan_handle_implicit_conversion_abort(
    ImplicitConversionData *Data, ValueHandle Src, ValueHandle Dst) {
  GET_REPORT_OPTIONS(true);
  handleImplicitConversion(Data, Opts, Src, Dst);
  Die();
}


void __ubsan_handle_invalid_builtin(InvalidBuiltinData *Data) {
  GET_REPORT_OPTIONS(true);
  handleInvalidBuiltin(Data, Opts);
}
void __ubsan_handle_invalid_builtin_abort(InvalidBuiltinData *Data) {
  GET_REPORT_OPTIONS(true);
  handleInvalidBuiltin(Data, Opts);
  Die();
}


void __ubsan_handle_invalid_objc_cast(InvalidObjCCast *Data,
                                               ValueHandle Pointer) {
  GET_REPORT_OPTIONS(false);
  handleInvalidObjCCast(Data, Pointer, Opts);
}
void __ubsan_handle_invalid_objc_cast_abort(InvalidObjCCast *Data,
                                                     ValueHandle Pointer) {
  GET_REPORT_OPTIONS(true);
  handleInvalidObjCCast(Data, Pointer, Opts);
  Die();
}



void __ubsan_handle_nonnull_return_v1(NonNullReturnData *Data,
                                               SourceLocation *LocPtr) {
  GET_REPORT_OPTIONS(false);
  handleNonNullReturn(Data, LocPtr, Opts, true);
}

void __ubsan_handle_nonnull_return_v1_abort(NonNullReturnData *Data,
                                                     SourceLocation *LocPtr) {
  GET_REPORT_OPTIONS(true);
  handleNonNullReturn(Data, LocPtr, Opts, true);
  Die();
}

void __ubsan_handle_nullability_return_v1(NonNullReturnData *Data,
                                                   SourceLocation *LocPtr) {
  GET_REPORT_OPTIONS(false);
  handleNonNullReturn(Data, LocPtr, Opts, false);
}

void __ubsan_handle_nullability_return_v1_abort(
    NonNullReturnData *Data, SourceLocation *LocPtr) {
  GET_REPORT_OPTIONS(true);
  handleNonNullReturn(Data, LocPtr, Opts, false);
  Die();
}


void __ubsan_handle_nonnull_arg(NonNullArgData *Data) {
  GET_REPORT_OPTIONS(false);
  handleNonNullArg(Data, Opts, true);
}

void __ubsan_handle_nonnull_arg_abort(NonNullArgData *Data) {
  GET_REPORT_OPTIONS(true);
  handleNonNullArg(Data, Opts, true);
  Die();
}

void __ubsan_handle_nullability_arg(NonNullArgData *Data) {
  GET_REPORT_OPTIONS(false);
  handleNonNullArg(Data, Opts, false);
}

void __ubsan_handle_nullability_arg_abort(NonNullArgData *Data) {
  GET_REPORT_OPTIONS(true);
  handleNonNullArg(Data, Opts, false);
  Die();
}
#endif

static void handlePointerOverflowImpl(PointerOverflowData *Data,
                                      ValueHandle Base,
                                      ValueHandle Result,
                                      ReportOptions Opts) {
    Ubsan_Location Loc = { LK_Source, { Data->Loc } };

    ErrorType ET;

    if (Base == 0 && Result == 0)
        ET = ErrorType_NullptrWithOffset;
    else if (Base == 0 && Result != 0)
        ET = ErrorType_NullptrWithNonZeroOffset;
    else if (Base != 0 && Result == 0)
        ET = ErrorType_NullptrAfterNonZeroOffset;
    else
        ET = ErrorType_PointerOverflow;

#if 0
    if (ignoreReport(Loc, Opts, ET))
        return;

    ScopedReport R(Opts, Loc, ET);
#else
    (void)Opts;
#endif

    if (ET == ErrorType_NullptrWithOffset) {
        Ubsan_Diag(&Loc, DL_Error, ET, "applying zero offset to null pointer");
    } else if (ET == ErrorType_NullptrWithNonZeroOffset) {
        Ubsan_Diag(&Loc, DL_Error, ET, "applying non-zero offset %X to null pointer", Result);
    } else if (ET == ErrorType_NullptrAfterNonZeroOffset) {
        Ubsan_Diag(&Loc, DL_Error, ET, "applying non-zero offset to non-null pointer %X produced null pointer", Base);
    } else if (((intptr_t)(Base) >= 0) == ((intptr_t)(Result) >= 0)) {
        if (Base > Result) {
            Ubsan_Diag(&Loc, DL_Error, ET, "addition of unsigned offset to %X overflowed to %X", (void *)Base, (void *)Result);
        } else {
            Ubsan_Diag(&Loc, DL_Error, ET, "subtraction of unsigned offset from %X overflowed to %X", (void *)Base, (void *)Result);
        }
    } else {
        Ubsan_Diag(&Loc, DL_Error, ET, "pointer index expression with base %X overflowed to %X", (void *)Base, (void *)Result);
    }
}

void __ubsan_handle_pointer_overflow(PointerOverflowData *Data,
                                              ValueHandle Base,
                                              ValueHandle Result) {
  GET_REPORT_OPTIONS(false);
  handlePointerOverflowImpl(Data, Base, Result, Opts);
}

void __ubsan_handle_pointer_overflow_abort(PointerOverflowData *Data,
                                                    ValueHandle Base,
                                                    ValueHandle Result) {
  GET_REPORT_OPTIONS(true);
  handlePointerOverflowImpl(Data, Base, Result, Opts);
  Die();
}


#if 0

void __ubsan_handle_cfi_check_fail(CFICheckFailData *Data,
                                            ValueHandle Value,
                                            uptr ValidVtable) {
  GET_REPORT_OPTIONS(false);
  if (Data->CheckKind == CFITCK_ICall || Data->CheckKind == CFITCK_NVMFCall)
    handleCFIBadIcall(Data, Value, Opts);
  else
    __ubsan_handle_cfi_bad_type(Data, Value, ValidVtable, Opts);
}

void __ubsan_handle_cfi_check_fail_abort(CFICheckFailData *Data,
                                                  ValueHandle Value,
                                                  uptr ValidVtable) {
  GET_REPORT_OPTIONS(true);
  if (Data->CheckKind == CFITCK_ICall || Data->CheckKind == CFITCK_NVMFCall)
    handleCFIBadIcall(Data, Value, Opts);
  else
    __ubsan_handle_cfi_bad_type(Data, Value, ValidVtable, Opts);
  Die();
}
#endif
