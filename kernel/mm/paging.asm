; vi:filetype=nasm

global page_directory_load
page_directory_load:
  mov eax, [esp + 4]
  mov cr3, eax
  ret

global paging_enable
paging_enable:
  mov eax, cr0
  or eax, 0x80000000
_paging_enabling:
  mov cr0, eax
  ret
