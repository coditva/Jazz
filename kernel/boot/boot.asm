; ---------------------------------------------------------------------
; The kernel for Jazz (github.com/coditva/Jazz)
; Author: Utkarsh Maheshwari
; vi:filetype=nasm
; ---------------------------------------------------------------------

bits 32
align 4

; ---------------------------------------------------------------------
; add multiboot compliance info

section .multiboot
  dd 0x1BADB002             ; magic number
  dd 0x00                   ; flags
  dd - (0x1BADB002 + 0x00)  ; checksum =>
                            ; magic + flags + checksum = zero


section .text               ; start code section

global start                ; declare start symbol as global so that it
start:                      ; is visible to the linker

  cli                       ; prevent interrupts from waking halted CPU
  mov esp, _stack           ; initialize stack

  push eax                  ; multiboot magic number
  push ebx                  ; multiboot info

extern lmain
  call lmain

  hlt                       ; halt CPU


; ---------------------------------------------------------------------

section .bss

  resb 8192                 ; reserve space for stack (8kb)
  _stack:
