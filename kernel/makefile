
CC            = gcc
CC_ASM        = nasm
CC_FLAGS      = -fno-builtin -fno-stack-protector
CD_FLAGS      = -g

BUILD_DIR     = ../build/kernel
TARGET        = $(BUILD_DIR)/kernel.bin

ASM_OBJS      = kernel_asm.o

OBJS          = kernel.o \
                interrupt/interrupt.o \
                interrupt/handler.o \
                boot/gdt.o \
                io/video/video.o \
                io/serial/serial.o \
                io/io.o \
                io/kio.o \
                boot/multiboot.o \
                mm/page_frame.o \
                lib/string.o

INCLUDE       = interrupt/interrupt.h \
                util/keyboard_map.h \
                config.h \
                boot/gdt.h \
                io/portio/portio.h \
                io/video/video.h \
                io/serial/serial.h \

INCLUDE_DIR   = include
phony         =

phony += all
all:          $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET):    $(OBJS) $(INCLUDE) $(ASM_OBJS) $(BUILD_DIR)
	ld -m elf_i386 -T linker.ld $(OBJS) $(ASM_OBJS) -o $@

$(ASM_OBJS):%_asm.o: %.asm $(INCLUDE_DIR)/asm/*.asm
	$(CC_ASM) -f elf32 $< -o $@

$(OBJS):%.o:   %.c $(INCLUDE) $(INCLUDE_DIR)/*
	$(CC) $(CC_FLAGS) $(CD_FLAGS) -m32 -I. -I${INCLUDE_DIR} -c $< -o $@

qemu: all
	qemu-system-x86_64 -kernel $(BUILD_DIR)/kernel.bin \
	  -serial file:///tmp/jazz_serial1.log

phony += clean
clean:
	rm -rf $(TARGET) $(OBJS) $(ASM_OBJS)

phony += tags
tags:
	ctags -R .


.PHONY: $(phony)
