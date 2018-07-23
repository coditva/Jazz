#include "idt/idt.h"
#include "isr/isr.h"
#include "util/video.h"
#include "util/util.h"
#include "util/keyboard_map.h"

extern void keyboard_handler_int(void);

void keyboard_handler(void)
{
  uint8_t status;

  write_port(0x20, 0x20);   /* ask for status */
  status = read_port(0x64); /* get status */

  if (status & 0x01) {      /* buffer is not empty */
    char data = -1;
    if ((data = read_port(0x60)) < 0) return; /* invalid data */
    kputc(keyboard_map[data]);
  }
}

void isr_set_keyboard(void)
{
  uint32_t keyboard_address = (uint32_t)keyboard_handler_int;
  idt_set_gate(0x21, keyboard_address, 0x08, 0x8e);
}

void isr_init_keyboard(void)
{
  /* TODO: This will disable all other interrupts; prevent that */
  write_port(0x21, 0xfd);
}
