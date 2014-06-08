#define __INTER_C__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "globals.h"
#include "parser.h"
#include "_memory.h"
#include "interpret.h"
#include "_queue.h"

#define DEBUG 1

i_node* _sys_proc(void* ptr_proc,int para_cnt,char** para_list)
{
    i_node* i_proc = (i_node*)malloc(sizeof(i_node));
    i_proc->type = I_SYS_OP;
    i_proc->data = ptr_proc;
    i_proc->para_cnt = para_cnt;
    i_proc->para_list = para_list;
    return i_proc;
}

void sch_init(void)
{
    cur_env = (Env*)init_hashtable(0);
    _i_node_add    = _sys_proc((void*)_sys_proc_add  , PARA_UNLIMITED, NULL);
    _i_node_div    = _sys_proc((void*)_sys_proc_div  , PARA_UNLIMITED, NULL);
    _i_node_mul    = _sys_proc((void*)_sys_proc_mul  , PARA_UNLIMITED, NULL);
    _i_node_minus  = _sys_proc((void*)_sys_proc_minus, PARA_UNLIMITED, NULL);
    _i_node_eq     = _sys_proc((void*)_sys_proc_eq   , PARA_UNLIMITED, NULL);
    _i_node_gt     = _sys_proc((void*)_sys_proc_gt   , PARA_UNLIMITED, NULL);
    _i_node_lt     = _sys_proc((void*)_sys_proc_lt   , PARA_UNLIMITED, NULL);
    
    /*register the system function in the environment*/
    insert("+",_i_node_add  ,cur_env);
    insert("-",_i_node_minus,cur_env);
    insert("*",_i_node_mul  ,cur_env);
    insert("/",_i_node_div  ,cur_env);
    insert("=",_i_node_eq   ,cur_env);
    insert(">",_i_node_gt   ,cur_env);
    insert("<",_i_node_lt   ,cur_env);

    _i_node_cons    = _sys_proc((void*)_sys_proc_cons , 2, NULL);
    _i_node_car     = _sys_proc((void*)_sys_proc_car  , 1, NULL);
    _i_node_cdr     = _sys_proc((void*)_sys_proc_cdr  , 1, NULL);

	/*cons,car,cdr*/
    insert("cons",_i_node_cons,cur_env);
    insert("car",_i_node_car,cur_env);
	insert("cdr",_i_node_cdr,cur_env);
	
	/*empty list*/
	insert("nil",_i_node_null,cur_env);

	_i_node_disp = _sys_proc((void*)_sys_proc_disp, 1, NULL);
	/*display*/
	insert("display",_i_node_disp,cur_env);
	
	/*set! family*/
	_i_node_set       = _sys_proc((void*)_sys_proc_set  , 2, NULL);
	insert("set!", _i_node_set, cur_env);
    
    _i_node_true = (i_node*)malloc(sizeof(i_node));
    _i_node_true->type = I_BOOL_TRUE;
    _i_node_false= (i_node*)malloc(sizeof(i_node));
    _i_node_false->type = I_BOOL_FALSE;
}

void insert(char* key,i_node* value,Env* env)
{
    put_hash(env,key,value);
}

int lookup(char* key,Value* value,Env* env)
{
    return get_hash(env,key,value);
}

Env* new_env(int size)
{
    return init_hashtable(size);
}

void del_env(Env* env)
{
    drop_hashtable((hashtable*)env);
}

i_node* _str_i_node(char* str)
{
	i_node* str_node = (i_node*)_alloc(sizeof(i_node));
	str_node->type = I_STRING;
	str_node->data = (void*)strdup(str);
	return str_node;
}

i_node* _pair_i_node(i_node* left,i_node* right)
{
	i_node* lp = (i_node*)_alloc(sizeof(i_node));
	lp->type = I_PAIR;
	lp->data = (void*)left;
	lp->next = right;
	return lp;
}

i_node* _num_i_node(char* str_num)
{
    i_node* num_node = (i_node*)_alloc(sizeof(i_node));
    num_node->type = I_NUM;
    num_node->data = (void*)strdup(str_num);
    return num_node;
}

/* 
 * two parameters: the first is the list of the parameter names, 
 * the second is the function body
 */
i_node* _lambda_i_node(e_node* para_list,e_node* fun_body)
{
    i_node* lambda_node = (i_node*)_alloc(sizeof(i_node));
    lambda_node->type = I_LAMBDA;
    int i;
    e_node* iter;
    for (i=0,iter=para_list->next; iter; iter=iter->child_n,i++);
    lambda_node->para_cnt = i;
    lambda_node->para_list = (char**)_alloc(i*sizeof(char*));

#if DEBUG
		printf("Before parameters %d\n", i);
#endif

		for (i=0,iter=para_list->next; iter!=NULL; iter=iter->child_n) {
#if DEBUG
        printf("Parameter %d: %s\n",i,iter->child_e);
#endif
        lambda_node->para_list[i++] = strdup(iter->child_e);
    }
    lambda_node->data = (void*)fun_body;
#if DEBUG
		fprintf(stderr, "func_body: %s\n", fun_body->child_e);
#endif
	lambda_node->next = NULL;
    return lambda_node;
}

i_node* _proc_execute(i_node* op,e_node* para_list,Env* env)
{
    int cnt = _para_num(para_list);
    if (op->para_cnt == PARA_UNLIMITED) { /*do nothing*/ }
    else if (cnt < op->para_cnt) {
        printf("PROC: Lack of parameter\n");
        return NULL;
    } else if (cnt > op->para_cnt) {
        printf("PROC: Too many parameters\n");
        return NULL;
    }
    return ((FUNPTR*)op->data)(para_list,env);
}

Env* _cpy_env(Env* e)
{
	Env* cpy = (Env*)_alloc(sizeof(Env));
	cpy->capacity = e->capacity;
	cpy->size = e->size;
	cpy->load_factor = e->load_factor;
	cpy->table = e->table;
	cpy->next = NULL;
	return cpy;
}

i_node* _cpy_i_node(i_node* n)
{
	i_node* cpy = (i_node*)_alloc(sizeof(Env));
	cpy->type = n->type;
	cpy->para_cnt = n->para_cnt;
	cpy->para_list = n->para_list;
	if (cpy->type == I_NUM) {
		return _num_i_node((char*)n->data);
	}
	return cpy;
}

i_node* _lambda_execute(i_node* op,e_node* para_list,Env* env)
{
    int i;
    e_node* iter;
    Env* nenv = new_env(10);

#if 0
	if (op->next!=NULL) {
		//Env* e = _cpy_env((Env*)op->next->data);
		//e->next = env;
		nenv->next = env;
	}
	else
		nenv->next = env;
#endif

	i = _para_num(para_list);
    if (i > op->para_cnt) {
        printf("LAMBDA: Too many parameters\n");
        return NULL;
    }
    else if (i < op->para_cnt) {
        printf("LAMBDA: Lack of parameter\n");
        return NULL;
    }
	i_node* t;
	
    for (i=0,iter=para_list; i<op->para_cnt; i++,iter=iter->child_n) {
#if DEBUG
		printf(">>>>>>>>>>>%s ",op->para_list[i]);
		print_node(iter);
		printf("%s's value: %s\n",op->para_list[i],(char*)(_interpret(iter,env))->data);
#endif
        insert(op->para_list[i],_interpret(iter,env),nenv);
    }
#if DEBUG
    printf("lambda_execute:  ");
	print_node((e_node*)op->data);
#endif
	
	if (op->next!=NULL) {
		Env* e = _cpy_env((Env*)op->next->data);
		e->next = env;
		nenv->next = e;
	}
	else
		nenv->next = env; /*put_hash is an operation that put the value
						to the single-level env,if it's found that
					    the value exists in the previous level
					    it will not be put,so this operation
					    should be laid back**************/
    i_node* ret = _interpret((e_node*)op->data,nenv);
	if (ret->type == I_LAMBDA) {
		ret->next = (i_node*)_alloc(sizeof(i_node));
		ret->next->data = nenv;
	}
    return ret;
}

e_node* _tolambda(e_node* node)
{
    e_node* ret = (e_node*)_alloc(sizeof(e_node));
    ret->child_e = NULL;
    ret->next = (e_node*)_alloc(sizeof(e_node));
    ret->next->child_e = strdup("lambda");
    ret->next->child_n = (e_node*)_alloc(sizeof(e_node));
	ret->next->child_n->child_e = NULL;
    ret->next->child_n->next = node->next->child_n;
    ret->next->child_n->child_n = node->child_n;
    return ret;
}

Env* _save_env(e_node* top,Env* env)
{
	Env* saved_env = new_env(2);
	queue* q = init_queue();
	push_queue(top,q);
	while (!empty_queue(q)) {
		e_node* x = top_queue(q);
		i_node* val;
		if (x->child_e && x->next==NULL) {
			if (lookup(x->child_e,&val,env)==0) {
				insert(x->child_e,val,saved_env);
#if DEBUG
				printf("SAVING %s\n",x->child_e);
#endif
			}
		}
		if (x->child_n)
			push_queue(x->child_n,q);
		if (x->next)
			push_queue(x->next,q);
	}
	_dealloc(q);
	return saved_env;
}

i_node* _define_execute(e_node* para_list,Env* env)
{
    int i;
    e_node* iter;

    for (i=0,iter=para_list; iter; iter=iter->child_n,i++);
    if (i>2) {
        printf("DEFINE: Too many parameters\n");
        return NULL;
    }
    else if (i<2) {
        printf("DEFINE: Lack of parameter\n");
        return NULL;
    }
    if (para_list->child_e) {
        if (para_list->child_n->child_e) { /*define number or id*/
            i_node* num_node;
            if (isNum(para_list->child_n->child_e)) {  /*number*/
                i_node* num_node = _num_i_node(para_list->child_n->child_e);
                insert(para_list->child_e,num_node,env);
            }
            else {  /*id*/
                i_node* id_node;
                if (lookup(para_list->child_n->child_e,&id_node,env)) {
                    printf("%s can't be found\n",
                           para_list->child_n->child_e);
                    return NULL;
                }
                insert(para_list->child_e,id_node,env);
            }
        }
        else if (para_list->child_n->next->child_e) {
			if (!strcmp(para_list->child_n->next->child_e,"lambda")) {
				i_node* tmp;
				Env* saved_env;
				insert(para_list->child_e,
						tmp=_interpret(para_list->child_n,env),
						env);
				saved_env = _save_env(para_list->child_n,env);
				tmp->next = (i_node*)_alloc(sizeof(i_node));
				tmp->next->data = (void*)saved_env;
			}
			else {
				insert(para_list->child_e,
						_interpret(para_list->child_n,env),
						env);
			}
		}
	}
	else { /*merely for defining function like (define (f a) (+ a 1))*/
		/*transfer (define (f a) (+ a 1)) -> (define f (lambda (a) (+ a 1))) */
		e_node* lambda_node = (e_node*)_tolambda(para_list);
		i_node* tmp;
		Env* saved_env;
#if 0
		insert(para_list->next->child_e,
				tmp=_interpret(lambda_node,cur_env),
				cur_env);
#endif
		insert(para_list->next->child_e,
				tmp=_lambda_i_node(lambda_node->next->child_n,para_list->child_n),
				env);
		saved_env = _save_env(lambda_node,env);
		tmp->next = (i_node*)_alloc(sizeof(i_node));
		tmp->next->data = (void*)saved_env; /*save the needed env*/
	}
	return NULL;
}

int _para_num(e_node* para_list)
{
	e_node* iter;
	int cnt = 0;
	for (iter=para_list; iter; iter=iter->child_n)
		cnt++;
	return cnt;
}

i_node* _if_execute(e_node* para_list,Env* env)
{
	int para_num = _para_num(para_list);
	if (para_num > 3) {
		printf("IF: Too many parameters\n");
		return NULL;
	}
	else if (para_num < 3) {
		printf("IF: Lack of parameter\n");
		return NULL;
	}
	i_node* true_or_false = _interpret(para_list,env);
	if (true_or_false == NULL)
		printf("IF: can't judge true or false\n");
#if 0
	if (true_or_false == NULL || true_stmt == NULL || false_stmt == NULL) {
		printf("Bad syntax\n");
		return NULL;
	}
#endif
	if (true_or_false == _i_node_true) {
		return _interpret(para_list->child_n,env);
	}
	else {
		return _interpret(para_list->child_n->child_n,env);
	}
}

i_node* _cond_execute(e_node* para_list,Env* env)
{
	int para_num = _para_num(para_list);
	if (para_num < 1) {
		printf("COND: one parameter at least\n");
		return NULL;
	}
	e_node* iter;
	for (iter=para_list; iter; iter=iter->child_n) {
		if (iter->next->child_e && !strcmp(iter->next->child_e,"else")) {
			if (iter->child_n != NULL) {
				printf("cond: bad syntax (`else' clause must be last)\n");
				return NULL;
			}
			return _interpret(iter->next->child_n,env);
		}
		i_node* cond = _interpret(iter->next,env);
		if (cond == _i_node_true)
			return _interpret(iter->next->child_n,env);
	}
	return NULL;
}

i_node* _interpret(e_node* top,Env* env)
{
	i_node* ret = NULL;
	if (top->child_e!=NULL && top->next==NULL) {
#if DEBUG
		printf("top->child_e->%s\n",top->child_e);
#endif
		if (isNum(top->child_e)) {
			return _num_i_node(top->child_e);
		}
		else if (isStr(top->child_e)) {
			return _str_i_node(top->child_e);
		}
		else if (!strcmp(top->child_e,"#t")) {
			return _i_node_true;
		}
		else if (!strcmp(top->child_e,"#f")) {
			return _i_node_false;
		}
		else {
			if (lookup(top->child_e,&ret,env) == 0)
				return ret;
			else {
				printf("Unknown identifier\n");
				return NULL;
			}
		}
	} else {
		if (top->next->child_e != NULL) {
			if (!strcmp(top->next->child_e,"lambda"))
			{
#if DEBUG
				printf("LAMBDA RECEIVED\n");
#endif
				return _lambda_i_node(top->next->child_n,
						top->next->child_n->child_n);
			}
			else if (!strcmp(top->next->child_e,"define"))
			{
#if DEBUG
				printf("DEFINE RECEIVED\n");
#endif
				return _define_execute(top->next->child_n,env);
			}
			else if (!strcmp(top->next->child_e,"if"))
			{
#if DEBUG
				printf("IF RECEIVED\n");
#endif
				return _if_execute(top->next->child_n,env);
			}
			else if (!strcmp(top->next->child_e,"cond"))
			{
#if DEBUG
				printf("COND RECEIVED\n");
#endif

				return _cond_execute(top->next->child_n,env);
			}
		}
		i_node* op = _interpret(top->next,env);
		if (op == NULL) {
			printf("could't find op\n");
			return NULL;
		}

		if (op->type == I_SYS_OP) {
			if (op == _i_node_add    ||
					op == _i_node_div    ||
					op == _i_node_minus  ||
					op == _i_node_mul    ||
					op == _i_node_eq     ||
					op == _i_node_gt     ||
					op == _i_node_lt     ||
					op == _i_node_cons   ||
					op == _i_node_car    ||
					op == _i_node_cdr    ||
					op == _i_node_disp   ||
					op == _i_node_set) {
				ret = _proc_execute(op,top->next->child_n,env);
			}
		}
		else if (op->type == I_LAMBDA) {
#if DEBUG
			printf("LAMBDA I NODE\n");
#endif
			ret = _lambda_execute(op,top->next->child_n,env);
		}
	}
	return ret;
}

void print_pair(i_node* pair)
{
	i_node* lp_val = (i_node*)pair->data;
	i_node* rp_val = (i_node*)pair->next;
	printf("(");
	_analyse_output(lp_val);
	printf(" ");
	_analyse_output(rp_val);
	printf(")");
}

void _analyse_output(i_node* result)
{
	if (result == _i_node_void) {
	} else if (result == _i_node_null) {
		printf("null");
	} else if (result == NULL) {
		printf("nil");
	}
	else if (result->type == I_NUM) {
		printf("%s",(char*)result->data);
	}
	else if (result->type == I_STRING) {
		printf("%s",(char*)result->data);
	}
	else if (result->type == I_BOOL_TRUE) {
		printf("#t");
	}
	else if (result->type == I_BOOL_FALSE) {
		printf("#f");
	}
	else if (result->type == I_SYS_OP) {
		if (result ==  _i_node_add)
			printf("#<procedure:+>");
		else if (result == _i_node_minus)
			printf("#<procedure:->");
		else if (result == _i_node_div)
			printf("#<procedure:/>");
		else if (result == _i_node_mul)
			printf("#<prosedure:*>");
		else if (result == _i_node_eq)
			printf("#<procedure:=>");
		else if (result == _i_node_gt)
			printf("#<procedure:>>");
		else if (result == _i_node_lt)
			printf("#<procedure:<>");
		else if (result == _i_node_cons)
			printf("#<procedure:cons>");
		else if (result == _i_node_car)
			printf("#<procedure:car>");
		else if (result == _i_node_cdr)
			printf("#<procedure:cdr>");
		else if (result == _i_node_disp)
			printf("#<procedure:display>");
		else if (result == _i_node_set)
			printf("#<procedure:set!>");
		else
			printf("error");
	}
	else if (result->type == I_PAIR) {
		print_pair(result);
	}
	else if (result->type == I_LAMBDA) {
		printf("#<procedure>");
	}
	return;
}

int main(void)
{
	e_node* top;
	i_node* result;
	sch_init();
	do {
		printf("scheme> ");
		top = parser();
#if DEBUG
		//print_node(top);
#endif
		printf("=> ");
		result = _interpret(top,cur_env);
		_analyse_output(result);
		puts("");
	} while(top!=NULL);
	return 0;
}

#undef __INTER_C__
