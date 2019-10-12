#ifndef ASSERT_H_TVZEHNNF
#define ASSERT_H_TVZEHNNF

#include <config.h>
#include <logger.h>

# ifdef DEBUG

/**
 * Check if certain condition is true. It prints out the file and line number
 * where the condition failed. Use for checking assumptions.
 */
#  define assert(CONDITION) \
  if (!(CONDITION)) { \
    klog(LOG_ERROR, "assertion failed at %s:%d\n", __FILE__, __LINE__); \
    while (1); /* halt execution */ \
  }

# else
#  define assert(...)
# endif

#endif /* end of include guard: ASSERT_H_TVZEHNNF */
