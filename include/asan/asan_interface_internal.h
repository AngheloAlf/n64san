#ifndef ASAN_INTERFACE_INTERNAL_H
#define ASAN_INTERFACE_INTERNAL_H

#include "sanitizer_common/sanitizer_internal_defs.h"

SANITIZER_INTERFACE_ATTRIBUTE
extern int __asan_option_detect_stack_use_after_return;

#endif