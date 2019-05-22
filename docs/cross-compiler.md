# Setting up a cross compiler
We use a cross compiler to compiler for a specific architecture that we're
targeting. We also need it to compiler binaries that run on that OS.

## What
We use the gcc compiler and linker for our purpose. For this, we download the
desired version for the compiler and compile it for the architecture we're
building our OS for.

## How
- Download the sources for GCC compiler and binutils to `.deps`
- Extract and build binutils.
- Use the new binutils to build the compiler and linker.
- Use the newly built compiler for compiler the OS.

## References
- [https://wiki.osdev.org/GCC_Cross-Compiler](https://wiki.osdev.org/GCC_Cross-Compiler)
- [Makefile for building the cross compiler](https://github.com/coditva/Jazz/blob/master/.deps/makefile)
