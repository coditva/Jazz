; vi:filetype=nasm

global page_directory_load
page_directory_load:
  mov eax, [esp + 8]
  mov cr3, eax
  ret

global paging_enable
paging_enable:
  mov eax, cr0
  or eax, 0x8000001
  mov cr0, eax
  ret
