# N64 UBSan

Minimal Undefined Behaviour Sanitizer implementation for N64 games.

UBSan is an undefined behaviour checker. UBSan modifies the program at compile-time to catch various kinds of undefined behavior during program execution.

For official documentation, see:

- <https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html>
- <https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html>

## Implemented options

The following is a list of compiler flags which _should_ be supported by this runtime library. Newer compilers may break compatibility.

- `-fsanitize=shift`
  - This option enables checking that the result of a shift operation is not undefined. Note that what exactly is considered undefined differs slightly between ISO C90 and C99. This option has two suboptions, `-fsanitize=shift-base` and `-fsanitize=shift-exponent`.

  - `-fsanitize=shift-exponent`
    - This option enables checking that the second argument of a shift operation is not negative and is smaller than the precision of the promoted first argument.
  - `-fsanitize=shift-base`
    - If the second argument of a shift operation is within range, check that the result of a shift operation is not undefined. Note that what exactly is considered undefined differs slightly between ISO C90 and C99.

- `-fsanitize=integer-divide-by-zero`
  - Detect integer division by zero.

- `-fsanitize=null`
  - This option enables pointer checking. Particularly, the application built with this option turned on will issue an error message when it tries to dereference a NULL pointer, or if a reference (possibly an rvalue reference) is bound to a NULL pointer, or if a method is invoked on an object pointed by a NULL pointer.

- `-fsanitize=signed-integer-overflow`
  - This option enables signed integer overflow checking. We check that the result of +, *, and both unary and binary - does not overflow in the signed arithmetics. This also detects INT_MIN / -1 signed division. Note, integer promotion rules must be taken into account.

- `-fsanitize=bounds`
  - This option enables instrumentation of array bounds. Various out of bounds accesses are detected. Flexible array members, flexible array member-like arrays, and initializers of variables with static storage are not instrumented.

- `-fsanitize=alignment`
  - This option enables checking of alignment of pointers when they are dereferenced, or when a reference is bound to insufficiently aligned target.

- `-fsanitize=object-size`
  - This option enables instrumentation of memory references using the __builtin_object_size function. Various out of bounds pointer accesses are detected.

- `-fsanitize=float-divide-by-zero`
  - Detect floating-point division by zero. Unlike other similar options, `-fsanitize=float-divide-by-zero` is not enabled by `-fsanitize=undefined`, since floating-point division by zero can be a legitimate way of obtaining infinities and NaNs.

- `-fsanitize=float-cast-overflow`
  - This option enables floating-point type to integer conversion checking. We check that the result of the conversion does not overflow. Unlike other similar options, `-fsanitize=float-cast-overflow` is not enabled by `-fsanitize=undefined`. This option does not work well with FE_INVALID exceptions enabled.

- `-fsanitize=pointer-overflow`
  - This option enables instrumentation of pointer arithmetics. If the pointer arithmetics overflows, a run-time error is issued.
