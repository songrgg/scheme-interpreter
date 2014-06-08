#ifndef __HASH_H__
#define __HASH_H__
#include "parser.h"

#define DEFAULT_SIZE 16
#define LOAD_FACTOR 0.8
typedef char* hashKey;

typedef struct _i_node {
    int type;
    void* data;
    int para_cnt;
    char** para_list;
	struct _i_node* next;
} i_node;
typedef i_node* Value;

typedef struct _node {
	int hash;
    hashKey key;
	Value v;
	struct _node* next;
} node;

typedef struct _hashtable {
	int size;
	int capacity;
    float load_factor;
	node** table;
	struct _hashtable *next;
} hashtable;

typedef hashtable Env;

unsigned int hash(char* arKey,unsigned int nKeyLength);
hashtable* init_hashtable(int size);
node* _lookup(hashtable* ht,hashKey k);
void put_hash(hashtable* ht, hashKey k, Value v);
int get_hash(hashtable* ht, hashKey k, Value* v);
void resize(hashtable* ht,float amplification);
void drop_hashtable(hashtable* ht);

#endif
