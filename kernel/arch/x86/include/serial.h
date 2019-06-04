#ifndef SERIAL_H_FT2N9HWC
#define SERIAL_H_FT2N9HWC

#include <config.h>

#define SERIAL_PORT1 0x3f8
#define SERIAL_PORT2 0x2f8
#define SERIAL_PORT3 0x3e8
#define SERIAL_PORT4 0x2e8

#ifdef DEBUG_TO_SERIAL
# include <arch/x86/include/serial.h>
# define DEBUG_SERIAL_PORT SERIAL_PORT1
#endif

void serial_init    ();
void serial_write   (int port, char *data);
int  serial_read    (int port);

#endif /* end of include guard: SERIAL_H_FT2N9HWC */
