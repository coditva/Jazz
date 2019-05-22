# Global Descriptor Table
[This commit](https://github.com/coditva/Jazz/tree/db24376) adds a GDT. It is used to setup segmentation in memory.

## What
A GDT is a table of entries which informs the CPU of the memory segments that the OS implements and their locations.

## How
- Allocate some memory for the GDT structure.
- Set the first GDT entry as a null entry.
- Set entries for kernel data and code segments and
user data and code segments.
- Load the table in the CPU with `lgdt` assembly command.
- After loading the gdt, the segments have to be reloaded by jumping to new code segment and reloading the data segments.

## References
- [1] [OSDev on Global Descriptor Table](https://wiki.osdev.org/Global_Descriptor_Table)
- [2] [OSDev GDT Tutorial](https://wiki.osdev.org/GDT_Tutorial)
