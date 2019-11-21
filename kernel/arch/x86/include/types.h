#ifndef TYPES_H_6BL3PEFC
#define TYPES_H_6BL3PEFC

#include <arch/generic/include/types.h>

#ifdef CONFIG_64BIT
#define BITS_PER_LONG    64
#define __kernel_ulong_t __u64
#define __kernel_long_t  __s64
#else
#define BITS_PER_LONG    32
#define __kernel_ulong_t __u32
#define __kernel_long_t  __s32
#endif

#endif /* end of include guard: TYPES_H_6BL3PEFC */
