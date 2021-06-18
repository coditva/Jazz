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
  cli                       ; clear interrupts flag (block interrupts)
  hlt                       ; hlt

hello:
  db "I love OSDev. My bootloader is awesome!", 0


