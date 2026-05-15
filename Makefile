BUILD_DIR := build
TARGET_PATH := $(BUILD_DIR)/amd-cpuid

.PHONY: all build run clean

build:
	mkdir -p build
	clang main.c -o $(TARGET_PATH)

run: build
	./$(TARGET_PATH)

all: build
	clang main.c -o $(BUILD_DIR) 

clean:
	rm -rf build
