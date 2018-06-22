
CC            = gcc
CC_ASM				= nasm
BUILD_DIR     = ../build/kernel
TARGET        = $(BUILD_DIR)/kernel.bin
OBJS  				= kernel.o idt.o
ASM_OBJS			= kernel_asm.o util/portio_asm.o
INCLUDE       = idt.h util/util.h

all:          $(TARGET) $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET):    $(OBJS) $(INCLUDE) $(ASM_OBJS)
	ld -m elf_i386 -T linker.ld $(OBJS) $(ASM_OBJS) -o $@

$(ASM_OBJS):%_asm.o: %.asm
	$(CC_ASM) -f elf32 $? -o $@

$(OBJS):%.o:   %.c
	$(CC) -m32 -I. -c $? -o $@

qemu: all
	qemu-system-x86_64 -kernel $(BUILD_DIR)/kernel.bin

clean:
	rm -rf $(TARGET) $(OBJS) $(ASM_OBJS)
