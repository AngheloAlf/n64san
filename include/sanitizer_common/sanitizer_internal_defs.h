#ifndef SANITIZER_DEFS_H
#define SANITIZER_DEFS_H

/**
 * Contains macro used in run-time libraries code.
 */

#include "sanitizer_platform.h"
#include "global_headers/libc.h"

#define uptr uintptr_t

#ifndef SANITIZER_DEBUG
# define SANITIZER_DEBUG 0
#endif

#define SANITIZER_STRINGIFY_(S) #S
#define SANITIZER_STRINGIFY(S) SANITIZER_STRINGIFY_(S)

// Only use SANITIZER_*ATTRIBUTE* before the function return type!
#define SANITIZER_INTERFACE_ATTRIBUTE __attribute__((visibility("default")))
#define SANITIZER_WEAK_ATTRIBUTE

// TLS is handled differently on different platforms
#define SANITIZER_TLS_INITIAL_EXEC_ATTRIBUTE

// GCC does not understand __has_feature
#if !defined(__has_feature)
# define __has_feature(x) 0
#endif

// Older GCCs do not understand __has_attribute.
#if !defined(__has_attribute)
# define __has_attribute(x) 0
#endif

#if !defined(__has_cpp_attribute)
#  define __has_cpp_attribute(x) 0
#endif


// ----------- ATTENTION -------------
// This header should NOT include any other headers to avoid portability issues.

// Common defs.
#define INTERFACE_ATTRIBUTE SANITIZER_INTERFACE_ATTRIBUTE

// Platform-specific defs.
# define ALWAYS_INLINE inline __attribute__((always_inline))
# define ALIAS(x) __attribute__((alias(x)))
// Please only use the ALIGNED macro before the type.
// Using ALIGNED after the variable declaration is not portable!
# define ALIGNED(x) __attribute__((aligned(x)))
# define FORMAT(f, a)  __attribute__((format(printf, f, a)))
# define NOINLINE __attribute__((noinline))
# define NORETURN  __attribute__((noreturn))
# define THREADLOCAL   __thread
# define LIKELY(x)     __builtin_expect(!!(x), 1)
# define UNLIKELY(x)   __builtin_expect(!!(x), 0)

#  define PREFETCH(x) __builtin_prefetch(x)
# define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#define UNUSED __attribute__((unused))
#define USED __attribute__((used))

#if !defined(_MSC_VER) || defined(__clang__)
# define NOEXCEPT noexcept
#else
# define NOEXCEPT throw()
#endif

// TODO?
#  define FALLTHROUGH

// NOTE: Functions below must be defined in each run-time.
void NORETURN Die();

void NORETURN CheckFailed(const char *file, int line, const char *cond,
                          u64 v1, u64 v2);

// Check macro
#define RAW_CHECK_MSG(expr, msg, ...)          \
  do {                                         \
    if (UNLIKELY(!(expr))) {                   \
      const char* msgs[] = {msg, __VA_ARGS__}; \
      for (const char* m : msgs) RawWrite(m);  \
      Die();                                   \
    }                                          \
  } while (0)

#define RAW_CHECK(expr) RAW_CHECK_MSG(expr, #expr "\n", )
#define RAW_CHECK_VA(expr, ...) RAW_CHECK_MSG(expr, #expr "\n", __VA_ARGS__)

#define CHECK_IMPL(c1, op, c2) \
  do { \
    __sanitizer::u64 v1 = (__sanitizer::u64)(c1); \
    __sanitizer::u64 v2 = (__sanitizer::u64)(c2); \
    if (UNLIKELY(!(v1 op v2))) \
      __sanitizer::CheckFailed(__FILE__, __LINE__, \
        "(" #c1 ") " #op " (" #c2 ")", v1, v2); \
  } while (false) \
/**/

#define CHECK(a)       CHECK_IMPL((a), !=, 0)
#define CHECK_EQ(a, b) CHECK_IMPL((a), ==, (b))
#define CHECK_NE(a, b) CHECK_IMPL((a), !=, (b))
#define CHECK_LT(a, b) CHECK_IMPL((a), <,  (b))
#define CHECK_LE(a, b) CHECK_IMPL((a), <=, (b))
#define CHECK_GT(a, b) CHECK_IMPL((a), >,  (b))
#define CHECK_GE(a, b) CHECK_IMPL((a), >=, (b))

#if SANITIZER_DEBUG
#define DCHECK(a)       CHECK(a)
#define DCHECK_EQ(a, b) CHECK_EQ(a, b)
#define DCHECK_NE(a, b) CHECK_NE(a, b)
#define DCHECK_LT(a, b) CHECK_LT(a, b)
#define DCHECK_LE(a, b) CHECK_LE(a, b)
#define DCHECK_GT(a, b) CHECK_GT(a, b)
#define DCHECK_GE(a, b) CHECK_GE(a, b)
#else
#define DCHECK(a)
#define DCHECK_EQ(a, b)
#define DCHECK_NE(a, b)
#define DCHECK_LT(a, b)
#define DCHECK_LE(a, b)
#define DCHECK_GT(a, b)
#define DCHECK_GE(a, b)
#endif

#define UNREACHABLE(msg) do { \
  CHECK(0 && msg); \
  Die(); \
} while (0)

#define UNIMPLEMENTED() UNREACHABLE("unimplemented")

#define COMPILER_CHECK(pred) static_assert(pred, "")

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))


#define GET_CALLER_PC() __builtin_return_address(0)
#define GET_CURRENT_FRAME() __builtin_frame_address(0)
inline void Trap() {
  __builtin_trap();
}


#endif  // SANITIZER_DEFS_H
