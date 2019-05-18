#include <kio.h>
#include <logger.h>
#include <multiboot.h>
#include <page_frame.h>
#include <kcheck.h>

#include "io/video/video.h"
#include "io/serial/serial.h"
#include "interrupt/interrupt.h"
#include "boot/gdt.h"

extern uint32_t endkernel;

extern void kmain(multiboot_info_t *multiboot_info, uint32_t multiboot_magic)
{
  video_clear();

  if (multiboot_magic != MULTIBOOT_EAX_MAGIC) {
    klog(LOG_ERROR, "invalid multiboot signature\n");
    return;
  }

#ifdef DEBUG
  multiboot_dump_info(multiboot_info, multiboot_magic);
#endif

  klog(LOG_DEBUG, "uintptr_t: %x\n", sizeof(uintptr_t));
  klog(LOG_DEBUG, "unsigned long: %x\n", sizeof(unsigned long));
  klog(LOG_DEBUG, "unsigned long long: %x\n", sizeof(unsigned long long));
  page_frame_init((uintptr_t)&endkernel, multiboot_info);
#ifdef DEBUG
  page_frame_dump_map();
  void *page1 = NULL, *page2 = NULL;

  /* sanity check for page_frame */
  page1 = page_frame_alloc();
  page_frame_free(page1);
  page2 = page_frame_alloc();
  kcheck(page1 == page2, "page_frame_alloc()");
#endif

  serial_init();
  gdt_init();
  idt_init();
  isr_set_keyboard();
  isr_set_double_fault();
  isr_init_keyboard();

  klog(LOG_INFO, "\nInitialization complete.\n");
  kprintf("\n$ ");

  while(1)
    ; /* nop */
}
