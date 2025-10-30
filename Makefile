CC = gcc
CFLAGS = -Wall -std=c99

TARGET = sum_test

SOURCES = main.c sum_algorithms.c

HEADERS = sum_algorithms.h

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

# Rule to build the executable from object files
# The executable depends on all object files
# $^ means "all prerequisites" (all the .o files)
# $@ means "the target" (sum_test)
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) -lm

# Rule to compile .c files into .o files
# This pattern rule applies to any .c file
# $< means "first prerequisite" (the .c file)
# $@ means "the target" (the .o file)
%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

run: $(TARGET)
	./$(TARGET)

test_quick: $(TARGET)
	@echo "Running quick test (2^20 to 2^24)..."
	@./$(TARGET) | grep -A 6 "2^2[0-4]"

rebuild: clean all

help:
	@echo "Available targets:"
	@echo "  make          - Build the program (default)"
	@echo "  make run      - Build and run with all test sizes"
	@echo "  make clean    - Remove compiled files"
	@echo "  make rebuild  - Clean and rebuild from scratch"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Build configuration:"
	@echo "  Compiler:     $(CC)"
	@echo "  C Flags:      $(CFLAGS)"
	@echo "  Linker Flags: $(LDFLAGS)"
	@echo ""
	@echo "The program tests summation algorithms with array sizes"
	@echo "from 2^10 (1,024) to 2^27 (134,217,728) elements."

.PHONY: all clean run test_quick rebuild help