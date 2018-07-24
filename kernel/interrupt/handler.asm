; vi:filetype=nasm

global keyboard_handler_int
keyboard_handler_int:
  pushad
  cld                       ; clear the direction flag on function entry
  extern keyboard_handler
  call keyboard_handler
  popad
  iret
