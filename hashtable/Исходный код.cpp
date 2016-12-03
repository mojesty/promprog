#include <stdio.h>
#include "list.h"
#include "hash_table.h"

int sum(int a,int b) {
	return a + b;
}

//typedef int sum(int a, int b);

int main() {/*
	List * data = List_ctor();
	push(data, 1,0);
	pop(data, 1);
	push(data, 1, 0);
	push(data, 2,1);
	push(data, 3, 0);
	print(data);
	pop(data, 3);
	pop(data, 2);
	pop(data, 1);
	print(data);
	pop(data, 0);*/
	//ht* table =ht_ctor(10, hashfunc1);
	int i = 4;
	ht * tbl = {};
	file_to_table(tbl, "test.txt");
	//ht_push(table,  &i, sizeof(int));
	return 0;
}