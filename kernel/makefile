ifndef TARGET
	TARGET    = i686-elf
endif

CC            = ../.deps/cross/bin/$(TARGET)-gcc
LD            = ../.deps/cross/bin/$(TARGET)-ld
CC_ASM        = nasm
CC_FLAGS      = $(CFLAGS) -fno-builtin -std=gnu99 -Wall -Wextra -Werror
CD_FLAGS      = -g

BUILD_DIR     = ../build/kernel
BIN           = $(BUILD_DIR)/kernel.bin

ASM_SRC       = $(shell find . -name "*.asm")
ASM_OBJ       = $(patsubst %.asm,%_asm.o,$(ASM_SRC))

SRC           = $(shell find . -name "*.c")
OBJ           = $(patsubst %.c,%.o,$(SRC))

INCLUDE       = $(shell find . -name "*.h")

INCLUDE_DIR   = include
phony         =

phony 			+= all
all:          	$(BIN)

$(BUILD_DIR):
				mkdir -p $(BUILD_DIR)

$(BIN):			.COMPILER_INFO $(BUILD_DIR) $(ASM_OBJ) $(OBJ) linker.ld
				@$(LD) -T linker.ld $(OBJ) $(ASM_OBJ) -o $@
				@echo building $@

$(ASM_OBJ):%_asm.o: %.asm makefile
				@$(CC_ASM) -f elf32 $< -o $@
				@echo building $@

$(OBJ):%.o:   	%.c $(INCLUDE) makefile
				@$(CC) $(CC_FLAGS) $(CD_FLAGS) -I. -I${INCLUDE_DIR} -c $< -o $@
				@echo building $@

.COMPILER_INFO:
				@touch .COMPILER_INFO
				@echo -- using `$(CC_ASM) -v`
				@echo -- using $(CC) `$(CC) -dumpversion` `$(CC) -dumpmachine`
				@echo -- using $(LD) `$(LD) -v`

phony 			+= qemu
qemu: 			all
				qemu-system-x86_64 -kernel $(BUILD_DIR)/kernel.bin \
					-serial file:///tmp/jazz_serial1.log

phony 			+= clean
clean:
				rm -rf $(BIN) $(OBJ) $(ASM_OBJ) .COMPILER_INFO

phony 			+= tags
tags:
				ctags -R .

.PHONY: 		$(phony)
