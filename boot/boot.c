#include <gdt.h>
#include <multiboot.h>
#include <string.h>
#include <elf.h>

void setup_longmode()
{
  // TODO(coditva):
}

void jump_to_kernel(void *kernel_entry, multiboot_info_t *multiboot_info)
{
  // TODO(coditva):
  (void)(kernel_entry);
  (void)(multiboot_info);
}

void lmain(multiboot_info_t *multiboot_info, uint32_t multiboot_magic)
{
  if (multiboot_magic != MULTIBOOT_EAX_MAGIC) {
    return;
  }

  gdt_init();

  void *kernel_entry = 0x0;
  char *kernel_mod_name = "kernel";

  if (multiboot_info->flags & MULTIBOOT_FLAG_MODS) {
    for (uint32_t i = 0; i < multiboot_info->mods_count; i++) {

      multiboot_mod_t *module = (multiboot_mod_t *)
          (multiboot_info->mods_addr + (i * sizeof(multiboot_mod_t)));

      if (strcmp((char *)&module->string, kernel_mod_name) == 0) {
        kernel_entry = elf_load((void *)(uintptr_t)module->mod_start,
            (void *)(uintptr_t)module->mod_end);
        (void)(kernel_entry);

      }
    }
  }

  setup_longmode();
  jump_to_kernel(kernel_entry, multiboot_info);

  while (1) {
    ; /* nop */
  }
}
