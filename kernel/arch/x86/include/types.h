#ifndef TYPES_H_6BL3PEFC
#define TYPES_H_6BL3PEFC

#include "arch/generic/include/types.h"

#ifdef CONFIG_64BIT
# define BITS_PER_LONG 64
typedef  __u64  __kernel_ulong_t;
typedef  __s64  __kernel_long_t;

#else
# define BITS_PER_LONG 32
typedef  __u32  __kernel_ulong_t;
typedef  __s32  __kernel_long_t;

#endif

#endif /* end of include guard: TYPES_H_6BL3PEFC */
