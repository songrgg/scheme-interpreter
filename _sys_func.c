#include "interpret.h"
#include "_memory.h"
#include "_bignum.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

i_node* _sys_proc_set(e_node* para_list,Env* env) {
  i_node* original = _interpret(para_list,env);
  i_node* replace  = _interpret(para_list->child_n,env);
  if (original && replace) {
    memcpy(original, replace, sizeof(*original));
  } else {
    fprintf(stderr, "set! <original> <replace>\n");
  }
}

i_node* _sys_proc_add(e_node* para_list,Env* env) {
    e_node* iter;
	char* sum = (char*)_alloc(2);
	sum[0] = '0'; sum[1] = 0;
    for (iter=para_list; iter; iter=iter->child_n) {
        i_node* tmp = _interpret(iter,env);
        if (tmp == NULL) {
            return NULL;
        }
        if (tmp->type != I_NUM) {
            printf("Parameter should be number*\n");
            return NULL;
        }
		char* t = sum;
		sum = _int_add(sum,(char*)tmp->data);
		_dealloc(t);
    }
    return _num_i_node(sum);
}

i_node* _sys_proc_minus(e_node* para_list,Env* env) {
    e_node* iter;
    char sum[30];
    int s = 0;
    if (para_list == NULL) {
        return _num_i_node("0");
    }
#if 0
    if (para_list->child_e) {
        if (para_list->child_n)
            s = atoi(para_list->child_e);
        else
            s = -atoi(para_list->child_e);
    }
    else {
#endif
        i_node* tmp = _interpret(para_list,env);
        if (tmp == NULL) {
            return NULL;
        }
        if (tmp->type!=I_NUM) {
            printf("Parameter should be number*\n");
            return NULL;
        }
        if (para_list->child_n)
            s = atoi((char*)tmp->data);
        else
            s = -atoi((char*)tmp->data);
#if 0
    }
#endif
    for (iter=para_list->child_n; iter; iter=iter->child_n) {
        i_node* tmp = _interpret(iter,env);
        if (tmp == NULL) {
            return NULL;
        }
        if (tmp->type != I_NUM) {
            printf("Parameter should be number*\n");
            return NULL;
        }
        s = s - atoi((char*)tmp->data);
    }
    sprintf(sum,"%d",s);
    return _num_i_node(sum);
}

i_node* _sys_proc_mul(e_node* para_list,Env* env) {
    e_node* iter;
    char sum[30];
    int s = 1;
    for (iter=para_list; iter; iter=iter->child_n) {
        i_node* tmp = _interpret(iter,env);
        if (tmp == NULL) {
            return NULL;
        }
        if (tmp->type != I_NUM) {
            printf("Parameter should be number*\n");
            return NULL;
        }
        s = s * atoi((char*)tmp->data);
    }
    sprintf(sum,"%d",s);
    return _num_i_node(sum);
}

i_node* _sys_proc_div(e_node* para_list,Env* env) {
    return NULL;
}

i_node* _sys_proc_eq(e_node* para_list,Env* env) {
    e_node* iter;
    i_node* tmp;
	char* value;
    int cnt = 0;
    for (iter=para_list; iter; iter=iter->child_n) {
        cnt++;
    }
    if (cnt < 2) {
        printf("Less parameters, more than 2 needed.\n");
        return NULL;
    }
    tmp = _interpret(para_list,env);
    if (tmp->type != I_NUM) {
        printf("Number needed\n");
        return NULL;
    }
    else
        value = (char*)tmp->data;
    for (iter=para_list->child_n; iter; iter=iter->child_n) {
        tmp = _interpret(iter,env);
        if (tmp->type != I_NUM) {
            printf("Number needed\n");
            return NULL;
        }
        else {
            if (strcmp(value,(char*)tmp->data)!=0) {
                return _i_node_false;
            }
        }
    }
    return _i_node_true;
}

i_node* _sys_proc_gt(e_node* para_list,Env* env) {
	e_node* iter;
	i_node* tmp;
	char* value;
	int cnt = 0;
	for (iter=para_list; iter; iter=iter->child_n) {
		cnt++;
	}
	if (cnt < 2) {
		printf("Less parameters, more than 2 needed.\n");
		return NULL;
	}
	tmp = _interpret(para_list,env);
	if (tmp->type != I_NUM) {
		printf("Number needed\n");
		return NULL;
	}
	else
		value = (char*)tmp->data;
	for (iter=para_list->child_n; iter; iter=iter->child_n) {
		tmp = _interpret(iter,env);
		if (tmp->type != I_NUM) {
			printf("Number needed\n");
			return NULL;
		}
		else {
			if (atoi(value) <= atoi((char*)tmp->data)) {
				return _i_node_false;
			}
			value = (char*)tmp->data;
		}
	}
	return _i_node_true;
}

i_node* _sys_proc_lt(e_node* para_list,Env* env) {
	e_node* iter;
	i_node* tmp;
	char* value;
	int cnt = 0;
	for (iter=para_list; iter; iter=iter->child_n) {
		cnt++;
	}
	if (cnt < 2) {
		printf("Less parameters, more than 2 needed.\n");
		return NULL;
	}
	tmp = _interpret(para_list,env);
	if (tmp->type != I_NUM) {
		printf("Number needed\n");
		return NULL;
	}
	else
		value = (char*)tmp->data;
	for (iter=para_list->child_n; iter; iter=iter->child_n) {
		tmp = _interpret(iter,env);
		if (tmp->type != I_NUM) {
			printf("Number needed\n");
			return NULL;
		}
		else {
			if (atoi(value) >= atoi((char*)tmp->data)) {
				return _i_node_false;
			}
			value = (char*)tmp->data;
		}
	}
	return _i_node_true;
}

i_node* _sys_proc_cons(e_node* para_list,Env* env)
{
	int para_num = _para_num(para_list);
	if (para_num != 2) {
		printf("cons only need 2 parameter.\n");
		return NULL;
	}
	return _pair_i_node(
			_interpret(para_list,env),
			_interpret(para_list->child_n,env));
}

i_node* _sys_proc_car(e_node* para_list,Env* env)
{
	int para_num = _para_num(para_list);
	if (para_num != 1) {
		printf("car only need 1 parameter.\n");
		return NULL;
	}
	i_node* pair = _interpret(para_list,env);
	return (i_node*)pair->data;
}

i_node* _sys_proc_cdr(e_node* para_list,Env* env)
{
	int para_num = _para_num(para_list);
	if (para_num != 1) {
		printf("cdr only need 1 parameter.\n");
		return NULL;
	}
	i_node* pair = _interpret(para_list,env);
	return (i_node*)pair->next;
}

i_node* _sys_proc_disp(e_node* para_list,Env* env)
{
	i_node* val = _interpret(para_list,env);
	if (val->type == I_STRING) {
		int i,len=strlen((char*)val->data);
		for (i=1; i<len-1; i++)
			printf("%c",((char*)val->data)[i]);
	}
	else
		_analyse_output(val);
	return _i_node_void;
}
