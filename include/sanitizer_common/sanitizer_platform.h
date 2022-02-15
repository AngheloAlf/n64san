#ifndef SANITIZER_PLATFORM_H
#define SANITIZER_PLATFORM_H

/**
 * Common platform macros.
 */

#define SANITIZER_GLIBC 0
#define SANITIZER_WORDSIZE 32

#define SANITIZER_MIPS 1
#define SANITIZER_MIPS32 1
#define SANITIZER_MIPS64 0

// SANITIZER_SUPPORTS_THREADLOCAL
// 1 - THREADLOCAL macro is supported by target
// 0 - THREADLOCAL macro is not supported by target
#define SANITIZER_SUPPORTS_THREADLOCAL 0

#endif  // SANITIZER_PLATFORM_H
