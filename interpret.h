#ifndef __INTERPRET_H__
#define __INTERPRET_H__

#include "parser.h"
#include "hash.h"
#include <stdio.h>

enum {
    I_LAMBDA=1, I_EXPR, I_DEFINE, I_COND, I_NUM, I_SYS_OP, I_BOOL_TRUE,
    I_BOOL_FALSE, I_INTER_USE, I_PAIR, I_STRING
};

#define max(a,b) (a)>(b)?(a):(b)

#define PARA_UNLIMITED -1 /*parameter with no limits*/
#define PARA_ZERO       0 /*no parameter needed*/
typedef i_node* FUNPTR(e_node* para_list,Env* env);

i_node* _interpret(e_node* top,Env* env);
void insert(char* key,i_node* value,Env* env);
int lookup(char* key,Value* value,Env* env);
Env* new_env(int size);
void del_env(Env* env);

i_node* _sys_proc(void* ptr_proc,int para_cnt,char** para_list);
void sch_init(void);
int isNum(char* str);
i_node* _num_i_node(char* str_num);
i_node* _lambda_i_node(e_node* para_list,e_node* fun_body);
i_node* _pair_i_node(i_node* left,i_node* right);
i_node* _str_i_node(char* str);
i_node* _proc_execute(i_node* op,e_node* para_list,Env* env);
i_node* _lambda_execute(i_node* op,e_node* para_list,Env* env);
i_node* _if_execute(e_node* para_list,Env* env);
i_node* _define_execute(e_node* para_list,Env* env);

void _analyse_output(i_node* result);
void print_pair(i_node* pair);

/*system basic function*/
i_node* _sys_proc_add(e_node* para_list,Env* env);
i_node* _sys_proc_div(e_node* para_list,Env* env);
i_node* _sys_proc_mul(e_node* para_list,Env* env);
i_node* _sys_proc_minus(e_node* para_list,Env* env);
i_node* _sys_proc_eq(e_node* para_list,Env* env);
i_node* _sys_proc_gt(e_node* para_list,Env* env);
i_node* _sys_proc_lt(e_node* para_list,Env* env);
i_node* _sys_proc_cons(e_node* para_list,Env* env);
i_node* _sys_proc_car(e_node* para_list,Env* env);
i_node* _sys_proc_cdr(e_node* para_list,Env* env);
i_node* _sys_proc_disp(e_node* para_list,Env* env);
i_node* _sys_proc_set(e_node* para_list,Env* env);

Env* cur_env;
#ifdef __INTER_C__
i_node* _i_node_add   = NULL;
i_node* _i_node_div   = NULL;
i_node* _i_node_minus = NULL;
i_node* _i_node_mul   = NULL;
i_node* _i_node_eq    = NULL;  /* identifier = */
i_node* _i_node_gt    = NULL;
i_node* _i_node_lt    = NULL;
i_node* _i_node_true  = NULL;
i_node* _i_node_false = NULL;
i_node* _i_node_cons  = NULL;
i_node* _i_node_car   = NULL;
i_node* _i_node_cdr   = NULL;
i_node* _i_node_null  = (i_node*)1;

i_node* _i_node_disp  = NULL;
i_node* _i_node_void  = (i_node*)2;
i_node* _i_node_set   = NULL;

#else
i_node* _i_node_add;
i_node* _i_node_div;
i_node* _i_node_minus;
i_node* _i_node_mul;
i_node* _i_node_eq;
i_node* _i_node_gt;
i_node* _i_node_lt;
i_node* _i_node_true;
i_node* _i_node_false;
i_node* _i_node_cons;
i_node* _i_node_car;
i_node* _i_node_cdr;
i_node* _i_node_null;
i_node* _i_node_disp;
i_node* _i_node_void;
i_node* _i_node_set;

#endif

#endif
