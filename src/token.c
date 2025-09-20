#include "token.h"


void push_back(TokenList *root,Token val){
	TokenList *newNode = malloc(sizeof(TokenList));
	newNode->val = val;
	newNode->next = NULL;

	TokenList *temp = root;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = newNode;
}	
