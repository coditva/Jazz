
CC            = gcc
CC_ASM				= nasm
BUILD_DIR     = ../build/kernel
TARGET        = $(BUILD_DIR)/kernel.bin

all:          $(TARGET)

$(TARGET):    $(BUILD_DIR) kernel.asm.o kernel.c.o
	ld -m elf_i386 -T linker.ld -o $@ $?

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

kernel.c.o:   kernel.c
	$(CC) -m32 -c $< -o $@

kernel.asm.o: kernel.asm
	$(CC_ASM) -f elf32 $< -o $@

clean:
	rm -f $(TARGET)
