#ifndef MULTIBOOT_H_RKNBOMGQ
#define MULTIBOOT_H_RKNBOMGQ

#include <types.h>

/* magic numbers for multiboot. these help us verify that multiboot structure is
 * indeed valid. */
#define MULTIBOOT_MAGIC                 0x1BADB002
#define MULTIBOOT_EAX_MAGIC             0x2BADB002

#define MULTIBOOT_MOD_ALIGN             0x00001000
#define MULTIBOOT_INFO_ALIGN            0x00000004

/* flags for multiboot_info */
#define MULTIBOOT_FLAG_MEMORY           0x00000001
#define MULTIBOOT_FLAG_BOOTDEV          0x00000002
#define MULTIBOOT_FLAG_CMDLINE          0x00000004
#define MULTIBOOT_FLAG_MODS             0x00000008
#define MULTIBOOT_FLAG_AOUT_SYMS        0x00000010
#define MULTIBOOT_FLAG_ELF_SHDR         0X00000020
#define MULTIBOOT_FLAG_MEM_MAP          0x00000040
#define MULTIBOOT_FLAG_DRIVE_INFO       0x00000080
#define MULTIBOOT_FLAG_CONFIG_TABLE     0x00000100
#define MULTIBOOT_FLAG_BOOT_LOADER_NAME 0x00000200
#define MULTIBOOT_FLAG_APM_TABLE        0x00000400
#define MULTIBOOT_FLAG_VBE_INFO         0x00000800
#define MULTIBOOT_FLAG_FRAMEBUFFER_INFO 0x00001000

/* flags for multiboot framebuffer type */
#define MULTIBOOT_FB_TYPE_INDEXED       0x00000000
#define MULTIBOOT_FB_TYPE_RGB           0x00000001
#define MULTIBOOT_FB_TYPE_EGA_TEXT      0x00000002


typedef struct _multiboot_info_t {
  uint32_t flags;

  uint32_t mem_lower;
  uint32_t mem_upper;

  uint32_t boot_device;

  uint32_t cmdline;

  uint32_t mods_count;
  uint32_t mods_addr;

  union {
    struct {
      uint32_t num;
      uint32_t size;
      uint32_t addr;
      uint32_t shndx;
    } __attribute__((packed)) efl_sec;

    struct {
      uint32_t tabsize;
      uint32_t strsize;
      uint32_t addr;
      uint32_t reserved;
    } __attribute__((packed)) aout_sym;
  };

  uint32_t mmap_len;
  uint32_t mmap_addr;

  uint32_t drives_len;
  uint32_t driver_addr;

  uint32_t config_table;

  uint32_t bootloader_name;

  uint32_t apm_table;

  struct {
    uint32_t control_info;
    uint32_t mode_info;
    uint16_t mode;
    uint16_t interface_seg;
    uint16_t interface_off;
    uint16_t interface_len;
  } __attribute__((packed)) vbe;

  struct {
    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t  bpp;
    uint8_t  type;

    union {
      struct {
        uint32_t addr;
        uint16_t num_colors;
      } __attribute__((packed)) palette;

      struct {
        uint8_t red_field_position;
        uint8_t red_mask_size;
        uint8_t green_field_position;
        uint8_t green_mask_size;
        uint8_t blue_field_position;
        uint8_t blue_mask_size;
      } __attribute__((packed));
    };
  } __attribute__((packed)) framebuffer;
} __attribute__((packed)) multiboot_info_t;


typedef struct _multiboot_memory_map_t {
  uint32_t size;
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t len_low;
  uint32_t len_high;
#define MULTIBOOT_MEM_TYPE_FREE     0x1     /* free memory */
#define MULTIBOOT_MEM_TYPE_RSVD     0x2     /* reserved by system */
#define MULTIBOOT_MEM_TYPE_ACPI     0x3     /* reclaimable ACPI memory */
#define MULTIBOOT_MEM_TYPE_NVS      0x4     /* non-volatile storage */
#define MULTIBOOT_MEM_TYPE_BADR     0x5     /* memory used by bad RAM modules */
  uint32_t type;
} __attribute__((packed)) multiboot_memory_map_t;


#ifdef DEBUG
void multiboot_dump_info    (multiboot_info_t *info, uint32_t multiboot_magic);
#endif

#define FOREACH_MEMORY_MAP(MMAP, INFO) \
  for(multiboot_memory_map_t *(MMAP) = \
        (multiboot_memory_map_t *)(uintptr_t)((INFO)->mmap_addr); \
      (uintptr_t)(MMAP) < (INFO)->mmap_addr + (INFO)->mmap_len; \
      (MMAP) = (multiboot_memory_map_t *) \
        ((uintptr_t)(MMAP) + (MMAP)->size + sizeof((MMAP)->size)) \
      )

#endif /* end of include guard: MULTIBOOT_H_RKNBOMGQ */
