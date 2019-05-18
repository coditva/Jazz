ENTRY(start)
SECTIONS
{
  . = 0x00100000;
  startkernel = .;

  .multiboot : {
    *(.multiboot)
  }

  .text : {
    *(.text)
  }

  .data ALIGN(0x1000) : {
    *(.data)
  }

  .bss  : {
    *(.bss)
    endkernel = .;
  }
}
