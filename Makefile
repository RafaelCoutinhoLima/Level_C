# Compilador/projeto
CC        := gcc
PROJ_NAME := level_c

# Estruturas de build
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
SRC_DIR   := src

# Flags de compilacao
CFLAGS  := -Wall -Wextra -O2 -std=c99 -I$(SRC_DIR)
LDFLAGS :=

# Detecta SO para linkar Raylib corretamente
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin) # macOS
    CFLAGS  += -I/usr/local/include -I/opt/homebrew/include
    LDFLAGS += -L/usr/local/lib -L/opt/homebrew/lib -lraylib \
               -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else ifeq ($(OS),Windows_NT) # Windows (MSYS2/MinGW)
    # Ajuste o prefixo se necessario (ex.: /mingw64)
    CFLAGS  += -I/mingw64/include
    LDFLAGS += -L/mingw64/lib -lraylib -lopengl32 -lgdi32 -lwinmm
else # Linux
    LDFLAGS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Todos os .c dentro de src/
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Alvos
.PHONY: all run clean rebuild help

all: $(BUILD_DIR)/$(PROJ_NAME)

$(BUILD_DIR)/$(PROJ_NAME): $(OBJS)
	@echo "Linking: $@"
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling: $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

run: all
	@echo "Running $(PROJ_NAME)..."
	@./$(BUILD_DIR)/$(PROJ_NAME)

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

rebuild: clean all

help:
	@echo "Targets:"
	@echo "  make           -> compila"
	@echo "  make run       -> compila e executa"
	@echo "  make clean     -> remove build/"
	@echo "  make rebuild   -> clean + build"
