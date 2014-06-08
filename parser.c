#include "_memory.h"
#include "parser.h"
#include "globals.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

TokenType curToken;
int level=0;

void getNextToken(void)
{
    int i;
    while ((curToken=getToken())==NEWLINE) {
        if (level == 0) continue;
        for (i=0; i<level+5; i++) {
            printf(" ");
        }
    }
    //fprintf(stderr,"CURTOKEN: %d\n",curToken);
}

void match(TokenType token,BOOLEAN ifneed_next)
{
	if (curToken==token) {
		//fprintf(stderr,"MATCH %d\n",token);
        if (ifneed_next==TRUE) getNextToken();
	}
	else {
		fprintf(stderr,"Token unmatch\n");
		exit(0);
	}
}

e_node* init_node(void)
{
	e_node* top=(e_node*)_alloc(sizeof(e_node));
	top->child_e=NULL;
	top->child_n=NULL;
	top->next=NULL;
	return top;
}

e_node* parser(void)
{
    level=0;
    getNextToken();
    return stmt();
}

e_node* stmt(void)
{
	e_node* top;
	if(curToken==LPAREN) {
        level++;
		match(LPAREN,TRUE);
		top=(e_node*)_alloc(sizeof(e_node));
		e_node* child_n=application();
		top->next=child_n;
		e_node* para_list=child_n;
		while(curToken!=RPAREN) {
            //fprintf(stderr,"curToken: %d\n",curToken);
			para_list->child_n=stmt();
			para_list=para_list->child_n;
		}
        level--;
        if(level) match(RPAREN,TRUE);
        else      match(RPAREN,FALSE);
		if (para_list) {
		  para_list->child_n=NULL;	
		}
	}
	else if(curToken==NUM) {
		//fprintf(stderr,"Num\n");
		top=num();
	}
	else if(curToken==ID) {
		//fprintf(stderr,"Id\n");
		top=id();
	}
	else if(curToken==STRING) {
		top=init_node();
		top->child_e = strdup(TokenString);
		if (level) match(STRING,TRUE );
		else       match(STRING,FALSE);
	}
	else if(curToken==EndOfFile) {
		top=NULL;
	}
	else {
		//fprintf(stderr,"Error\n");
		fprintf(stderr,"stmt unmatch\n");
		return NULL;
	}
	return top;
}

e_node* id(void)
{
	e_node* top=init_node();
	int len=strlen(TokenString);
	top->child_e=_alloc(len+1);
	strcpy(top->child_e,TokenString);
    //fprintf(stderr,"LEVEL: %d\n",level);
	if(level) match(ID,TRUE );
    else      match(ID,FALSE);
	return top;
}

e_node* num(void)
{
	e_node* top=init_node();
	int len=strlen(TokenString);
	top->child_e=_alloc(len+1);
	strcpy(top->child_e,TokenString);
	if(level) match(NUM,TRUE );
    else      match(NUM,FALSE);
	return top;
}

e_node* application(void)
{
	e_node* top;
    //fprintf(stderr,"curToken: %d\n",curToken);
	if(curToken==ID) {
		return id();
	}
	else if(curToken==LPAREN) {
        level++;
		match(LPAREN,TRUE);
		top=(e_node*)_alloc(sizeof(e_node));
		e_node* child_n=application();
		top->next=child_n;
		e_node* para_list=child_n;
		while(curToken!=RPAREN) {
			para_list->child_n=stmt();
			para_list=para_list->child_n;
		}
        level--;
        if(level) match(RPAREN,TRUE );
        else      match(RPAREN,FALSE);
		para_list->child_n=NULL;
	} else if(curToken==RPAREN) {
		top=NULL;
	} else {
		fprintf(stderr, "Application define error\n");
	}
	return top;
}

void print_node(e_node* top)
{
	e_node* v=top;
	if(v==NULL) return;
	if(v->next!=NULL) {
		print_node(v->next);
	}
	if(v->child_e)
		fprintf(stderr,"%s ",v->child_e);
	if(v->child_n)
		print_node(v->child_n);
}
