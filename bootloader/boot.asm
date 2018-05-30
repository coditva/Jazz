; ---------------------------------------------------------------------
; The bootloader for Jazz (github.com/utkarshme/Jazz)
; Author: Utkarsh Maheshwari
; vi:filetype=nasm
; ---------------------------------------------------------------------


bits  16
org   0x7c00

boot:

  mov ax, 0x2401
  int 0x15                  ; enable A20 bit

  mov ax, 0x3               ; set text-mode to 3
  int 0x10                  ; set VGA text-mode


; ---------------------------------------------------------------------
; we will enter 32bit protected mode now
; but first, gotta setup a Global Descriptor Table (GDT)

  lgdt [gdt_pointer]        ; load the gdt table
  mov  eax, cr0             ; set protected mode
  or   eax, 0x1             ; on special
  mov  cr0, eax             ; CPU reg cr0

  jmp CODE_SEG:boot2        ; long jump to the code segment


; ---------------------------------------------------------------------
; define the data structure for a GDT

gdt_start:
  dq 0x0
gdt_code:
  dw 0xFFFF
  dw 0x0
  db 0x0
  db 10011010b
  db 11001111b
  db 0x0
gdt_data:
  dw 0xFFFF
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0
gdt_end:


; ---------------------------------------------------------------------
; setup the data structure for the gdt_pointer

gdt_pointer:
  dw gdt_end - gdt_start
  dd gdt_start


; ---------------------------------------------------------------------
; define segments

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


; ---------------------------------------------------------------------
; setup complete, let's get into 32bit!

bits 32

boot2:
  mov ax, DATA_SEG          ; setup segment registers
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax


; ---------------------------------------------------------------------
; let's print something using VGA text

  mov ebx, 0xb8000          ; the VGA buffer is located at 0xb8000
                            ; we will be sending all text to this

  mov esi, hello            ; load the string to be printed

.loop:
  lodsb                     ; load a byte
  add al, 0x0               ; check if it is zero
  jz halt                   ; halt if it is
  or eax, 0x0200            ; add formatting info
  mov word [ebx], ax        ; send to VGA
  add ebx, 2                ; increment address
  jmp .loop

halt:
  cli                       ; clear interrupts flag
  hlt                       ; hlt

hello:
  db "I love OSDev. My bootloader is awesome!", 0


; ---------------------------------------------------------------------
; mark this as bootable

  times 510-($-$$) db 0     ; fill the remaining memory with 0
  dw 0xaa55                 ; add the magic number
