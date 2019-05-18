ifndef TARGET
	TARGET    = x86_64-elf
endif

CC            = ../.deps/cross/bin/$(TARGET)-gcc
LD            = ../.deps/cross/bin/$(TARGET)-ld
CC_ASM        = nasm
CC_FLAGS      = $(CFLAGS) -fno-builtin -std=gnu99 -Wall -Wextra -Werror
CD_FLAGS      = -g

BUILD_DIR     = ../build/kernel
BIN           = $(BUILD_DIR)/kernel.bin

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
                lib/string.o \
                lib/kcheck.o

INCLUDE       = interrupt/interrupt.h \
                util/keyboard_map.h \
                config.h \
                boot/gdt.h \
                io/portio/portio.h \
                io/video/video.h \
                io/serial/serial.h \

INCLUDE_DIR   = include
phony         =

phony 			+= all
all:          	$(BIN)

$(BUILD_DIR):
				mkdir -p $(BUILD_DIR)

$(BIN):			$(OBJS) $(INCLUDE) $(ASM_OBJS) $(BUILD_DIR)
				$(LD) -T linker.ld $(OBJS) $(ASM_OBJS) -o $@

$(ASM_OBJS):	%_asm.o:%.asm $(INCLUDE_DIR)/asm/*.asm
				$(CC_ASM) -f elf64 $< -o $@

$(OBJS):%.o:   	%.c $(INCLUDE) $(INCLUDE_DIR)/*
				$(CC) $(CC_FLAGS) $(CD_FLAGS) -I. -I${INCLUDE_DIR} -c $< -o $@

phony 			+= qemu
qemu: 			all
				qemu-system-x86_64 -kernel $(BUILD_DIR)/kernel.bin \
					-serial file:///tmp/jazz_serial1.log

phony 			+= clean
clean:
				rm -rf $(BIN) $(OBJS) $(ASM_OBJS)

phony 			+= tags
tags:
				ctags -R .

.PHONY: 		$(phony)
