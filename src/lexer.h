#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef enum {
  TOKEN_DATATYPE,
  TOKEN_IDENT,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_CHAR,
  TOKEN_ASSIGN,
  TOKEN_PLUS_ASSIGN,
  TOKEN_MINUS_ASSIGN,
  TOKEN_PRINT,
  TOKEN_SEMICOLON,
  TOKEN_EOF
} TokenType;

typedef struct {
  TokenType type;
  char *lexeme;
} Token;

Token *lexFile(const char *path, int *out_len);
