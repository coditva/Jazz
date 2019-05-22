# Booting with Multiboot specification compliant bootloader

## What
Multiboot specification is an interface between a bootloader and an OS in which
the bootloader collects certain information like memory size, architecture info,
etc. and informs the OS about it. Because of a fixed interface, any multiboot
compliant OS can be booted using any multiboot compliant bootloader.

## How
- The bootloader puts the boot info into the agreed upon layout.
- The bootloader sets the magic multiboot header.
- The OS verifies the header and reads the data and puts it to use.

## References
- [The Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
- [kernel/include/multiboot.h](https://github.com/coditva/Jazz/blob/master/kernel/include/multiboot.h)
