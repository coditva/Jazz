#ifndef TYPES_H_V0Q5GEL4
#define TYPES_H_V0Q5GEL4

#include <config.h>

#ifdef ARCH_X86_64
#define CONFIG_64BIT
#include <arch/x86/include/types.h>
#endif

#ifdef ARCH_X86
#include <arch/x86/include/types.h>
#endif

#endif /* end of include guard: TYPES_H_V0Q5GEL4 */
