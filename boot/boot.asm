; vi:filetype=nasm

section .text               ; start code section
align 4

section .multiboot
  dd 0x1BADB002             ; magic number
  dd 0x00                   ; flags
  dd - (0x1BADB002 + 0x00)  ; checksum =>
                            ; magic + flags + checksum = zero

extern lmain

global start
start:
  cli                       ; prevent interrupts from waking halted CPU
  mov esp, _stack           ; initialize stack
  push eax                  ; multiboot magic number
  push ebx                  ; multiboot info
  call lmain                ; call the lmain function in boot.c
  hlt                       ; halt CPU

section .bss

  resb 8192                 ; reserve space for stack (8kb)
  _stack:
