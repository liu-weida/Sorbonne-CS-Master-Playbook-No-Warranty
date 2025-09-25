CC = gcc
CFLAGS = -Wall -O3 -lm

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

MAIN_EXEC = main
DEPS = $(wildcard $(INCLUDE_DIR)/*.h)

all: $(MAIN_EXEC)

$(MAIN_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(MAIN_EXEC)

.PHONY: all clean
