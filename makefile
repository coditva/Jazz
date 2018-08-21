
BUILD_DIR  = build
DISK_IMG   = $(BUILD_DIR)/disk.img
BOOTLOADER = $(BUILD_DIR)/bootloader/boot.bin
KERNEL     = $(BUILD_DIR)/kernel/kernel.bin

.PHONY: all clean $(KERNEL)


all:           $(DISK_IMG)

$(DISK_IMG):   $(BUILD_DIR) $(BOOTLOADER) $(KERNEL)
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd if=$(BOOTLOADER) of=$(DISK_IMG) bs=512 count=1 seek=0
	dd if=$(KERNEL) of=$(DISK_IMG) bs=512 seek=1

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOTLOADER): bootloader/boot.asm
	$(MAKE) --directory bootloader

$(KERNEL):
	$(MAKE) --directory kernel

qemu: all
	@echo "---------------------------------------------"
	@echo "The development on the bootloader is paused."
	@echo "Use \"make qemu_kernel\" instead."
	@echo "---------------------------------------------"
	qemu-system-x86_64 -drive format=raw,file=build/disk.img

qemu_kernel: all
	make --directory kernel qemu

clean:
	$(MAKE) --directory bootloader clean
	$(MAKE) --directory kernel clean
	rm -f $(DISK_IMG)
