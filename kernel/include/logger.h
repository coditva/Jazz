#ifndef DEBUG_H_IEXU6FAG
#define DEBUG_H_IEXU6FAG

# include "config.h"

# define LOG_ERROR  0
# define LOG_WARN   1
# define LOG_INFO   2
# define LOG_DEBUG  3

# define LOG_HRULE \
    "------------------------------------------------------------------------\n"

# ifdef DEBUG
#  include<kio.h>

#  define klog(LOG_LEVEL, FORMAT, ...) \
       keprintf(LOG_LEVEL, FORMAT, ##__VA_ARGS__)

#  define klog_status_init(INIT_STRING) \
       keprintf(LOG_DEBUG, "[INIT] %s\n", INIT_STRING)

#  define klog_status_ok(INIT_STRING) \
       keprintf(LOG_INFO, "[ OK ] %s\n", INIT_STRING)

#  define klog_status_fail(INIT_STRING) \
       keprintf(LOG_ERROR, "[FAIL] %s\n", INIT_STRING)

# else  /* ifdef DEBUG */
#  define klog(...)
#  define klog_status_init(...)
#  define klog_status_ok(...)
#  define klog_status_fail(...)
# endif /* ifdef DEBUG */

#endif /* end of include guard: DEBUG_H_IEXU6FAG */
