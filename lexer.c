#define __LEXER_C__
#include "globals.h"
#include "lexer.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char buf[MAXLINE];
static int bufIndex = 0;
static int bufSize = 0;


char getch(void)
{
	if (bufIndex == bufSize) {
		if ((fgets(buf, MAXLINE, stdin)) == NULL) {
			return 0;
		}
		bufIndex = 0;
		bufSize = strlen(buf);
	}
	return buf[bufIndex++];
}

void ungetch(void)
{
	bufIndex--;
}

TokenType getToken(void)
{
	TokenType curToken;
	State state = START;
	TokenStringIndex=0;
	char ch = getch();
	if(ch == 0){
		return EndOfFile;
	}

	while (state != END) {
		switch (state) {
			case START:
				if(ch==' '||ch=='\t'){
					ch=getch();
					state=START;
				}
				else if(ch=='"'){
					TokenString[TokenStringIndex++]=ch;
					state=INSTR;
				}
				else if(ch=='\n'){
					lineno++;
                    state=END;
                    curToken=NEWLINE;
				}
				else if(ch=='('){
					state=END;
					curToken=LPAREN;
				}
				else if(ch==')'){
					state=END;
					curToken=RPAREN;
				}
				else if(isdigit(ch)){
					TokenString[TokenStringIndex++]=ch;
					state=INNUM;
				}
				else{
					TokenString[TokenStringIndex++]=ch;
					state=INID;
				}
				break;
			case INNUM:
				ch = getch();
				if(!isdigit(ch)){
					if(ch==' '||ch=='\t'||ch==0){
						state=END;
						curToken=NUM;
					}
					else if(ch=='('||ch==')'){
						state=END;
						ungetch();
						curToken=NUM;
					}
					else if(ch=='\n'){
						state=END;
                        ungetch();
						curToken=NUM;
					}else{
						state=INID;
						TokenString[TokenStringIndex++]=ch;
					}
				} else
					TokenString[TokenStringIndex++]=ch;
				break;
			case INSTR:
				ch = getch();
				if(ch=='"'){
					TokenString[TokenStringIndex++]=ch;
					curToken = STRING;
					state=END;
				}
				else{
					TokenString[TokenStringIndex++]=ch;
					state=INSTR;
				}
				break;
			case INID:
				ch = getch();
				if(ch==' '||ch=='\t'||ch==0){
					state=END;
					curToken=ID;
				}
				else if(ch=='('||ch==')'){
					state=END;
					ungetch();
					curToken=ID;
				}
				else if(ch=='\n'){
					state=END;
					curToken=ID;
					ungetch();
				}
				else{
					TokenString[TokenStringIndex++]=ch;
				}
				break;
		}
	}
	TokenString[TokenStringIndex]=0;
#if 0
	if(curToken==ID||curToken==NUM)
		fprintf(stderr,"%s\n",TokenString);
	else if(curToken==LPAREN)
		fprintf(stderr,"(\n");
	else if(curToken==RPAREN)
		fprintf(stderr,")\n");
	if(curToken==ID){
		Key* found;
		if((found=keylookup(TokenString))){
			return found->tt;
		}
	}
#endif
	return curToken;
}
