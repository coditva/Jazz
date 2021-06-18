; ---------------------------------------------------------------------
; The bootloader for Jazz (github.com/coditva/Jazz)
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


; ---------------------------------------------------------------------
; now we will read the disk for our kernel code
; it is recommended that the disk read must be tried atleast thrice

load_disk:

  mov ah, 0x02              ; the BIOS routine for reading disk sectors
  mov al, 0x01              ; the number of sectors to read
  mov ch, 0x0               ; track/cylinder number
  mov cl, 0x3               ; sector number
  mov dh, 0x0               ; head number
  mov dl, 0x1               ; drive number

  int 13h                   ; on return, ah = status, al = number of sectors read,
  jc load_disk_error        ; carry = 0 if success, 1 if fail
  jmp load_disk_done

  ;cmp al, 0x01
  ;jne load_disk_error

load_disk_error:
  jmp load_disk

load_disk_done:
  cli
  hlt



  mov ax, 0x1000
  mov es, ax
  mov bx, 0x0

  jmp 0x1000:0x0
  hlt

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
; mark this as bootable

  times 510-($-$$) db 0     ; fill the remaining memory with 0
  dw 0xaa55                 ; add the magic number
