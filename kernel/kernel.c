#include "util/video.h"
#include "idt/idt.h"
#include "isr/isr.h"

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
