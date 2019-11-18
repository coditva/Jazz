ifndef ARCH
	ARCH    = i686-elf
endif

# Set debug mode
ifndef RELEASE
	RELEASE = false
endif

# Set architecture type
ifndef ARCH_TYPE
	ARCH_TYPE = ARCH_X86
endif

CC            = ../.deps/cross/bin/$(ARCH)-gcc
LD            = ../.deps/cross/bin/$(ARCH)-ld
CC_ASM        = nasm
CC_FLAGS      = $(CFLAGS) -fno-builtin -std=gnu99 -Wall -Wextra -Werror

ifeq ($(RELEASE), false)
CD_FLAGS      = -g
endif

LINT          = clang-tidy
LINT_FLAGS    = -fix-errors

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
				@echo building $@
				@$(LD) -T linker.ld $(OBJ) $(ASM_OBJ) -o $@

$(ASM_OBJ):%_asm.o: %.asm makefile
				@echo building $@
				@$(CC_ASM) -f elf32 $< -o $@

$(OBJ):%.o:   	%.c $(INCLUDE) config.h makefile
				@echo building $@
				@$(CC) $(CC_FLAGS) $(CD_FLAGS) -I. -I${INCLUDE_DIR} -c $< -o $@

.COMPILER_INFO:
				@touch .COMPILER_INFO
				@echo -- using `$(CC_ASM) -v`
				@echo -- using $(CC) `$(CC) -dumpversion` `$(CC) -dumpmachine`
				@echo -- using $(LD) `$(LD) -v`

config.h:		makefile configure.sh
				@echo "Generating $@"
				@RELEASE=$(RELEASE) bash configure.sh

phony 			+= configure
configure:		config.h

phony 			+= qemu
qemu: 			all
				qemu-system-x86_64 -kernel $(BIN) \
					-serial file:///tmp/jazz_serial1.log

gdb:			all
				gdb $(BIN) \
					-ex "set architecture i386:x86-64" \
					-ex "target remote | qemu-system-x86_64 -kernel $(BIN) \
						-serial file:///tmp/jazz_serial1.log -gdb stdio -S"

# config.h file needs to exist before linting.
phony			+= lint
lint:			configure
				$(LINT) `find . -name "*.c" -or -name "*.h"` $(LINT_FLAGS)\
					-- -I. -Iinclude/

phony 			+= clean
clean:
				rm -rf $(BIN) $(OBJ) $(ASM_OBJ) .COMPILER_INFO

phony 			+= distclean
distclean:
				rm -rf $(BIN) $(OBJ) $(ASM_OBJ) .COMPILER_INFO config.h

phony 			+= tags
tags:
				ctags -R .

.PHONY: 		$(phony)
