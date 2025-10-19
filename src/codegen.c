
#include "parser.h"
#include <stdio.h>

void generateAssembly(ASTNode *program, const char *outfile) {
  FILE *f = fopen(outfile, "w");
  if (!f) {
    perror("fopen");
    return;
  }

  // -----------------------
  // SECTION: .data
  // -----------------------
  fprintf(f, "section .data\n");

  // Emit variables and string literals
  for (int i = 0; i < program->stmt_count; i++) {
    ASTNode *stmt = program->statements[i];

    if (stmt->type == AST_ASSIGN) {
      ASTNode *var = stmt->left;
      ASTNode *value = stmt->right;

      if (value->type == AST_NUMBER)
        fprintf(f, "%s dq %s\n", var->value, value->value);
      else if (value->type == AST_CHAR)
        fprintf(f, "%s db %s, 0\n", var->value, value->value);
      else if (value->type == AST_STRING)
        fprintf(f, "%s db %s, 0\n", var->value, value->value);
    }

    else if (stmt->type == AST_PRINT) {
      ASTNode *val = stmt->left;
      if (val->type == AST_STRING) {
        // newline
        fprintf(f, "print_str db %s, 0xa\n", val->value);
        fprintf(f, "print_len equ $ - print_str\n");
      }
    }
  }

  // -----------------------
  // SECTION: .text
  // -----------------------
  fprintf(f, "\nsection .text\n");
  fprintf(f, "global _start\n\n_start:\n");

  // Emit code for print calls
  for (int i = 0; i < program->stmt_count; i++) {
    ASTNode *stmt = program->statements[i];

    if (stmt->type == AST_PRINT) {
      ASTNode *val = stmt->left;

      if (val->type == AST_STRING) {
        fprintf(f, "    mov rax, 1        ; sys_write\n"
                   "    mov rdi, 1        ; stdout\n"
                   "    mov rsi, print_str ; address of string\n"
                   "    mov rdx, print_len ; length\n"
                   "    syscall\n");
      }
    }
  }

  // Exit syscall
  fprintf(f, "\n    mov rax, 60     ; sys_exit\n"
             "    xor rdi, rdi\n"
             "    syscall\n");

  fclose(f);
}
