#ifndef ALIGN_H_VTBWC7HK
#define ALIGN_H_VTBWC7HK

#include <types.h>

#define align(base, size) \
  ((uintptr_t)base % size) \
  ? ((uintptr_t)base & ~(size - 1)) + size \
  : (uintptr_t)base

#define align_to_type(base, type)   align(base, sizeof(type))

#endif /* end of include guard: ALIGN_H_VTBWC7HK */
