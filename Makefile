CC = gcc
CFLAGS = -Wall -O2
SRC_DIR = src
UTIL_SRC_DIR = src/utils
BUILD_DIR = build

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
UTIL_SRC_FILES := $(wildcard $(UTIL_SRC_DIR)/*.c)

OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
UTIL_OBJ_FILES := $(patsubst $(UTIL_SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(UTIL_SRC_FILES))

all: $(OBJ_FILES) $(UTIL_OBJ_FILES)
	$(CC) $(CFLAGS) -o exe $^

install: $(OBJ_FILES) $(UTIL_OBJ_FILES)
	$(CC) $(CFLAGS) -o cneofetch $^
	rm -rf /usr/local/bin/cneofetch
	mv cneofetch /usr/local/bin

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(UTIL_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -rf exe cneofetch

.PHONY: all clean
