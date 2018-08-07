#include "video/video.h"
#include "serial/serial.h"
#include "interrupt/interrupt.h"

#include "debug/debug.h"

extern void kmain()
{
  kclear_screen();

  klog(ERR_LOG, "\nInitializing Serial ports... ");
  serial_init(SERIAL_PORT1);
  serial_init(SERIAL_PORT2);
  serial_init(SERIAL_PORT3);
  serial_init(SERIAL_PORT4);
  klog(ERR_LOG, "OK");

  klog(ERR_LOG, "\nInitializing IDT... ");
  idt_init();
  klog(ERR_LOG, "OK");

  klog(ERR_LOG, "\nInitializing ISRs... ");
  isr_set_keyboard();
  isr_set_double_fault();
  isr_init_keyboard();
  klog(ERR_LOG, "OK");

  klog(ERR_LOG, "\nInitialization complete.\n");
  kprintf("\n$ ");

  while(1)
    ; /* nop */
}
