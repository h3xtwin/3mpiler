#define _POSIX_C_SOURCE 200809L
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------- Type checkers -----------------
bool isNumber(char thing) { return thing >= '0' && thing <= '9'; }

bool isCall(char *thing) { return strcmp(thing, "print") == 0; }

bool isString(char *str) {
  if (str[0] != '\"')
    return false;
  for (int i = 1; i < strlen(str) - 1; i++) {
    if (str[i] == '\"')
      return false;
  }
  return true;
}

bool isChar(char *str) { return str[0] == '\'' && str[2] == '\'' && !str[3]; }

bool isConstantNumber(char *str) {
  for (int i = 0; i < strlen(str); i++)
    if (!isNumber(str[i]))
      return false;
  return true;
}

bool isDataType(char *str) {
  return strcmp(str, "int") == 0 || strcmp(str, "char") == 0 ||
         strcmp(str, "string") == 0;
}

bool isSemiColon(char thing) { return thing == ';'; }
bool isUSELESS(char thing) { return thing == ' ' || thing == '\n'; }

// ----------------- Substring helper -----------------
char *subString(char *str, int left, int right) {
  if (right < left)
    return NULL;
  int len = right - left;
  char *subStr = malloc(len + 1);
  for (int i = 0; i < len; i++, left++)
    subStr[i] = str[left];
  subStr[len] = '\0';
  return subStr;
}

// ----------------- Lexer -----------------
Token getTokenType(char *subStr) {
  if (isCall(subStr)) {
    printf("%s is a call\n", subStr);
    return (Token){TOKEN_PRINT, strdup(subStr)};
  } else if (isConstantNumber(subStr)) {
    printf("%s is a constant number\n", subStr);
    return (Token){TOKEN_NUMBER, strdup(subStr)};
  } else if (strcmp(subStr, "=") == 0) {
    printf("%s is an assigner\n", subStr);
    return (Token){TOKEN_ASSIGN, strdup(subStr)};
  } else if (strcmp(subStr, "+=") == 0) {
    printf("%s is a plus assigner\n", subStr);
    return (Token){TOKEN_PLUS_ASSIGN, strdup(subStr)};
  } else if (strcmp(subStr, "-=") == 0) {
    printf("%s is a minus assigner\n", subStr);
    return (Token){TOKEN_MINUS_ASSIGN, strdup(subStr)};
  } else if (isString(subStr)) {
    printf("%s is a string\n", subStr);
    return (Token){TOKEN_STRING, strdup(subStr)};
  } else if (isChar(subStr)) {
    printf("%s is a char\n", subStr);
    return (Token){TOKEN_CHAR, strdup(subStr)};
  } else if (isDataType(subStr)) {
    printf("%s is a data type\n", subStr);
    return (Token){TOKEN_DATATYPE, strdup(subStr)};
  } else {
    printf("%s is a variable\n", subStr);
    return (Token){TOKEN_IDENT, strdup(subStr)};
  }
}

Token *code_lexer(char *str, int *out_len) {
  int count = 0, cap = 64;
  Token *tokens = malloc(sizeof(Token) * cap);
  int left = 0, right = 0;

  while (str[left]) {
    while (isUSELESS(str[left]))
      left++;
    right = left;

    if (isSemiColon(str[left])) {
      Token t = {TOKEN_SEMICOLON, strdup(";")};
      tokens[count++] = t;
      left++;
      continue;
    }

    if (str[left] == '"') {
      right++;
      while (str[right] && str[right] != '"')
        right++;
      right++;
    } else {
      while (str[right] && !isUSELESS(str[right]) && !isSemiColon(str[right]))
        right++;
    }

    char *subStr = subString(str, left, right);
    if (!subStr || strlen(subStr) == 0) {
      free(subStr);
      left = right;
      continue;
    }

    Token t = getTokenType(subStr);

    if (count >= cap) {
      cap *= 2;
      tokens = realloc(tokens, sizeof(Token) * cap);
    }

    tokens[count++] = t;
    free(subStr);
    left = right;
  }

  tokens[count++] = (Token){TOKEN_EOF, strdup("EOF")};
  *out_len = count;
  return tokens;
}

// ----------------- Read file -----------------

char *fileToString(const char *path) {
  FILE *pFile = fopen(path, "r");
  if (!pFile) {
    perror("failed to open file");
    return NULL;
  }

  fseek(pFile, 0, SEEK_END);
  long len = ftell(pFile);
  fseek(pFile, 0, SEEK_SET);

  char *buffer = malloc(len + 1);
  fread(buffer, 1, len, pFile);
  buffer[len] = '\0';
  fclose(pFile);
  return buffer;
}

Token *lexFile(const char *path, int *out_len) {
  char *contents = fileToString(path);
  if (!contents)
    return NULL;
  Token *tokens = code_lexer(contents, out_len);
  free(contents);
  return tokens;
}
