CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = 3mpile

# Source and object files
SRC = src/main.c src/lexer.c src/parser.c src/codegen.c
OBJ = $(SRC:.c=.o)

# Default rule
all: $(TARGET)

# Link all object files into the final binary
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Compile .c to .o
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Optional: rebuild from scratch
rebuild: clean all
