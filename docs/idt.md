# Interrupt Descriptor Table

[This commit](https://github.com/UtkarshMe/Jazz/commit/df53ccbab6157607aff2b18cf7d7566d36a592f6) adds our interrupt descriptor table.


## What
Interrupt Descriptor Table or IDT is a vector table with addresses of handlers for various interrupts. There are 256 interrupts on x86 architecture. The first 32 (0x1f) interrupts are left for Intel to handle and the rest are meant for the developer to take advantage of.


## How
Whenever a interrupt has to be generated (e.g. pressing of keyboard key), that information is sent to the device called a Programmable Interrupt Controller (PIC). The PIC is responsible for raising the specific interrupt from the type of Interrupt Request (IRQ).

There are generally 2 PICs: master and slave, cascaded. See [OSdev wiki](https://wiki.osdev.org/Interrupts) for detailed explanation.


## See it in action

```bash
cd kernel
make qemu
```
You will not see any difference because nothing happens :P
