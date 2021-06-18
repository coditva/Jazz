ENTRY(start)
SECTIONS
{
  . = 0x00000000;

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
  }
}
