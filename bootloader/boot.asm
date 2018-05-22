; The bootloader for Jazz (github.com/utkarshme/Jazz)
; Author: Utkarsh Maheshwari
; vi:filetype=nasm

jmp $                     ; keep jumping to the current instruction
times 510-($-$$) db 0     ; fill the remaining memory with 0
dw 0xaa55                 ; add the magic number
