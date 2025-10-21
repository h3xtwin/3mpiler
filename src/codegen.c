
#include "parser.h"
#include <stdio.h>
#include <string.h>

// Poor solution but find variable assign node.
// (Developers note: do your research before making a compiler)

ASTNode *findVarAssign(ASTNode *program, const char *var) {
  for (int i = 0; i < program->stmt_count; i++) {
    ASTNode *stmt = program->statements[i];
    if (stmt->type == AST_ASSIGN) {
      if (strcmp(stmt->left->value, var) == 0)
        return stmt;
    }
  }
  return NULL;
}

// generating Assembly

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
      static int count = 0;
      if (val->type == AST_STRING) {
        fprintf(f, "print_str%d db %s, 0xa\n", count, val->value);
      } else if (val->type == AST_NUMBER) {
        fprintf(f, "print_str%d db \"%s\", 0xa\n", count, val->value);
      } else if (val->type == AST_VARIABLE) {
        ASTNode *assignVar = findVarAssign(program, val->value);
        if (!assignVar) {
          printf("couldnt grab variable value for %s", val->value);
        }
        char *type = assignVar->left->dataType;
        char *varValue = assignVar->right->value;
        // printf("%s, %s", type, varValue);
        if (strcmp(type, "int") == 0) {
          fprintf(f, "print_str%d db \"%s\", 0xa\n", count, varValue);
        } else {
          fprintf(f, "print_str%d db %s, 0xa\n", count, varValue);
        }

        assignVar = NULL;
      }
      fprintf(f, "print_len%d equ $ - print_str%d\n", count, count);
      count++;
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

      static int count2 = 0;
      fprintf(f,
              "    mov rax, 1        ; sys_write\n"
              "    mov rdi, 1        ; stdout\n"
              "    mov rsi, print_str%d ; address of string\n"
              "    mov rdx, print_len%d ; length\n"
              "    syscall\n",
              count2, count2);
      count2++;
    }
  }

  // Exit syscall
  fprintf(f, "\n    mov rax, 60     ; sys_exit\n"
             "    xor rdi, rdi\n"
             "    syscall\n");

  fclose(f);
}
