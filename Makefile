# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LIBS = -lcurl -lcjson -lssl -lcrypto

# Directories
SRC_DIR = src
INCLUDE_DIR = include
EXAMPLES_DIR = examples
BUILD_DIR = build

# Source files
SRC_FILES = $(SRC_DIR)/gmail_auth.c $(SRC_DIR)/gmail_client.c $(SRC_DIR)/gmail_messages.c $(SRC_DIR)/gmail_utils.c
SRC_OBJS = $(SRC_FILES:.c=.o)

# Example files
EXAMPLES = get_request get_token read_email send_email
EXAMPLE_TARGETS = $(EXAMPLES:%=$(BUILD_DIR)/%)

# Default target
all: $(BUILD_DIR) $(EXAMPLE_TARGETS)

# Build examples
$(BUILD_DIR)/%: $(EXAMPLES_DIR)/%.c $(SRC_OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(SRC_OBJS) $(LIBS)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files into objects
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(SRC_OBJS)
