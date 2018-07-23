; ---------------------------------------------------------------------
; The kernel for Jazz (github.com/utkarshme/Jazz)
; Author: Utkarsh Maheshwari
; vi:filetype=nasm
; ---------------------------------------------------------------------

bits 32
section .text               ; start code section
align 4

; ---------------------------------------------------------------------
; add multiboot compliance info

  dd 0x1BADB002             ; magic number
  dd 0x00                   ; flags
  dd - (0x1BADB002 + 0x00)  ; checksum =>
                            ; magic + flags + checksum = zero


%include "interrupt/interrupt.asm"
%include "interrupt/handler.asm"
%include "memory/gdt.asm"


extern kmain                ; it is convention to name main kernel
                            ; function kmain

global start                ; declare start symbol as global so that it
                            ; is visible to the linker

start:
  cli                       ; prevent interrupts from waking halted CPU
  mov esp, _stack           ; initialize stack
  call kmain                ; call the kernel function in C
  hlt                       ; halt CPU


; ---------------------------------------------------------------------

section .bss

  resb 8192                 ; reserve space for stack (8kb)
  _stack:
