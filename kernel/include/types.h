/**
 * Defines the 2nd layer of types to be used internally.
 */
#ifndef TYPES_H_VMW2LPX1
#define TYPES_H_VMW2LPX1

#include <include/asm/types.h>

#ifndef _SIZE_T
#define _SIZE_T
typedef __kernel_size_t size_t;
#endif /* ifndef _SIZE_T */

#ifndef _INT_TYPES
#define _INT_TYPES // NOLINT

typedef __kernel_int8_t  int8_t;
typedef __kernel_uint8_t uint8_t;

typedef __kernel_int16_t  int16_t;
typedef __kernel_uint16_t uint16_t;

typedef __kernel_int32_t  int32_t;
typedef __kernel_uint32_t uint32_t;

typedef __kernel_uint64_t uint64_t;
typedef __kernel_int64_t  int64_t;

typedef __kernel_ulong_t uintptr_t;
typedef __kernel_long_t  intptr_t;

#endif /* ifndef _INT_TYPES */

#ifndef NULL
#define NULL (void *)0x0
#endif /* ifndef NULL */

#ifndef bool
#define bool unsigned short int
#define true 1
#define false 0
#endif /* ifndef bool */

#endif /* end of include guard: TYPES_H_VMW2LPX1 */
