CC = gcc
PROJ_NAME = level_c

BUILD_DIR   = build
OBJ_DIR     = $(BUILD_DIR)/obj
USER_SRC_DIR = src
LIB_SRC_DIR  = lib/cli-lib/src
INCLUDE_DIR = lib/cli-lib/include

VPATH = $(USER_SRC_DIR) $(LIB_SRC_DIR)

# gcc where to find headers (.h files)
CFLAGS = -Wall -g -std=c99 -I$(INCLUDE_DIR)
# -lm links the math library (required by cli-lib's timer)
LIBS = -lm

# Find all .c file *names* in both source directories

USER_SRCS = $(notdir $(wildcard $(USER_SRC_DIR)/*.c))
LIB_SRCS  = $(notdir $(wildcard $(LIB_SRC_DIR)/*.c))

# Create the list of object files that will live in OBJ_DIR

OBJ_FILES = $(USER_SRCS:%.c=$(OBJ_DIR)/%.o) $(LIB_SRCS:%.c=$(OBJ_DIR)/%.o)

# Default rule
all: $(BUILD_DIR)/$(PROJ_NAME)
# Rule to create the final executable
$(BUILD_DIR)/$(PROJ_NAME): $(OBJ_DIR) $(OBJ_FILES)
	@echo "Linking executable: $@"
	@$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $(LIBS)
# Rule to create the build/obj directory
$(OBJ_DIR):
	@echo "Creating directory: $@"
	@mkdir -p $(OBJ_DIR)
# Rule to compile .c files into .o file
# and this rule will compile it into the OBJ_DIR
$(OBJ_DIR)/%.o: %.c
	@echo "Compiling: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean up all built files

clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)

# Run the project

run: all
	@echo "Running $(PROJ_NAME)..."
	@./$(BUILD_DIR)/$(PROJ_NAME)