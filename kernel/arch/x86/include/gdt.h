#ifndef GDT_H_9RIWT3SN
#define GDT_H_9RIWT3SN

#include <types.h>

/**
 * The structure of the entries in the GDT.
 */
typedef struct _gdt_entry_t
{
  uint16_t limit_0;
  uint16_t base_0;
  uint8_t  base_1;
  uint8_t  access_byte;
  uint8_t  flags_limit_1;
  uint8_t  base_2;
} __attribute__((packed)) gdt_entry_t;

#define GDT_SIZE 3

typedef struct _gdt_ptr_t
{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void gdt_init(void);
void reload_segments(void);

/* Using flat setup. TODO: Separate code and data segments(?) */
#define CODE_SEGMENT_OFFSET 0x0
#define CODE_SEGMENT_SIZE   0xffffffff
#define DATA_SEGMENT_OFFSET 0x0
#define DATA_SEGMENT_SIZE   0xffffffff

#endif /* end of include guard: GDT_H_9RIWT3SN */
