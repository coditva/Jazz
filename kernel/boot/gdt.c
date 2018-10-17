#include <logger.h>
#include "boot/gdt.h"

gdt_entry_t gdt[GDT_SIZE];
extern void gdt_load(gdt_entry_t *, uint16_t size);

void gdt_set_entry(uint32_t offset, uint32_t base, uint32_t limit,
    uint8_t access_byte)
{
  if (limit > 65536) {              /* adjust granularity */
    limit = limit >> 12;
    gdt[offset].flags_limit_1 = 0xc0;
  } else {
    gdt[offset].flags_limit_1 = 0x40;
  }

  gdt[offset].base_0 = (uint16_t)base & 0xff;
  gdt[offset].base_1 = (uint8_t)(base >> 16) & 0xff;
  gdt[offset].base_2 = (uint8_t)(base >> 24) & 0xff;

  gdt[offset].limit_0 = (uint16_t)limit & 0xff;
  gdt[offset].flags_limit_1 |= (uint16_t)(limit >> 16) & 0xf;

  gdt[offset].access_byte = access_byte;
}

void gdt_init(void)
{
  klog_status_init("GDT");

  /* first entry is NULL entry */
  gdt_set_entry(0, 0, 0, 0);

  /* TODO: Remove sepecial numbers from these statements */
  /* kernel code and data segments */
  gdt_set_entry(1, CODE_SEGMENT_OFFSET, CODE_SEGMENT_SIZE, 0x9a);
  gdt_set_entry(2, DATA_SEGMENT_OFFSET, DATA_SEGMENT_SIZE, 0x92);

  /* user code and data segments */
  gdt_set_entry(3, CODE_SEGMENT_OFFSET, CODE_SEGMENT_SIZE, 0xfa);
  gdt_set_entry(4, DATA_SEGMENT_OFFSET, DATA_SEGMENT_SIZE, 0xf2);

  /* TODO: Setup tss (what?) */
  /* TODO: Setup ldt? */

  gdt_load(gdt, GDT_SIZE * sizeof(gdt_entry_t) - 1);
  reload_segments();

  klog_status_ok("GDT");
}
