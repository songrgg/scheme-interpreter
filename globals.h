#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define MAXLINE 8192
typedef enum {
	ID, NUM, LPAREN, RPAREN, DEFINE, LAMBDA, CONS, CAR, CDR, NEWLINE, STRING, EndOfFile
} TokenType;

typedef struct key {
	TokenType tt;
	char *name;
} Key;

typedef enum {
	START, END, INID, INNUM, INSTR
} State;

typedef enum { TRUE, FALSE } BOOLEAN;


#endif
