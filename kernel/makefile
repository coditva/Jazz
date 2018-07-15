
CC            = gcc
CC_ASM				= nasm
BUILD_DIR     = ../build/kernel
TARGET        = $(BUILD_DIR)/kernel.bin

all:          $(TARGET) $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET):    kernel_asm.o kernel_c.o
	ld -m elf_i386 -T linker.ld kernel_asm.o kernel_c.o -o $@

kernel_c.o:   kernel.c
	$(CC) -m32 -c $? -o $@

kernel_asm.o: kernel.asm
	$(CC_ASM) -f elf32 $? -o $@

qemu: all
	qemu-system-x86_64 -kernel $(BUILD_DIR)/kernel.bin

clean:
	rm -rf $(TARGET) *.o
