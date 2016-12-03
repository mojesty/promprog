#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


List * List_ctor(){
	List * This = NULL;
	if ((This = (List*)malloc(sizeof(List))) == NULL) {
		return NULL;
	}
	This->size = 0;
	This->start = This->end = NULL;
	return This;
}

list * list_ctor(void* val, int size, list * prev, list * next){
	list * ptr = NULL;
	if ((ptr = (list*)malloc(sizeof(list))) == NULL) {
		return NULL;
	}
	if ((ptr->data = calloc(size, 1)) == NULL) {
		free(ptr);
		return NULL;
	}
	memcpy(ptr->data, val, size);
	ptr->size = size;
	//ptr->data = val;
	ptr->next = next;
	ptr->prev = prev;
	return ptr;
}

list * list_dtor(list * This) {
	if (!This) return NULL;
	list * ptr = This->next;
	free(This->data);
	free(This);
	return ptr;
}

void List_dtor(List * This) {
	This->size = -1;
	list * ptr = This->start;
	while ((ptr = list_dtor(ptr)) != NULL);
	This->start = This->end = NULL;
}

int List_dump(List * This) {
	//list_dump not created yet..
	return 0;
}

void* front(List * This) {
	return This->start->data;
}

void* back(List * This) {
	if (This->size == 1) return This->start->data;
	return This->end->data;
}

int push_front(List * This, void* val, int size) {
	list * ptr = list_ctor(val, size, NULL, This->start);		//создаем новый узел
	if (!ptr) return MALLOC_ERROR;						//проверяем создался ли он
	if (This->start) This->start->prev = ptr;							//и если да, то прицепляем его к списку
	This->start = ptr;
	if (This-> size == 0) This->end = ptr;						//если список изначально был пустым
	This->size++;
	return 0;
}

int pop_front(List * This) {
	if (This->size == 0) return SIZE_ERROR;
	list * tmp = This->start;
	if (This->start->next) This->start->next->prev = NULL;
	This->start = This->start->next;
	list_dtor(tmp);
	This->size--;
	return 0;
}

int push_back(List * This, void* val, int size) {
	list * ptr = list_ctor(val, size, This->end? This->end: This->start, NULL);
	if (!ptr) return MALLOC_ERROR;
	This->end ? This->end->next = ptr : This->start->next = ptr;
	This->end = ptr;
	This->size++;
	return 0;
}

int pop_back(List * This) {
	if (This->size == 0) return SIZE_ERROR;
	list * tmp = This->end;
	if (This->end->prev) This->end->prev->next = NULL;
	This->end = This->end->prev;
	list_dtor(tmp);
	This->size--;
	return 0;
}

//inserts element AFTER the element with number num
int push(List * This, void* val, int size, int num) {
	if (num > This->size) return SIZE_ERROR;
	if (num == 0) return push_front(This, val, size);
	if (num == This->size) return push_back(This, val, size);
	int i = 0;
	list * ptr = This->start;
	for (i = 1; i < num; i++, ptr = ptr->next);
	list * elem = list_ctor(val, size, ptr, ptr->next);
	if (!ptr) return MALLOC_ERROR;
	ptr->next->prev = elem;
	ptr->next = elem;
	This->size++;
	return 0;
}

//deletes element with given data
int pop(List * This,  void* val, int size) {
	if (This->size == 0) return SIZE_ERROR;
	if (!memcmp(This->start->data, val, size)) return pop_front(This);
	if (!memcmp(This->end->data, val, size)) return pop_back(This);
	int i = 0;
	list * ptr = This->start;
	while (ptr->next->next != NULL) {
		ptr = ptr->next;
		if (memcmp(ptr->data, val, size)) {
			ptr = ptr->prev;
			list * elem = ptr->next;	//this element will be removed
			ptr->next = elem->next;
			elem->next->prev = ptr;
			list_dtor(elem);
			This->size--;
		}
	}
	return 0;
}

//Searches the first element in the list which has given data
//returns 1, if element was found, 0 otherwise
int search(List * This, void* val, int size) {
	if (This->size == 0) return 0;
	list * ptr = This->start;
	int i = 0;
	for (i = 0; i < This->size; i++, ptr = ptr->next) {
		if (memcmp(ptr->data, val, size)) return 1;
	}
	return 0;
}

//Accesses the element with number num (numbers start from 1!)
void* find(List * This, int num) {
	if (num>This->size) return NULL;
	int i = 0;
	list * ptr = This->start;
	for (i = 1; i < num; i++, ptr = ptr->next);
	return ptr->data;
}

int print(List * This) {
	int i = 0;
	printf("Printing contents of list. Size of list is %d\n", This->size);
	if (This->size == 0) return 0;
	list * ptr = This->start;
	for (i = 0;i < This->size;i++) {
		//printf("%5lf ", ptr->data);
		ptr = ptr->next;
	}
	printf("\n");
	return 0;
}