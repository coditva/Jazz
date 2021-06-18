ifndef GCC_VERSION
	GCC_VERSION = 5.4.0
endif

ifndef BINUTILS_VERSION
	BINUTILS_VERSION = 2.29
endif

ifndef TARGET
	TARGET = i686-elf
endif

DEPS_PREFIX       = $(shell pwd)/.deps/cross

BUILD_DIR         = build
DISK_IMG          = $(BUILD_DIR)/disk.img
BOOTLOADER        = $(BUILD_DIR)/bootloader/bootloader.bin
BOOT              = $(BUILD_DIR)/boot/boot.bin
KERNEL            = $(BUILD_DIR)/kernel/kernel.bin

MAKE_FLAGS        = -j 8
MAKE_CMD          = $(MAKE) \
						PREFIX=$(DEPS_PREFIX) \
						ARCH=$(TARGET) \
						GCC_VERSION=$(GCC_VERSION) \
						BINUTILS_VERSION=$(BINUTILS_VERSION) \
						$(MAKE_FLAGS)

phony = all
all: $(DISK_IMG)

$(DISK_IMG): $(BUILD_DIR) $(BOOTLOADER) $(BOOT) $(KERNEL)
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd if=$(BOOTLOADER) of=$(DISK_IMG) bs=512 count=1 seek=0
	dd if=$(KERNEL) of=$(DISK_IMG) bs=512 seek=1

phony += $(BUILD_DIR)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

phony += $(BOOTLOADER)
$(BOOTLOADER): bootloader/boot.asm
	$(MAKE_CMD) --directory bootloader

phony += $(BOOT)
$(BOOT): deps
	$(MAKE_CMD) --directory boot

phony += $(KERNEL)
$(KERNEL): deps
	$(MAKE_CMD) --directory kernel

phony += qemu
qemu: all
	@echo "---------------------------------------------"
	@echo "The development on the bootloader is paused."
	@echo "Use \"make qemu_kernel\" instead."
	@echo "---------------------------------------------"
	qemu-system-x86_64 -drive format=raw,file=build/disk.img

phony += qemu_kernel
qemu_kernel: $(KERNEL)
	$(MAKE_CMD) --directory kernel qemu

phony += lint
lint:
	$(MAKE_CMD) --directory boot lint
	$(MAKE_CMD) --directory kernel lint

phony += clean
clean:
	$(MAKE_CMD) --directory bootloader clean
	$(MAKE_CMD) --directory boot clean
	$(MAKE_CMD) --directory kernel clean
	rm -f $(DISK_IMG)

distclean:
	$(MAKE_CMD) --directory .deps distclean

phony += test
test: all
	@echo "No tests added"

phony += deps
deps:
	$(MAKE_CMD) --directory .deps all

.PHONY: $(phony)
