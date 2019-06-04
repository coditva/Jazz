#include <arch/x86/include/gdt.h>
#include <logger.h>
#include <multiboot.h>
#include <types.h>

#include <arch/x86/include/interrupt.h>
#include <arch/x86/include/serial.h>
#include <arch/x86/include/video.h>

extern void kmain(multiboot_info_t *, uint32_t);

void lmain(multiboot_info_t *multiboot_info, uint32_t multiboot_magic)
{
  if (multiboot_magic != MULTIBOOT_EAX_MAGIC) {
    klog(LOG_ERROR, "invalid multiboot signature\n");
    return;
  }

#ifdef DEBUG
  multiboot_dump_info(multiboot_info, multiboot_magic);
#endif

  video_clear();
  gdt_init();
  serial_init();
  idt_init();
  isr_set_keyboard();
  isr_set_double_fault();
  isr_init_keyboard();

  kmain(multiboot_info, multiboot_magic);
}
