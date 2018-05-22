
CC         = nasm
BUILD_DIR  = ../build/bootloader
TARGET     = $(BUILD_DIR)/boot.bin

all:       $(TARGET)

$(TARGET): boot.asm $(BUILD_DIR)
	$(CC) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGET)
