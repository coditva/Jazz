#include <logger.h>
#include <types.h>
#include "interrupt/interrupt.h"
#include "io/portio/portio.h"

extern void idt_load(idt_ptr_t *);
extern void keyboard_handler_int(void);
extern void double_fault_handler_int(void);

void isr_set_keyboard(void)
{
  uint32_t keyboard_address = (uint32_t)&keyboard_handler_int;
  idt_set_gate(0x21, keyboard_address, 0x08, 0x8e);
}

void isr_set_double_fault(void)
{
  idt_set_gate(0x08, (uint32_t)&double_fault_handler_int, 0x08, 0x8f);
}

void isr_init_keyboard(void)
{
  klog_status_init(LOG_DEBUG, "keyboard");
  /* TODO: This will disable all other interrupts; prevent that */
  write_port(0x21, 0xfd);
  klog_status_ok(LOG_DEBUG);
}

idt_t idt[IDT_SIZE];

void idt_set_gate(int offset, uint32_t base, uint16_t selector,
    uint8_t type_attr)
{
  idt[offset].offset_1  = base & 0xffff;
  idt[offset].offset_2  = (base & 0xffff0000) >> 16;
  idt[offset].selector  = selector;
  idt[offset].type_attr = type_attr;
  idt[offset].zero      = 0;
}

void idt_init(void)
{
  klog_status_init(LOG_DEBUG, "IDT");

  uint32_t idt_address = (uint32_t)&idt;
  idt_ptr_t idt_pointer = {
    .limit = (sizeof(idt_t) * IDT_SIZE) - 1,
    .base = idt_address
  };

  /* begin initializing ICW in cascade mode */
  write_port(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);
  write_port(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);

  /* ICW2: set offsets for master and slave PICs. First 32 (0x1f) interrupts are
   * reserved by Intel, so start from 0x20. Each PIC has 0-7 interrupts. */
  write_port(PIC1_DATA, 0x20);
  write_port(PIC2_DATA, 0x28);

  /* ICW3: setup cascading */
  write_port(PIC1_DATA, 0x04);
  write_port(PIC2_DATA, 0x02);

  /* ICW4: addition info about environment */
  write_port(PIC1_DATA, ICW4_8086);
  write_port(PIC2_DATA, ICW4_8086);


  /* mask interrupts */
  write_port(PIC1_DATA, 0xff);
  write_port(PIC2_DATA, 0xff);

  idt_load(&idt_pointer);

  klog_status_ok(LOG_DEBUG);
}
