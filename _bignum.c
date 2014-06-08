#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"_bignum.h"
#include "_memory.h"

typedef enum {
	FRACTION, INTEGER, DECIMAL
} numType;

char* _frac_add(char* num1, char* num2)
{
	return NULL;
}

static void reverse(char* str)
{
	int len = strlen(str);
	int i,j;
	for(i=0,j=len-1; i<j; i++,j--) {
		char tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}
}

char* _int_add(char* num1, char* num2)
{
	int num1_len = strlen(num1);
	int num2_len = strlen(num2);
	int num_len = num1_len>num2_len?num1_len:num2_len;
	int i,j,k,s,a,b,c;
	char* sum = (char*)_alloc(num_len+2);
	for (i=num1_len-1,j=num2_len-1,c=k=0; i>=0 || j>=0 || c; i--,j--) {
		if (i>=0)
			a = num1[i] - '0';
		else
			a = 0;
		if (j>=0)
			b = num2[j] - '0';
		else
			b = 0;
		s = a + b + c;
		c = s/10;
		sum[k++] = s%10 + '0';
	}
	sum[k] = 0;
	reverse(sum);
	return sum;
}

char* _int_minus(char* num1, char* num2)
{
	return NULL;
}
