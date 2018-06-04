# Writing a Bootloader in 32bit Mode

## What is a bootloader

Once the BIOS is loaded, it looks around for a bootable device by looking at the [Boot Sector](boot-sector). If it finds one, it starts executing the program on it. That program can be a very small program of just a few lines of assembly code to a very powerful program like [GRUB](https://wiki.osdev.org/GRUB). This program is responsible to perform various checks and load the operating system.


## Our bootloader

Our [**first** bootloader](https://github.com/UtkarshMe/Jazz/blob/df24113c45b520ae559ba338f3cdba2a3b655e3d/boot_hello.asm) was a very tiny program which printed "Hello" on the screen and looped.

Our **second**, improved bootloader will enable 32-bit protected mode of the Intel 86 processor and print something using the VGA text. The 32-bit protected mode addresses memory by using [Descriptor Tables](https://en.wikipedia.org/wiki/Global_Descriptor_Table). [This commit](https://github.com/UtkarshMe/Jazz/tree/725541095db3a620b3cfeae4819ae801e19bd9a1) adds a file [`bootloader/boot.asm`](https://github.com/UtkarshMe/Jazz/blob/725541095db3a620b3cfeae4819ae801e19bd9a1/bootloader/boot.asm).

The contents of the bootloader are loaded on the address `0x7c00` in the RAM. Hence we use `org 0x7c00` in out bootloader. Next we enable the `A20` bit i.e. the 21st address line for the _intel x86_ processor and set the VGA text mode. All _x86_ processors are started in **16-bit real mode**. To use **32-bit protected mode**, we must set the lowest bit in the special CPU register `cr0`. We set up our _Global Descriptor Table_, initialize segment registers and print some text in VGA mode while in protected mode.

Our **third** bootloader load the kernel. It is a convention of sorts to use physical address `0x100000` as the starting address for kernels and thus our bootloader will `jmp` to it after switching to 32-bit protected mode. The bootloader can be found [here]().


## See it in action
To make things easier, we have automated the build process using GNU Make:
```bash
make        # creates the build/bootloader/boot.bin and build/disk.img
make qemu   # run the qemu emulator
```
