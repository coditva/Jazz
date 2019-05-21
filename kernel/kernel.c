#include <kcheck.h>
#include <kio.h>
#include <logger.h>
#include <mm/page_frame.h>
#include <mm/paging.h>
#include <multiboot.h>

#include "boot/gdt.h"
#include "interrupt/interrupt.h"
#include "io/serial/serial.h"
#include "io/video/video.h"

extern uintptr_t endkernel;
extern uintptr_t startkernel;

extern void kmain(multiboot_info_t *multiboot_info, uint32_t multiboot_magic)
{
  video_clear();

  klog(LOG_DEBUG, "Kernel starts at: 0x%x\n", &startkernel);
  klog(LOG_DEBUG, "Kernel ends at  : 0x%x\n", &endkernel);

  if (multiboot_magic != MULTIBOOT_EAX_MAGIC) {
    klog(LOG_ERROR, "invalid multiboot signature\n");
    return;
  }

#ifdef DEBUG
  multiboot_dump_info(multiboot_info, multiboot_magic);
#endif

  page_frame_init(&endkernel, multiboot_info);
#ifdef DEBUG
  page_frame_dump_map();
  { /* sanity check for page_alloc */
    void *page1 = NULL;
    void *page2 = NULL;

    /* sanity check for page_frame */
    page1 = page_frame_alloc();
    page_frame_free(page1);
    page2 = page_frame_alloc();
    page_frame_free(page2);
    kcheck(page1 == page2, "page_frame_alloc()");
  }
#endif

  serial_init();
  gdt_init();
  idt_init();
  isr_set_keyboard();
  isr_set_double_fault();
  isr_init_keyboard();

  paging_init();

#ifdef DEBUG
  { /* sanity check for paging */
    uintptr_t *page = page_frame_alloc();
    uintptr_t *addr1 = (void *)0x00400000;
    uintptr_t *addr2 = addr1 + 1024;

    paging_map_page(page, addr1, 0x02);
    paging_map_page(page, addr2, 0x02);

    *addr1 = 0xcafebabe;
    *addr2 = 0xdeadbeef;
    kcheck(*addr1 == *addr2, "paging enabled");
  }
#endif

  klog(LOG_INFO, "\nInitialization complete.\n");
  kprintf("\n$ ");

  while(1) {
    ; /* nop */
}
}
