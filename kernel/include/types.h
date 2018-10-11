/**
 * Defines the 2nd layer of types to be used internally.
 */
#ifndef TYPES_H_VMW2LPX1
#define TYPES_H_VMW2LPX1

#include "util/types.h"

#ifndef _SIZE_T
#define _SIZE_T
typedef __kernel_size_t size_t;
#endif /* ifndef _SIZE_T */

#ifndef _INT_TYPES
#define _INT_TYPES
typedef __kernel_int8_t     int8_t;
typedef __kernel_uint8_t    uint8_t;
typedef __kernel_int16_t    int16_t;
typedef __kernel_uint16_t   uint16_t;
typedef __kernel_int32_t    int32_t;
typedef __kernel_uint32_t   uint32_t;
typedef __kernel_uint64_t   uint64_t;
typedef __kernel_int64_t    int64_t;
#endif /* ifndef _INT_TYPES */

#define types_concat(HIGH, LOW)     ( (HIGH << (sizeof(HIGH))) | LOW )

#ifndef NULL
#define NULL (void *)0x0
#endif /* ifndef NULL */

#endif /* end of include guard: TYPES_H_VMW2LPX1 */
