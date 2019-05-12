# The Boot Sector
_And then there was a bootable OS_

## Theory
So here the theory goes... A machine which has no OS, has something call a Basic Input/Output System or BIOS. BIOS is the first thing that gets loaded when you press the power button.

After that, the computer looks for a bootable device to load an OS. But how does BIOS know which device is bootable? Here comes the concept of a boot sector. Every memory disk drive is divided into segments known as sectors. So to find if a disk is bootable, the first 512 byte sector of the disk, called the boot sector, are checked.

If the last two bytes of the boot sector are set to the number 0xaa55 (called the boot sector signature), the disk is assumed to have a boot loader which is responsible for loading the OS.


## See it in action
[This commit](https://github.com/coditva/Jazz/tree/df24113c45b520ae559ba338f3cdba2a3b655e3d) adds a file called `boot_hello.asm` which when compiled with `nasm` and placed on the boot sector of a disk makes the disk bootable. Upon booting with that disk you are shown `Hello`.

Unless you want to make a device bootable after every change you make and then reboot your PC with that device, here's something which can help:

Compile the `boot_hello.asm` file:
```bash
nasm boot_hello.asm -o boot_hello.bin
```

Use `qemu` (or any CPU emulator) to load the OS: `qemu-system-x86_64 boot_hello.bin`
