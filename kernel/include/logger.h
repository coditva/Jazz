#ifndef DEBUG_H_IEXU6FAG
#define DEBUG_H_IEXU6FAG

#include "config.h"

# define LOG_ERROR  0
# define LOG_WARN   1
# define LOG_INFO   2
# define LOG_DEBUG  3

# ifdef DEBUG

#  ifdef DEBUG_TO_SERIAL
#   include "serial/serial.h"
#   define klog(LOG_LEVEL, LOG_FORMAT, ...) \
        serial_printf(SERIAL_PORT1, LOG_FORMAT, ##__VA_ARGS__)

#  else
#   include <kio.h>
#   define klog(LOG_LEVEL, LOG_FORMAT, ...) \
        keprintf(LOG_LEVEL, LOG_FORMAT, ##__VA_ARGS__)
#  endif /* end of DEBUG_TO_SERIAL */

# else
#  define klog(...)
# endif /* end of DEBUG */

#endif /* end of include guard: DEBUG_H_IEXU6FAG */
