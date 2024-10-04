#! In case it cant find the defintion add 
# #elif defined (__AVR_AVR128DB48__)
# #  include <avr/ioavr128db48.h>' to the used io.h file",
# with '#' included

# MCU and Programmer Configuration
MCU = avr128db48
PROGRAMMER = pkobn_updi
PORT = usb

# Paths
ifeq ($(OS),Windows_NT)
	USER_HOME = $(USERPROFILE)
	ATPACK_DIR = $(USER_HOME)/Documents/Microchip.AVR-Dx_DFP.2.6.303
	MKDIR = mkdir
	DEL = del /Q
else
	USER_HOME = $(HOME)
	ATPACK_DIR = $(USER_HOME)/Documents/Microchip.AVR-Dx_DFP.2.6.303
	MKDIR = mkdir -p
	DEL = rm -rf
endif

DEV_SPECS = $(ATPACK_DIR)/gcc/dev/$(MCU)
INCLUDE_PATH = $(ATPACK_DIR)/include

# Directories
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

# Source files and build settings
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
TARGET = $(BUILD_DIR)/main.elf
FLASH_FILE = $(BUILD_DIR)/main.hex
CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -B $(DEV_SPECS) -isystem $(INCLUDE_PATH) -I$(INCLUDE_DIR) -Os

# Compilation rule
all: $(TARGET)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the target ELF file
$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the hex file
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	avr-objcopy -O ihex $< $@

# Flash rule
flash: $(FLASH_FILE)
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(MCU) -U flash:w:$(FLASH_FILE):i

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash clean