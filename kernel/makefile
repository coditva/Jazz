
CC            = gcc
CC_ASM        = nasm
BUILD_DIR     = ../build/kernel
TARGET        = $(BUILD_DIR)/kernel.bin

ASM_OBJS      = kernel_asm.o \
                util/portio_asm.o

OBJS          = kernel.o \
                video/video.o \
                interrupt/interrupt.o \
                interrupt/handler.o \
                serial/serial.o \
                memory/gdt.o

INCLUDE       = util/portio.h \
                video/video.h \
                util/types.h \
                interrupt/interrupt.h \
                util/keyboard_map.h \
                config.h \
                serial/serial.h \
                memory/gdt.h \
                debug/debug.h

INCLUDE_DIR   = include

all:          $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET):    $(OBJS) $(INCLUDE) $(ASM_OBJS) $(BUILD_DIR)
	ld -m elf_i386 -T linker.ld $(OBJS) $(ASM_OBJS) -o $@

$(ASM_OBJS):%_asm.o: %.asm
	$(CC_ASM) -f elf32 $? -o $@

$(OBJS):%.o:   %.c $(INCLUDE) $(INCLUDE_DIR)/*
	$(CC) -fno-stack-protector -m32 -I. -I${INCLUDE_DIR} -c $< -o $@

qemu: all
	qemu-system-x86_64 -kernel $(BUILD_DIR)/kernel.bin -serial file:`mktemp /tmp/jazz_serial1_XXXXXX.log`

clean:
	rm -rf $(TARGET) $(OBJS) $(ASM_OBJS)
