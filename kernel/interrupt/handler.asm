; vi:filetype=nasm

global keyboard_handler_int
keyboard_handler_int:
  extern keyboard_handler
  call keyboard_handler
  iretd
