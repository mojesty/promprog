#ifndef HASH_TABLE_H

#define HASH_TABLE_H
#include "list.h"
#define ht hash_table

typedef struct hash_table {
	int size;
	List ** data;
};

ht* ht_ctor(int size);
int ht_dtor(ht * This);
int ht_push(ht * This, int func(void * value), void* val, int size);
int ht_pop(ht * This, int func(void * value), void* val, int size);
int ht_find(ht * This, int func(void * value), void* val, int size);
void file_to_table(ht * table, char * path);
int ht_sizes(ht * This, char * path);
int hashfunc4(void * s);
int hashfunc2(void * s);
int hashfunc3(void * s);
int hashfunc1(void * s);

#endif