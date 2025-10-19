#define _POSIX_C_SOURCE 200809L
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------- Util -----------------
Token *current(Parser *p) {
  if (p->pos >= p->len)
    return NULL;
  return &p->tokens[p->pos];
}

Token *advance(Parser *p) {
  if (p->pos >= p->len)
    return NULL;
  return &p->tokens[p->pos++];
}

// ----------------- Primary expressions -----------------
ASTNode *parsePrimary(Parser *p) {
  Token *t = current(p);
  if (!t)
    return NULL;

  ASTNode *node = malloc(sizeof(ASTNode));
  node->left = node->right = NULL;
  node->statements = NULL;
  node->stmt_count = 0;

  switch (t->type) {
  case TOKEN_NUMBER:
    node->type = AST_NUMBER;
    node->value = strdup(t->lexeme);
    break;
  case TOKEN_CHAR:
    node->type = AST_CHAR;
    node->value = strdup(t->lexeme);
    break;
  case TOKEN_STRING:
    node->type = AST_STRING;
    node->value = strdup(t->lexeme);
    break;
  case TOKEN_IDENT:
    node->type = AST_VARIABLE;
    node->value = strdup(t->lexeme);
    break;
  default:
    free(node);
    return NULL;
  }

  advance(p);
  return node;
}

// ----------------- Statements -----------------
ASTNode *parseStatement(Parser *p) {
  Token *t = current(p);
  if (!t)
    return NULL;

  // --- Print statement ---
  if (t->type == TOKEN_PRINT) {
    advance(p);                      // skip 'print'
    ASTNode *expr = parsePrimary(p); // number, string, char, variable
    if (!expr)
      return NULL;

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_PRINT;
    node->left = expr;
    node->right = NULL;
    node->value = NULL;
    return node;
  }

  // --- int/char/string var = <val> ---
  if (t->type == TOKEN_DATATYPE) {
    advance(p); // skip datatypes
    Token *varToken = current(p);
    if (!varToken || varToken->type != TOKEN_IDENT)
      return NULL;

    ASTNode *varNode = malloc(sizeof(ASTNode));
    varNode->type = AST_VARIABLE;
    varNode->value = strdup(varToken->lexeme);
    varNode->left = varNode->right = NULL;
    varNode->statements = NULL;
    varNode->stmt_count = 0;
    advance(p);

    Token *eq = current(p);
    if (!eq || eq->type != TOKEN_ASSIGN)
      return NULL;
    advance(p); // skip the =

    ASTNode *value = parsePrimary(p);
    if (!value)
      return NULL;

    ASTNode *assignNode = malloc(sizeof(ASTNode));
    assignNode->type = AST_ASSIGN;
    assignNode->left = varNode;
    assignNode->right = value;
    assignNode->value = NULL;
    assignNode->statements = NULL;
    assignNode->stmt_count = 0;
    return assignNode;
  }

  // --- Unknown token ---
  advance(p);
  return NULL;
}

// ----------------- Program -----------------
ASTNode *parseProgram(Token *tokens, int len) {
  Parser p = {tokens, 0, len};
  ASTNode *program = malloc(sizeof(ASTNode));
  program->type = AST_PROGRAM;
  program->statements = malloc(sizeof(ASTNode *) * 64);
  program->stmt_count = 0;
  program->value = NULL;
  program->left = program->right = NULL;

  while (current(&p) && current(&p)->type != TOKEN_EOF) {
    ASTNode *stmt = parseStatement(&p);
    if (stmt)
      program->statements[program->stmt_count++] = stmt;
  }

  return program;
}

// ----------------- Print -----------------
void printAST(ASTNode *node, int indent) {
  for (int i = 0; i < indent; i++)
    printf("  ");

  switch (node->type) {
  case AST_NUMBER:
    printf("Number: %s\n", node->value);
    break;
  case AST_CHAR:
    printf("Char: %s\n", node->value);
    break;
  case AST_STRING:
    printf("String: %s\n", node->value);
    break;
  case AST_VARIABLE:
    printf("Variable: %s\n", node->value);
    break;
  case AST_ASSIGN:
    printf("Assign:\n");
    printAST(node->left, indent + 1);
    printAST(node->right, indent + 1);
    break;
  case AST_PRINT:
    printf("Print:\n");
    printAST(node->left, indent + 1);
    break;
  case AST_PROGRAM:
    printf("Program:\n");
    for (int i = 0; i < node->stmt_count; i++)
      printAST(node->statements[i], indent + 1);
    break;
  }
}
