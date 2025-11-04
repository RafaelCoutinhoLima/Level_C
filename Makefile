CC = gcc
PROJ_NAME = level_c

BUILD_DIR   = build
OBJ_DIR     = $(BUILD_DIR)/obj
SRC_DIR     = src

CFLAGS = -Wall -Wextra -O2 -std=c99 -I$(SRC_DIR)
LDFLAGS =

# Flags e libs para raylib (ajuste conforme seu SO)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin) # macOS
    CFLAGS += -I/usr/local/include -I/opt/homebrew/include
    LDFLAGS += -L/usr/local/lib -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else # Linux
    LDFLAGS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(BUILD_DIR)/$(PROJ_NAME)

$(BUILD_DIR)/$(PROJ_NAME): $(OBJS)
	@echo "Linking: $@"
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling: $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

run: all
	@echo "Running $(PROJ_NAME)..."
	@./$(BUILD_DIR)/$(PROJ_NAME)