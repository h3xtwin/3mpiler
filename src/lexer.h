#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isNumber(char thing);

bool isKeyword(char *thing);

bool isOperator(char thing);

bool isConstant(char *thing);

bool isDataType(char *thing);

bool isSemiColon(char thing);

bool isSemiColon(char thing);

bool isUSELESS(char thing);

char *subString(const char *str, int left, int right);

TokenList parse_code(const char *str);

#endif
