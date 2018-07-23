# Keyboard Interrupt Service Routine
[This commit]() adds a interrupt service routine (ISR) for the keyboard.

## What
An Interrupt Service Routine (ISR) is a function that is assigned to handle an interrupt. This setting of the interrupt handler is also called setting the gate (see `kernel/idt/idt.c: idt_set_gate()`). Since this routine is called directly by the CPU and must return by the assembly instruction `iret` or `iretd`, we use a "wrapper" in assembly which calls the handler in C and returns with `iretd`.


## How
The keyboard ISR is set in `kernel/isr/isr.c: isr_set_keyboard()` and enabled in `isr_init_keyboard()`.

## See it in action
```bash
cd kernel
make qemu
```
You will see a prompt like:
```
type here:
```
Start typing and see it on the screen!
_Note: The enter key does not work as of now._
