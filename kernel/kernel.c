#include "util/video.h"
#include "interrupt/interrupt.h"

extern void kmain()
{
  kclear_screen();

  idt_init();

  isr_set_keyboard();
  isr_init_keyboard();

  kprintf("type here: ");

  while(1)
    ; /* nop */
}
