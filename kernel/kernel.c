#include "video/video.h"
#include "serial/serial.h"
#include "interrupt/interrupt.h"

extern void kmain()
{
  kclear_screen();

  eprintf(ERR_LOG, "\nInitializing Serial ports... ");
  serial_init(SERIAL_PORT1);
  serial_init(SERIAL_PORT2);
  serial_init(SERIAL_PORT3);
  serial_init(SERIAL_PORT4);
  eprintf(ERR_LOG, "OK");

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
