#include <multiboot.h>
#include <logger.h>


static inline void multiboot_dump_mmap(multiboot_info_t *info)
{
#ifdef DEBUG
  if (info->flags & MULTIBOOT_FLAG_MEM_MAP) {
    klog(LOG_DEBUG, "mem map len     : 0x%x\n", info->mmap_len);
    klog(LOG_DEBUG, "mem map addr    : 0x%x\n", info->mmap_addr);

    int i = 0;
    klog(LOG_DEBUG, LOG_HRULE);
    klog(LOG_DEBUG, "   | size       | base                | length              | type\n");
    klog(LOG_DEBUG, LOG_HRULE);
    FOREACH_MEMORY_MAP(mmap, info) {
      klog(LOG_DEBUG, " %d | 0x%x | 0x%x %x | 0x%x %x | %d\n",
          i, mmap->size, mmap->base_addr_high, mmap->base_addr_low,
          mmap->len_high, mmap->len_low, mmap->type);
      i++;
    }
    klog(LOG_DEBUG, LOG_HRULE);
  } else {
    klog(LOG_WARN, "memory map not set\n");
  }
#endif
}

inline void multiboot_dump_info(multiboot_info_t *info, uint32_t multiboot_magic)
{
  if (multiboot_magic != MULTIBOOT_EAX_MAGIC) {
    klog(LOG_ERROR, "invalid multiboot signature\n");
    return;
  }

#ifdef DEBUG
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "MULTIBOOT DUMP START\n");
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "multiboot header: 0x%x\n", info);
  klog(LOG_DEBUG, "flags           : 0x%x\n", info->flags);
  klog(LOG_DEBUG, "mem lower       : 0x%x\n", info->mem_lower);
  klog(LOG_DEBUG, "mem upper       : 0x%x\n", info->mem_upper);
  klog(LOG_DEBUG, "boot device     : 0x%x\n", info->boot_device);
  klog(LOG_DEBUG, "cmdline         : %s\n",   info->cmdline);
  klog(LOG_DEBUG, "module count    : %d\n",   info->mods_count);
  klog(LOG_DEBUG, "module address  : 0x%x\n", info->mods_addr);
  multiboot_dump_mmap(info);
  klog(LOG_DEBUG, "config table    : 0x%x\n", info->config_table);
  klog(LOG_DEBUG, "bootloader name : %s\n",   info->bootloader_name);
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "MULTIBOOT DUMP END\n");
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "\n");
#endif
}
