#ifndef DEBUG_H_IEXU6FAG
#define DEBUG_H_IEXU6FAG

#include "config.h"

# define LOG_ERROR  0
# define LOG_WARN   1
# define LOG_INFO   2
# define LOG_DEBUG  3

# ifdef DEBUG
#  define klog(LOG_LEVEL, FORMAT, ...) \
       keprintf(LOG_LEVEL, FORMAT, ##__VA_ARGS__)
# else
#  define klog(...)
# endif /* end of DEBUG */

#endif /* end of include guard: DEBUG_H_IEXU6FAG */
