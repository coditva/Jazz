#include "serial.h"
#include "util/portio.h"

#define SERIAL_RECEIVED(_PORT)    (read_port(_PORT + 5) & 1)
#define SERIAL_SEND_EMPTY(_PORT)  (read_port(_PORT + 5) & 1)

void serial_write(int port, int data)
{
  while (SERIAL_SEND_EMPTY(port) == 0)
    ; /* nop */

  write_port(port, data);
}

int serial_read(int port)
{
  while (SERIAL_RECEIVED(port) == 0)
    ; /* nop */

  return read_port(port);
}
