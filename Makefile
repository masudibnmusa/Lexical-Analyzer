
## Makefile

```makefile
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
TARGET = lexical_analyzer
SRCDIR = src
SOURCES = $(SRCDIR)/main.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build
release: CFLAGS += -O2
release: $(TARGET)

# Create build directory
build:
	mkdir -p build

# Install (copy to /usr/local/bin)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Clean build artifacts
clean:
	rm -f $(TARGET)
	rm -rf build

# Run tests
test: $(TARGET)
	@echo "Running basic tests..."
	@./$(TARGET) < test_input.txt || true

# Format code
format:
	clang-format -i $(SOURCES)

.PHONY: all debug release clean install test format
