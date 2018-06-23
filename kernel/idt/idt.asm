; vi:filetype=nasm

global idt_load
idt_load:
  mov edx, [esp + 4]        ; load the param passed to "idt_load"
  lidt [edx]                ; load the idt
  sti                       ; set interrupts
  ret                       ; return
