# Compiler and flags
CC = gcc
# Tirei o -Werror para evitar que avisos simples na biblioteca do professor travem a compilação
CFLAGS = -Wall -g -I$(INCLUDE_DIR) -DUTF8_SUPPORT

# Project name (MUDAMOS AQUI PARA O NOME DO SEU JOGO)
PROJ_NAME = sharklog

# Target directories
BUILD_DIR   = build
OBJ_DIR     = $(BUILD_DIR)/obj
SRC_DIR     = src
INCLUDE_DIR = include

# Source files
# O wildcard pega automaticamente todos os .c (jogo, logica, main, screen, etc)
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
# Removi a dependência fixa de 'tabuleiro.h' aqui para evitar erros genéricos.
# Agora ele recompila se o .c mudar.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
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