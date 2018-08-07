#ifndef DEBUG_H_IEXU6FAG
#define DEBUG_H_IEXU6FAG

#include "config.h"

#ifdef DEBUG

# define ERR_ERROR 0
# define ERR_WARN  1
# define ERR_INFO  2
# define ERR_LOG   3

# ifdef DEBUG_TO_SERIAL
#  include "serial/serial.h"
#  define klog(LOG_LEVEL, LOG_FORMAT, ...) serial_printf(SERIAL_PORT1, LOG_FORMAT, ##__VA_ARGS__)

# else
#  include "video/video.h"
#  define klog(LOG_LEVEL, LOG_FORMAT, ...) eprintf(LOG_LEVEL, LOG_FORMAT, ##__VA_ARGS__)

# endif /* end of DEBUG_TO_SERIAL */

#endif /* end of DEBUG */

#endif /* end of include guard: DEBUG_H_IEXU6FAG */
