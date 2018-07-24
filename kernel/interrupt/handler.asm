; vi:filetype=nasm

global keyboard_handler_int
keyboard_handler_int:
  pushad
  cld                       ; clear the direction flag on function entry
  extern keyboard_handler
  call keyboard_handler
  popad
  iret

global double_fault_handler_int
double_fault_handler_int:
  pushad
  cld                       ; clear the direction flag on function entry
  extern double_fault_handler
  call double_fault_handler
  popad
  iret
