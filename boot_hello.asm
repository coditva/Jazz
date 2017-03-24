; Use bios interrupts to display 'Hello' on screen
mov ah, 0x0e

mov al, 'H'
int 0x10
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
mov al, 'l'
int 0x10
mov al, 'o'
int 0x10

; keep jumping to the current line.
jmp $

; Add the magic number to the last two bytes.
times 510-($-$$) db 0
dw 0xaa55
