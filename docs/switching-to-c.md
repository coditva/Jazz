# Switching to C

[This commit](https://github.com/coditva/Jazz/commit/7eb743b28d8a9797020375b171733160f49d3c54) adds our first kernel written in C.


## The entry point

You might want to write your whole kernel in C but even then you can't really avoid writing a little bit of assembly. Our entry point is [this file](https://github.com/coditva/Jazz/blob/7eb743b28d8a9797020375b171733160f49d3c54/kernel/kernel.asm). The first thing to do is declare your symbols (`start` and `kmain`) in our case. Secondly, initialize the stack pointer `esp` to some reserved stack space. Next, call the main function in the kernel `kmain`.

It is important to note that all this is done in the `.text` section. The reserved space for stack is allocated in `.bss` (Block Started by Symbol) section.

We also add 'multiboot' info to this file for the bootloaders like `grub` as well as our CPU emulator `qemu`.


## The kernel in C

Our first basic kernel simply does what [our bootloader](boot-loader) did before we wrote a kernel - it prints a message on the screen using the VGA text mode. The [kernel.c](https://github.com/coditva/Jazz/blob/7eb743b28d8a9797020375b171733160f49d3c54/kernel/kernel.c) file is self-sufficient if someone wants to know what it does.


## The linker

Since we'll be calling the `kmain` function in `kernel.c` from `kernel.asm`, we need to link the binaries. For this we will use the linker program `ld`. We configure `ld` using a simple [linker script](https://github.com/coditva/Jazz/blob/7eb743b28d8a9797020375b171733160f49d3c54/kernel/linker.ld).


## See it in action

```bash
nasm -f elf32 kernal.asm -o kernel_asm.o
gcc -m32 -c kernel.c -o kernel_c.o
ld -m elf_i386 -T linker.ld kernel_asm.o kernel_c.o -o kernel.bin

# or use the makefile
make
```

To run, use:

```bash
qemu-system-x86_64 -kernel kernel.bin

# or use the makefile
make qemu_kernel
```
