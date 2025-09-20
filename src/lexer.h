#ifndef LEXER_H 
#define LEXER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"

bool isNumber(char thing);

bool isKeyword(char *thing);

bool isOperator(char thing);

bool isConstant(char *thing);

bool isDataType(char *thing);

bool isSemiColon(char thing);

char* subString(const char* str, int left, int right);


TokenList parse_code(const char *str);

#endif
