; ---------------------------------------------------------------------
; The kernel for Jazz (github.com/utkarshme/Jazz)
; Author: Utkarsh Maheshwari
; vi:filetype=nasm
; ---------------------------------------------------------------------

bits 32
section .text               ; start code section

extern kernel_main          ; the main kernel function
global begin                ; declare start symbol as global so that it
                            ; is visible to the linker

begin:

  cli                       ; prevent interrupts from waking halted CPU
  mov esp, _stack           ; initialize stack
  call kernel_main          ; call the kernel function in C
  hlt                       ; halt CPU


; ---------------------------------------------------------------------

section .bss

  resb 8192                 ; reserve space for stack (8kb)
  _stack:
