#include <kio.h>
#include <logger.h>
#include <multiboot.h>

#include "io/video/video.h"
#include "io/serial/serial.h"
#include "interrupt/interrupt.h"
#include "memory/gdt.h"

extern void kmain(multiboot_info_t *multiboot_info, uint32_t multiboot_magic)
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
