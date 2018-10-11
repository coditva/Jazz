#ifndef DEBUG_H_IEXU6FAG
#define DEBUG_H_IEXU6FAG

#include "config.h"

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

#  define klog_status_init(LOG_LEVEL, INIT_STRING) \
       keprintf(LOG_LEVEL, "Initializing %s...", INIT_STRING)

#  ifdef DEBUG_TO_SERIAL
#   define STATUS_FORMAT_STRING "%s\n"
#  else
#   define STATUS_FORMAT_STRING "\n%c%c%c%c%c%c%s", 8, 8, 8, 8, 8, 8
#  endif
#  define klog_status_ok(LOG_LEVEL) \
       keprintf(LOG_LEVEL, STATUS_FORMAT_STRING, "  [OK]");

#  define klog_status_fail(LOG_LEVEL) \
       keprintf(LOG_LEVEL, STATUS_FORMAT_STRING, "[FAIL]");

# else
#  define klog(...)
#  define klog_status_init(...)
#  define klog_status_ok(...)
#  define klog_status_fail(...)
# endif /* end of DEBUG */

#endif /* end of include guard: DEBUG_H_IEXU6FAG */
