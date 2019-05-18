#include "io/portio/portio.h"
#include "io/serial/serial.h"
#include <io.h>
#include <logger.h>
#include <stdarg.h>

#define SERIAL_RECEIVED(_PORT)    (read_port((_PORT) + 5) & 1)
#define SERIAL_SEND_EMPTY(_PORT)  (read_port((_PORT) + 5) & 0x20)

static inline void serial_init_port(int port)
{
  write_port(port + 1, 0x00);    // Disable all interrupts
  write_port(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
  write_port(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
  write_port(port + 1, 0x00);    //                  (hi byte)
  write_port(port + 3, 0x03);    // 8 bits, no parity, one stop bit
  write_port(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
  write_port(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

void serial_init()
{
  klog_status_init("serial ports");

  serial_init_port(SERIAL_PORT1);
  serial_init_port(SERIAL_PORT2);
  serial_init_port(SERIAL_PORT3);
  serial_init_port(SERIAL_PORT4);

  klog_status_ok("serial ports");
}

void serial_write(int port, char *data)
{
  while(*data != '\0') {
    while (SERIAL_SEND_EMPTY(port) == 0) {
      ; /* nop */
}

    write_port(port, (int)*data);
    data++;
  }
}

int serial_read(int port)
{
  while (SERIAL_RECEIVED(port) == 0) {
    ; /* nop */
}

  return read_port(port);
}
