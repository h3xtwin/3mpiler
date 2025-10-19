#include "codegen.h"
#include "lexer.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: 3mpile <file>\n");
    return 1;
  }

  char *path = argv[1];
  int token_count = 0;

  // --------- lexer ---------
  Token *tokens = lexFile(path, &token_count);
  if (!tokens) {
    fprintf(stderr, "Failed to lex file: %s\n", path);
    return 1;
  }
#if DEBUG
  printf("Lexing complete. %d tokens generated.\n", token_count);
#endif

  // --------- parser ---------
  ASTNode *program = parseProgram(tokens, token_count);
  if (!program) {
    fprintf(stderr, "Parsing failed!\n");
    free(tokens);
    return 1;
  }
#if DEBUG
  printf("Parsing complete. %d statements in program.\n", program->stmt_count);
  printAST(program, 0);
#endif

  // --------- generate assembly ---------
  generateAssembly(program, "out.asm");
  printf("Code generation completed! outputted to out.asm\n");

  free(tokens);
  return 0;
}
