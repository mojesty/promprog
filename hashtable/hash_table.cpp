#define _CRT_SECURE_NO_WARNINGS

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



ht* ht_ctor(int size) {
	ht * ptr = (ht*)malloc(sizeof(ht));
	if (!ptr) return NULL;
	ptr->size = size;
	if ((ptr->data = (List**)malloc(size * sizeof(List*))) == NULL) {
		free(ptr);
		return NULL;
	}
	int i = 0, j = 0;
	for (i = 0; i < size; i++) {
		if ((ptr->data[i] = List_ctor()) == NULL) {
			for (j = 0; j <= i; j++) free(ptr->data[j]);
			free(ptr->data);
			free(ptr);
			return NULL;
		}
	}
	return ptr;
}

int ht_dtor(ht * This) {
	int i = 0;
	for (i = 0; i < This->size; i++) List_dtor(This->data[i]);
	//memset(This->data, 0, This->size * sizeof(List));		//destroy all data inside lists
	This->size = 0;
	free(This->data);
	free(This);
	return 0;
}

int ht_push(ht * This,  int func(void * value), void* val, int size) {
	int hash = func(val) % This->size;
	//if (hash >= This->size) return SIZE_ERROR;
	return push(This->data[hash], val, size, 0);
}

int ht_pop(ht * This, int func(void * value), void* val, int size) {
	int hash = func(val) % This->size;
	//if (hash >= This->size) return SIZE_ERROR;
	return pop(This->data[hash], val, size);
}

int ht_find(ht * This, int func(void * value), void* val, int size) {
	int hash = func(val) % This->size;
	//if (hash >= This->size) return SIZE_ERROR;
	return search(This->data[hash], val, size);
}

int ht_sizes(ht * This, char * path){
	int i = 0;
	FILE * f = fopen(path, "w");
	for (i = 0; i < This->size; i++) fprintf(f, "%d ", This->data[i]->size);
	fclose(f);
	return 0;
}

int hashfunc1(void * s) {
	return 1;
}

int hashfunc2(void * s) {
	return ((char*)s)[0] - 'a';
}

int hashfunc3(void * s){
	return strlen((char*)s);
}

int hashfunc4(void * s) {
	int res = 0;
	int i = 0;
	char * s1 = (char*)s;
	while (s1[i + 1] != '\0') {
		res += s1[i];
		i++;
	}
	return res % 100;
}

void file_to_table(ht * table, char * path){
	FILE * f = fopen(path, "r");
	if (!f) {
		printf("Unable to read file %s \n", path);
		return;
	}
	table = ht_ctor(10);
	char buf[50];
	while (fscanf(f, "%s \n", &buf) == 1){
		int sw = strlen(buf);
		ht_push(table, hashfunc2, &buf, sw);
		ht_pop(table, hashfunc2, &buf, sw);
	}
	fclose(f);
	ht_sizes(table, "sizes2.txt");
	ht_dtor(table);
	return;
}