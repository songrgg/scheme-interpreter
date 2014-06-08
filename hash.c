#include "hash.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

unsigned int hash(char* arKey,unsigned int nKeyLength)
{
	unsigned int h=0,g;
	char* arEnd=arKey+nKeyLength;

	while(arKey<arEnd)
	{
		h=(h<<4)+*arKey++;
		if ((g=(h&0xF0000000))) {
			h=h^(g>>24);
			h=h^g;
		}
	}
	return h;
}

hashtable* init_hashtable(int size)
{
	int i;
	hashtable* ht=malloc(sizeof(hashtable));
	ht->capacity=size>0?size:DEFAULT_SIZE;
	ht->size=0;
	ht->load_factor= LOAD_FACTOR;
	ht->table= malloc(ht->capacity*sizeof(struct _node*));
	for(i=0;i<ht->capacity;i++) {
		ht->table[i]= NULL;
	}
	ht->next= NULL;
	return ht;
}

node* _lookup(hashtable* ht,hashKey k)
{
	unsigned int hashkey= hash(k,strlen(k));
	hashtable* searched_hashtable= ht;
	while (searched_hashtable!=NULL) {
		node** table= searched_hashtable->table;
		node* list= table[hashkey%searched_hashtable->capacity];
		while (list!=NULL) {
			if (hashkey==list->hash && 0==strcmp(list->key,k))
				return list;
			list= list->next;
		}
		searched_hashtable=searched_hashtable->next;
	}
	return NULL;
}

void put_hash(hashtable* ht,hashKey k,Value v)
{
	node* found= _lookup(ht,k);
	unsigned int hi;
	if(found == NULL) {
		float load_factor= (float)ht->size/ht->capacity;
		if (load_factor>ht->load_factor) {
#if 0
			fprintf(stderr,"RESIZING....,%f\n",load_factor);
#endif
			resize(ht,1.5);
		}
		hi= hash(k,strlen(k));
		found= malloc(sizeof(struct _node));
		found->key= strdup(k);
		found->hash= hi;
		found->v= v;
		found->next= ht->table[hi%ht->capacity];
		ht->table[hi%ht->capacity]= found;
		ht->size++;
	}
	found->v= v;
}

/*return 1 when failure, 0 on success*/
int get_hash(hashtable* ht,hashKey k,Value* v)
{
	node* found= _lookup(ht,k);
	if(found == NULL)
		return 1;
	*v= found->v;
	return 0;
}

void resize(hashtable* ht,float amplification)
{
	int i,new_size;
	new_size= (int)ht->capacity*amplification;
	node** origin_table= ht->table;
	node** new_table=malloc(new_size*sizeof(node*));
	/*init new table*/
	for(i=0;i<new_size;i++)
		new_table[i]=NULL;
	for(i=0;i<ht->capacity;i++){
		node* start= origin_table[i];
		for(;start;){
			node* next= start->next;
			start->next=new_table[start->hash%new_size];
			new_table[start->hash%new_size]=start;
			start=next;
		}
	}
	free(ht->table);
	ht->table= new_table;
	ht->capacity= new_size;
}

void drop_hashtable(hashtable* ht)
{
	int i;
	for(i=0;i<ht->capacity;i++){
		node* start= ht->table[i];
		for(;start;){
			node* next= start->next;
			free(start->key);

			/*TODO*/

			free(start);
			start= next;
		}
	}
	free(ht->table);
}
