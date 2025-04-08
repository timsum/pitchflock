CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = 
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests

LIB_NAME = libpitchflock.a
LIB_SRC = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SRC))

TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_BIN = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_SRC))

INSTALL_LIB_DIR = /usr/local/lib
INSTALL_INCLUDE_DIR = /usr/local/include/pitchflock

all: $(BUILD_DIR) $(LIB_NAME) tests

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_NAME): $(LIB_OBJ)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(TEST_BIN)

$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(LIB_NAME)
	$(CC) $(CFLAGS) $< -L. -lpitchflock -o $@

install: $(LIB_NAME)
# Create installation directories
	mkdir -p $(INSTALL_LIB_DIR)
	mkdir -p $(INSTALL_INCLUDE_DIR)
	# Copy the library
	
	cp $(LIB_NAME) $(INSTALL_LIB_DIR)/
	# Copy the header files
	cp include/*.h $(INSTALL_INCLUDE_DIR)/

uninstall:
# Remove the installed library and headers
	rm -f $(INSTALL_LIB_DIR)/$(LIB_NAME)
	rm -rf $(INSTALL_INCLUDE_DIR)

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME)

.PHONY: all tests clean install uninstall
