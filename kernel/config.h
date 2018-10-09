#ifndef CONFIG_H_9LYYBTSW
#define CONFIG_H_9LYYBTSW

/* Comment following line to stop printing debug messages */
#define DEBUG


#ifdef DEBUG
# define LOG_LEVEL ERR_INFO

/* Uncomment this line to send debug output to serial ports: */
# define DEBUG_TO_SERIAL 

#ifdef DEBUG_TO_SERIAL
  #include "io/serial/serial.h"
  #define DEBUG_SERIAL_PORT SERIAL_PORT1
#endif

#endif

#endif /* end of include guard: CONFIG_H_9LYYBTSW */
