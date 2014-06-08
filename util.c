#define __UTIL_C__
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "globals.h"

Key keyword[KEYNUM] = {
	{ CAR,    "car"    },
	{ CDR,    "cdr"    },
	{ CONS,   "cons"   },
	{ DEFINE, "define" },
	{ LAMBDA, "lambda" },
	{ LPAREN, "("      },
	{ RPAREN, ")"      }
};

Key *keylookup(char *name)
{	
	int i;
	for (i = 0; i<KEYNUM; i++) {
		if (strcmp(keyword[i].name, name) == 0) {
			return keyword+i;
		}
	}
	return NULL;
}

int isNum(char* str)
{
    int i,cnt;
    for (i=0,cnt=0; str[i]; i++) {
        if (!cnt && str[i]=='.') {
            cnt=1;
        }
        if (str[i]<'0' || str[i]>'9' || cnt > 1)
            return 0;
    }
    return 1;
}

int isStr(char* str)
{
	return str[0]=='"' && str[strlen(str)-1]=='"';
}