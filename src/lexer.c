#include "lexer.h"

void pushCharacter(TokenList *root, TokenType type, char val) {
  char temp[2] = {val, '\0'};
  push_back(root, (Token){type, temp});
}

void pushString(TokenList *root, TokenType type, char *val) {
  push_back(root, (Token){type, val});
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
//++++++++++++++++++++++      TOKEN TYPES     ++++++++++++++++++++=
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=

bool isNumber(char thing) {
  return (thing == '0' || thing == '1' || thing == '2' || thing == '3' ||
          thing == '4' || thing == '5' || thing == '6' || thing == '7' ||
          thing == '8' || thing == '9');
}

bool isKeyword(char *thing) {
  if (!strcmp(thing, "if") || !strcmp(thing, "else") || !strcmp(thing, "for") ||
      !strcmp(thing, "void"))
    return true;
  return false;
}

bool isOperator(char thing) {
  return (thing == '+' || thing == '-' || thing == '/' || thing == '*' ||
          thing == '%' || thing == '=');
}

// TODO HANDLE STRINGS

bool isConstant(char *thing) {
  if (thing[0] == '\"') {
    // check if string ends at the end and not abruptly

    for (int i = 1; i < strlen(thing) - 1; i++) {
      if (thing[i] == '\"')
        return false;
    }
  }
  // if not string then check if it's a number
  else {
    for (int i = 0; i < strlen(thing); i++) {
      // if there isn't a number in the middle it's not a valid constant
      if (!isNumber(thing[i]))
        return false;
    }
  }

  return true;
}

bool isDataType(char *thing) {
  if (!strcmp(thing, "int") || !strcmp(thing, "float") ||
      !strcmp(thing, "string") || !strcmp(thing, "char") ||
      !strcmp(thing, "double"))
    return true;
  return false;
}

bool isSemiColon(char thing) { return thing == ';'; }

bool isUSELESS(char thing) { return (thing == '\n' || thing == ' '); }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
//++++++++++++++++++++++      PARSE CODE      ++++++++++++++++++++=
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=

char *subString(const char *str, int left, int right) {
  if (left > right) {
    printf("left side can't be bigger than right\n");
    return NULL;
  }

  int len = right - left;
  char *subStr = malloc(len + 1);
  for (int i = 0; i < len; left++, i++)
    subStr[i] = str[left];
  subStr[len] = '\0';
  if (subStr)
    return subStr;
  return NULL;
}

TokenList parse_code(const char *str) {
  int left, right;
  TokenList *tokens =
      malloc(sizeof(TokenList)); // declare a linked list of tokens

  left = 0;
  right = left;
  while (str[left] != '\0') {

    // check characters first
    if (isOperator(str[left])) {
      printf("%c is an operator\n", str[left]);
      pushCharacter(tokens, OPERATOR, str[left]);
      left++;
    } else if (isSemiColon(str[left])) {
      printf("%c is semicolon\n", str[left]);
      pushCharacter(tokens, SPECIAL, str[left]);
      left++;
    }
    // if it's empty space or new line skip it until something else appears
    else if (isUSELESS(str[left]))
      while (isUSELESS(str[left]))
        left++;

    else {
      // if the characters dont work check string
      right = left;
      while (str[right] != '\0' && str[right] != ' ' &&
             !isOperator(str[right]) && !isSemiColon(str[right]))
        right++;

      char *token = subString(str, left, right);
      if (isKeyword(token)) {
        printf("%s is a keyword\n", token);
        pushString(tokens, KEYWORD, token);
      } else if (isConstant(token)) {
        printf("%s is a constant\n", token);
        pushString(tokens, CONSTANT, token);
      } else if (isDataType(token)) {
        printf("%s is a data type\n", token);
        pushString(tokens, DATATYPE, token);
      } else {
        // if all else doesn't work it must be a variable
        printf("%s is a variable\n", token);
        pushString(tokens, VARIABLE, token);
      }
      free(token);
      token = NULL;
      left = right;
    }
  }
  tokens = tokens->next; // the first token is NULL.
  return *tokens;
}

int main() {

  char *code = "string name = \"kilo\";\nint isPushing = 1;";
  parse_code(code);
  return 0;
}
