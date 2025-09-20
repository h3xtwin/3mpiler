CC = gcc

3mpiler:
	$(CC) src/lexer.c src/token.c -o 3mpiler

clean:
	rm 3mpiler
