#include "lexer.h"
#include "token.h"


void pushCharacter(TokenList *root, TokenType type, char val){
	char temp[2] = {val, '\0'};
	push_back(root, (Token){type,temp});
}

void pushString(TokenList *root, TokenType type, char *val){
	push_back(root,(Token){type,val});
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
//++++++++++++++++++++++      TOKEN TYPES     ++++++++++++++++++++=
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=

bool isNumber(char thing){
	return (thing == '0' || thing == '1' || thing == '2' || thing == '3' || thing == '4' || thing == '5' || thing == '6' || thing == '7' || thing == '8' || thing == '9');
}

bool isKeyword(char *thing){
	if (!strcmp(thing,"if") || !strcmp(thing,"else") || !strcmp(thing,"for") || !strcmp(thing,"void")) return true;
	return false;
}

bool isOperator(char thing){
	return (thing == '+' || thing == '-' || thing == '/' || thing == '*' || thing == '%' || thing == '=');
}

bool isConstant(char *thing){
	for (int i =0; i<strlen(thing);i++){
		if (!isNumber(thing[i]))return false;
	}
	return true;
}

bool isDataType(char *thing){
	if (!strcmp(thing,"int") || !strcmp(thing,"float") || !strcmp(thing,"string") || !strcmp(thing,"char") || !strcmp(thing,"double"))
		return true;
	return false;
}

bool isSemiColon(char thing){
	return thing == ';';
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=
//++++++++++++++++++++++      PARSE CODE      ++++++++++++++++++++=
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=

char* subString(const char* str, int left, int right){
	if (left > right){
		printf("left side can't be bigger than right\n");
		return NULL;
	}
	
	int len = right - left;
	char *subStr = malloc(len+1); //allocate in the memory the length needed + 1 for the terminator \0
	for (int i =0; i<len; left++, i++) subStr[i] = str[left];
	subStr[len] = '\0';
	return subStr;
}


TokenList parse_code(const char *str){
	int left,right;
	TokenList *tokens = malloc(sizeof(TokenList)); // declare a linked list of tokens

	left = 0;
	right = left;
	while(str[left]!= '\0'){
		
		if (str[left] == '\0') break;
	

		//check characters first
		if (isOperator(str[left])){
			printf("%c is an operator\n", str[left]);
			pushCharacter(tokens,OPERATOR,str[left]);
			left++;
		}
		else if (isSemiColon(str[left])) {
			printf("%c is semicolon\n", str[left]);
			pushCharacter(tokens,SPECIAL,str[left]);
			left++;
		}
		else if (str[left] == ' ') while(str[left] == ' ') left++;
		else{
			// if the characters dont work check string
			right = left;
			while (str[right] != '\0' && str[right] != ' ' && !isOperator(str[right]) && !isSemiColon(str[right]))
				right++;
				
			char *token = subString(str,left,right);
			if (isKeyword(token)){
				printf("%s is a keyword\n",token);
				pushString(tokens,KEYWORD,token);
			}
			else if (isConstant(token)){
				printf("%s is a constant\n",token);
				pushString(tokens,CONSTANT,token);
			}
			else if (isDataType(token)){
				printf("%s is a data type\n",token);
				pushString(tokens,DATATYPE,token);
			}
			else {
				//if all else doesn't work it must be a variable
				printf("%s is a variable\n",token);
				pushString(tokens,VARIABLE,token);
			}
			free(token);
			token=NULL;
			left = right;
		}
		
	}
	tokens = tokens->next; // the first token is NULL.
	return *tokens;
}


int main(){
	
	char *code = "int kilo = 53;";
	parse_code(code);
	return 0;
}
