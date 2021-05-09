#include <kio.h>
#include <logger.h>
#include <mm/memory_alloc.h>
#include <mm/page_alloc.h>
#include <mm/page_frame.h>
#include <mm/paging.h>
#include <multiboot.h>

extern const uintptr_t endkernel;
extern const uintptr_t startkernel;

extern void kmain(multiboot_info_t *multiboot_info, uint32_t multiboot_magic)
{
  klog(LOG_DEBUG, "Kernel starts at: 0x%x\n", &startkernel);
  klog(LOG_DEBUG, "Kernel ends at  : 0x%x\n", &endkernel);

  if (multiboot_verify(multiboot_info, multiboot_magic)) {
    klog(LOG_ERROR, "invalid multiboot signature\n");

    return;
  }

  page_frame_init((void *)&endkernel, multiboot_info);
  page_alloc_init();
  paging_init();

  klog(LOG_INFO, "\nInitialization complete.\n");
  kprintf("\n$ ");

  while (1) {
    ; /* nop */
  }
}
