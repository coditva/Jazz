#ifndef STDARG_H_WG3Q0UEA
#define STDARG_H_WG3Q0UEA

#define va_list char *

#define va_rounded_size(TYPE)                                                  \
  ((sizeof(TYPE) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#define va_start(AP, LAST) ((AP) = (va_list) & (LAST) + va_rounded_size(LAST))

#define va_arg(AP, TYPE)                                                       \
  (*(TYPE *)(((AP) += va_rounded_size(TYPE)) - va_rounded_size(TYPE)))

#define va_end(AP)

#endif /* end of include guard: STDARG_H_WG3Q0UEA */
