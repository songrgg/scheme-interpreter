#ifndef __LEXER_H__
#define __LEXER_h__

#ifdef __LEXER_C__
#undef __LEXER_C__
#define EXTERN 
#else
#define EXTERN extern
#endif
#include "globals.h"

EXTERN int lineno;
EXTERN char TokenString[8192];
EXTERN int TokenStringIndex;

char getch(void);
void ungetch(void);
TokenType getToken(void);

#undef EXTERN
#endif
