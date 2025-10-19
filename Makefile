CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = 3mpile

SRC = src/main.c src/lexer.c src/parser.c src/codegen.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all
