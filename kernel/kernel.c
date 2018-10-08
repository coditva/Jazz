#include "video/video.h"
#include "serial/serial.h"
#include "interrupt/interrupt.h"
#include "memory/gdt.h"

#include <logger.h>
#include <kio.h>

extern void kmain()
{
  video_clear();

  klog(LOG_DEBUG, "\nInitializing Serial ports... ");
  serial_init(SERIAL_PORT1);
  serial_init(SERIAL_PORT2);
  serial_init(SERIAL_PORT3);
  serial_init(SERIAL_PORT4);
  klog(LOG_DEBUG, "OK");

  klog(LOG_DEBUG, "\nInitializing GDT... ");
  gdt_init();
  klog(LOG_DEBUG, "OK");

  klog(LOG_DEBUG, "\nInitializing IDT... ");
  idt_init();
  klog(LOG_DEBUG, "OK");

  klog(LOG_DEBUG, "\nInitializing ISRs... ");
  isr_set_keyboard();
  isr_set_double_fault();
  isr_init_keyboard();
  klog(LOG_DEBUG, "OK");

  klog(LOG_DEBUG, "\nInitialization complete.\n");
  kprintf("\n$ ");

  while(1)
    ; /* nop */
}
