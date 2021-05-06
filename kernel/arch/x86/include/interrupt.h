#ifndef INTERRUPT_H_Y4EC6WHZ
#define INTERRUPT_H_Y4EC6WHZ

#include <types.h>

#define PIC1         0x20 /* IO base address for master PIC */
#define PIC2         0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

#define ICW1_ICW4      0x01 /* ICW4 (not) needed */
#define ICW1_SINGLE    0x02 /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL     0x08 /* Level triggered (edge) mode */
#define ICW1_INIT      0x10 /* Initialization - required! */

#define ICW4_8086       0x01 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM       0x10 /* Special fully nested (not) */

#define KEYBOARD_INTERRUPT_GATE    0x08
#define KERNEL_CODE_SEGMENT_OFFSET 0x8e

/**
 * The structure of the IDT
 *
 * The 32 bit offset for each entry is divided into two (offset_1 & offset_2).
 * The selector must point to a valid entry in the GDT or LDT.
 * The type_attr is:
 * +---+---+---+---+---+---+---+---+
 * | P |  DPL  | S |    GateType   |
 * +---+---+---+---+---+---+---+---+
 *  - P   : (present) 0 for unused interrupts.
 *  - DPL : (descriptor priviledge level) the minimum privilege level for the
 *          calling descriptor.
 *  - S   : (storage segment) set to 0 for interrupt/trap gates.
 *  - Type: one of the 5 possible gate types.
 */
typedef struct _idt_t
{
  uint16_t offset_1;  /* lower offset 15-0 */
  uint16_t selector;  /* code segment selector */
  uint8_t  zero;      /* always zero */
  uint8_t  type_attr; /* types and attributes */
  uint16_t offset_2;  /* upper offset 31-16 */
} __attribute__((packed)) idt_t;

/**
 * The structure of the pointer to idt which is called by lidt instruction to
 * load the idt.
 */
typedef struct _idt_ptr_t
{
  uint16_t limit; /* size of the idt - 1 */
  uint32_t base;  /* start address of the idt */
} __attribute__((packed)) idt_ptr_t;

#define IDT_SIZE 256

void idt_init(void);
void idt_set_gate(int offset, uint32_t base, uint16_t selector,
                  uint8_t type_attr);

void isr_init_keyboard(void);
void isr_set_keyboard(void);
void isr_set_double_fault(void);

#endif /* end of include guard: INTERRUPT_H_Y4EC6WHZ */
