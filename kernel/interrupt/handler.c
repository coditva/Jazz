#include "interrupt/handler.h"
#include "util/portio.h"
#include "util/types.h"
#include "util/keyboard_map.h"
#include "video/video.h"
#include "debug/debug.h"

void keyboard_handler(void)
{
  uint8_t status;

  write_port(0x20, 0x20);   /* ask for status */
  status = read_port(0x64); /* get status */

  if (status & 0x01) {      /* buffer is not empty */
    char data = -1;
    if ((data = read_port(0x60)) < 0) return; /* invalid data */
    kputc(keyboard_map[data]);
    update_cursor();
  }
}

void div_by_zero_handler(void)
{
  uint8_t status;
  kprintf("ISR: Divide by zero error");
}

void double_fault_handler(void)
{
  klog(ERR_ERROR, "\n------- DOUBLE FAULT -------\n");
}
