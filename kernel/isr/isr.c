#include "idt/idt.h"
#include "isr/isr.h"
#include "util/video.h"
#include "util/util.h"

extern void keyboard_handler_int(void);

void keyboard_handler(void)
{
  /* TODO */
  kputc('a');
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
