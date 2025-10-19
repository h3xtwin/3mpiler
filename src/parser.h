#pragma once
#include "lexer.h"

typedef enum {
  AST_NUMBER,
  AST_CHAR,
  AST_STRING,
  AST_VARIABLE,
  AST_ASSIGN,
  AST_PRINT,
  AST_PROGRAM
} ASTType;

typedef struct ASTNode {
  ASTType type;

  // For leaf nodes
  char *value;

  // For assignment / print
  struct ASTNode *left;
  struct ASTNode *right;

  // For program root (list of statements)
  struct ASTNode **statements;
  int stmt_count;

} ASTNode;

typedef struct {
  Token *tokens;
  int pos;
  int len;
} Parser;

ASTNode *parseProgram(Token *tokens, int len);
ASTNode *parseStatement(Parser *p);
ASTNode *parsePrimary(Parser *p);
Token *current(Parser *p);
Token *advance(Parser *p);

// print AST
void printAST(ASTNode *node, int indent);
