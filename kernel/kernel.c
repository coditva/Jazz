#include "util/video.h"
#include "interrupt/interrupt.h"

extern void kmain()
{
  kclear_screen();

  idt_init();

  isr_set_keyboard();
  isr_set_double_fault();
  isr_init_keyboard();

  kprintf("type here: ");

  while(1)
    ; /* nop */
}
