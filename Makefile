# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g -I$(INCLUDE_DIR) -DUTF8_SUPPORT

#project name
PROJ_NAME = cli-lib-example

# Target directories
BUILD_DIR   = build
OBJ_DIR     = $(BUILD_DIR)/obj
SRC_DIR     = src
INCLUDE_DIR = include

# Source files
SRC_FILES = $(notdir $(wildcard $(SRC_DIR)/*.c))
OBJ_FILES = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

# -----------------------------
# Build target
# -----------------------------
all: $(OBJ_DIR) $(OBJ_FILES)
	@echo Creating $(BUILD_DIR)/$(PROJ_NAME)
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(PROJ_NAME) $(OBJ_FILES)
	@echo Build complete! UTF-8 emojis ready 🎉

# -----------------------------
# Build directories
# -----------------------------
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

# -----------------------------
# Object files compilation
# -----------------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/tabuleiro.h
	@echo Compiling $@...
	@$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------
# Clean target
# -----------------------------
clean:
	rm -rf $(BUILD_DIR)
	@echo Clean complete!

# -----------------------------
# Rebuild target (clean + all)
# -----------------------------
rebuild: clean all
	@echo Rebuild complete!

# -----------------------------
# Run target
# -----------------------------
run: all
	@echo Running $(PROJ_NAME)...
	./$(BUILD_DIR)/$(PROJ_NAME)
