#ifndef __UTIL_H__
#define __UTIL_H__
#ifdef __UTIL_C__
#undef __UTIL_C__
#define EXTERN
#else
#define EXTERN extern
#endif
#include "globals.h"
#define KEYNUM 7

Key* keylookup(char *name);
int isNum(char* str);
int isStr(char* str);

#undef EXTERN
#endif
