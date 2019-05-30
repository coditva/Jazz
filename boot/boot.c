#include <multiboot.h>

void lmain(multiboot_info_t *multiboot_info, uint32_t multiboot_magic)
{
  if (multiboot_magic != MULTIBOOT_EAX_MAGIC) {
    return;
  }

  (void)(multiboot_info);

  while (1) {
    ; /* nop */
  }
}
