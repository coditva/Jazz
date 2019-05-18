ENTRY(start)
SECTIONS
{
  . = 0x00100000;

  .multiboot : {
    *(.multiboot)
  }

  .text : {
    *(.text)
  }

  .data : {
    *(.data)
    . = ALIGN(4096);
  }

  .bss  : {
    *(.bss)
    endkernel = .;
  }
}
