# Define directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Find all source files (*.c)
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)

# Create object files from source files (*.o)
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Define the name of the final executable
EXECUTABLE = $(BIN_DIR)/program

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Rule to compile the program
$(EXECUTABLE): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(OBJ_FILES) -o $@

# Rule to create object files from C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up compiled files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: clean
