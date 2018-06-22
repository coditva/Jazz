
CC            = gcc
CC_ASM				= nasm
BUILD_DIR     = ../build/kernel
TARGET        = $(BUILD_DIR)/kernel.bin
OBJS  				= kernel.o idt.o
INCLUDE       = idt.h

all:          $(TARGET) $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET):    $(OBJS) $(INCLUDE) kernel_asm.o
	ld -m elf_i386 -T linker.ld $(OBJS) kernel_asm.o -o $@

kernel_asm.o: kernel.asm
	$(CC_ASM) -f elf32 $? -o $@

$(OBJS):%.o:   %.c
	$(CC) -m32 -c $? -o $@

qemu: all
	qemu-system-x86_64 -kernel $(BUILD_DIR)/kernel.bin

clean:
	rm -rf $(TARGET) *.o
