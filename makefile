
BUILD_DIR  = build
DISK_IMG   = $(BUILD_DIR)/disk.img
BOOTLOADER = $(BUILD_DIR)/bootloader/boot.bin
KERNEL     = $(BUILD_DIR)/kernel/kernel.bin

.PHONY: all bootloader clean


all:           $(DISK_IMG)

$(DISK_IMG):   $(BUILD_DIR) $(BOOTLOADER) $(KERNEL)
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd if=$(BOOTLOADER) of=$(DISK_IMG) bs=512 count=1 seek=0

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOTLOADER): bootloader/boot.asm
	$(MAKE) --directory bootloader

$(KERNEL): kernel/kernel.c kernel/kernel.asm kernel/linker.ld
	$(MAKE) --directory kernel

qemu:
	qemu-system-x86_64 -drive format=raw,file=build/disk.img

clean:
	$(MAKE) --directory bootloader clean
	rm -f $(DISK_IMG)
