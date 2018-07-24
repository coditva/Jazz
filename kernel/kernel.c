#include "util/video.h"
#include "interrupt/interrupt.h"

extern void kmain()
{
  kclear_screen();

  eprintf(ERR_LOG, "\nInitializing IDT... ");
  idt_init();
  eprintf(ERR_LOG, "OK");

  eprintf(ERR_LOG, "\nInitializing ISRs... ");
  isr_set_keyboard();
  isr_set_double_fault();
  isr_init_keyboard();
  eprintf(ERR_LOG, "OK");

  eprintf(ERR_LOG, "\nInitialization complete.\n");
  kprintf("\n$ ");

  while(1)
    ; /* nop */
}
