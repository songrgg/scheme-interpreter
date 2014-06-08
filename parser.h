#ifndef __PARSER_H__
#define __PARSER_H__
#include "globals.h"

typedef struct node {
	char* child_e;
	struct node* child_n;
	struct node* next;
} e_node;

void match(TokenType token,BOOLEAN ifneed_next);
e_node* init_node(void);
e_node* parser(void);
e_node* stmt(void);
e_node* id(void);
e_node* num(void);
e_node* application(void);

#endif

