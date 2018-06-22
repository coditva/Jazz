; Port IO - read/write to a port
; vi:filetype=nasm

global read_port
global write_port


read_port:
  mov edx, [esp + 4]
  in al, dx
  ret

write_port:
  mov edx, [esp + 4]
  mov al, [esp + 8]
  out dx, al
  ret
