#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

typedef enum {
	KEYWORD,
	OPERATOR,
	VARIABLE,
	CONSTANT,
	DATATYPE,
	SPECIAL
}TokenType;

typedef struct{
	TokenType type;
	char *lexeme;
}Token;

typedef struct TokenList{
	Token val;
	struct TokenList *next;
}TokenList;

void push_back(TokenList *root,Token val);
#endif
