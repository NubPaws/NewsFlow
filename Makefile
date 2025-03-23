# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude

# Directories
SRC_DIR     = src
OBJ_DIR     = obj
INCLUDE_DIR = include

# Files
SRCS    = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET  = app

# Default target
.PHONY: all
all: $(TARGET)

# Link the executable from object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build files
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
