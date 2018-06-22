#ifndef IDT_H_7TALJSEW
#define IDT_H_7TALJSEW

#include <inttypes.h>

/**
 * The structure of the IDT
 *
 * The 64 bit offset for each entry is divided into two (offset_1 & offset_2).
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
typedef struct _idt_t {
  uint16_t offset_1;    /* lower offset 15-0 */
  uint16_t selector;    /* code segment selector */
  uint8_t  zero;        /* always zero */
  uint8_t  type_attr;   /* types and attributes */
  uint16_t offset_2;    /* upper offset 31-16 */
} idt_t;


/**
 * Structure of the idt pointer used by lidt.
 */
typedef struct _idt_ptr_t {
  uint16_t limit;       /* total size of the idt - 1 */
  uint32_t base;        /* first entry in the idt */
} idt_ptr_t;

#define IDT_SIZE 256

#endif /* end of include guard: IDT_H_7TALJSEW */
