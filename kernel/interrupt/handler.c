#include <types.h>
#include <logger.h>
#include <kio.h>
#include "interrupt/handler.h"
#include "io/portio/portio.h"
#include "util/keyboard_map.h"
#include "io/video/video.h"

void keyboard_handler(void)
{
  uint8_t status;

  write_port(0x20, 0x20);   /* ask for status */
  status = read_port(0x64); /* get status */

  if (status & 0x01) {      /* buffer is not empty */
    char data = -1;
    if ((data = read_port(0x60)) < 0)  /* invalid data */
      return;
    kputc(keyboard_map[(unsigned int)data]);
  }
}

void div_by_zero_handler(void)
{
  klog(LOG_ERROR, "\n------ DIVIDE BY ZERO ------\n");
}

void double_fault_handler(void)
{
  klog(LOG_ERROR, "\n------- DOUBLE FAULT -------\n");
}
