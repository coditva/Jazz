#include <kio.h>
#include <logger.h>
#include <multiboot.h>
#include <page_frame.h>

#include "io/video/video.h"
#include "io/serial/serial.h"
#include "interrupt/interrupt.h"
#include "boot/gdt.h"

extern void kmain(multiboot_info_t *multiboot_info, uint32_t multiboot_magic)
{
  video_clear();

  multiboot_dump_info(multiboot_info);
  page_frame_init(multiboot_info);
  page_frame_dump_map();

  serial_init();
  gdt_init();
  idt_init();
  isr_set_keyboard();
  isr_set_double_fault();
  isr_init_keyboard();

  klog(LOG_DEBUG, "\nInitialization complete.\n");
  kprintf("\n$ ");

  while(1)
    ; /* nop */
}
