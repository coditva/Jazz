; vi:filetype=nasm

gdtr  dw 0
      dd 0

; TODO: Think about moving to "higher half kernel"

global gdt_load
gdt_load:
  mov eax, [esp + 4]        ; load gdt address
  mov [gdtr + 2], eax
  mov ax, [esp + 8]         ; load gdt size
  mov [gdtr], ax
  lgdt [gdtr]               ; load idt
  ret

global reload_segments
reload_segments:
  jmp 0x08:complete_reload_segments     ; load 0x08 into code selector (CS)

complete_reload_segments:
  mov ax, 0x10              ; load 0x10 into data selector
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  ret
