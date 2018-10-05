#include <multiboot.h>
#include <logger.h>

void multiboot_dump_info(multiboot_info_t *info)
{
  klog(LOG_INFO, "------------ MULTIBOOT DUMP ------------\n");
  klog(LOG_INFO, "Multiboot header: 0x%x\n", info);
  klog(LOG_INFO, "flags           : 0x%x\n", info -> flags);
  klog(LOG_INFO, "mem lower       : 0x%x\n", info -> mem_lower);
  klog(LOG_INFO, "mem upper       : 0x%x\n", info -> mem_upper);
  klog(LOG_INFO, "boot device     : 0x%x\n", info -> boot_device);
  klog(LOG_INFO, "cmdline         : 0x%x\n", info -> cmdline);
  klog(LOG_INFO, "module count    : 0x%x\n", info -> mods_count);
  klog(LOG_INFO, "module address  : 0x%x\n", info -> mods_addr);
  klog(LOG_INFO, "mem map len     : 0x%x\n", info -> mmap_len);
  klog(LOG_INFO, "mem map addr    : 0x%x\n", info -> mmap_addr);
  klog(LOG_INFO, "config table    : 0x%x\n", info -> config_table);
  klog(LOG_INFO, "bootloader name : 0x%x\n", info -> bootloader_name);
  klog(LOG_INFO, "----------------------------------------\n");
}
