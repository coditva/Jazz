
CC         = nasm
BUILD_DIR  = ../build/bootloader
BIN        = $(BUILD_DIR)/boot.bin

all:       $(BIN)

$(BIN): boot.asm $(BUILD_DIR)
	$(CC) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(BIN)
