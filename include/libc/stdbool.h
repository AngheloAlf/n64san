#ifndef STDBOOL
#define STDBOOL

#include "ultra64/types.h"

#define __bool_true_false_are_defined 1

#ifndef __cplusplus

#define bool u32
#define false 0
#define true 1

#endif /* __cplusplus */

#endif /* STDBOOL */
