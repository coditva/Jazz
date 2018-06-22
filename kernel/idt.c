#include "idt.h"

extern void idt_load();

idt_t idt[IDT_SIZE];

void idt_set_gate(int offset, uint32_t base, uint16_t selector,
    uint8_t type_attr)
{
  idt[offset].offset_1  = base & 0xffff;
  idt[offset].offset_2  = (base >> 16) & 0xffff;
  idt[offset].selector  = selector;
  idt[offset].type_attr = type_attr;
  idt[offset].zero      = 0;
}

void idt_init(void)
{
  idt_ptr_t idt_pointer = {
    .limit = sizeof(idt_t) * IDT_SIZE - 1,
    .base  = (uint32_t)&idt
  };

  idt_load(idt_pointer);
}
