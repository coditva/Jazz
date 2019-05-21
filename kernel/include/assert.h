#ifndef ASSERT_H_TVZEHNNF
#define ASSERT_H_TVZEHNNF

# ifdef DEBUG

#  define assert(CONDITION) \
  if (!(CONDITION)) { \
    klog(LOG_ERROR, "assertion failed at %s:%d\n", __FILE__, __LINE__); \
    while (1); /* halt execution */ \
  }

# else
#  define assert(...)
# endif

#endif /* end of include guard: ASSERT_H_TVZEHNNF */
