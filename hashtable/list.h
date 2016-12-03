#ifndef LIST_H

#define LIST_H

const int MALLOC_ERROR = -1;
const int SIZE_ERROR = -1;

//Stores buffer data with given size inside node
typedef struct list {
	void * data;
	int size;
	list * prev;
	list * next;
};

typedef struct List {
	int size;
	list * start;
	list * end;
};



List* List_ctor();
void List_dtor(List * This);
list* list_ctor(void* val, int size, list * prev, list * next);
list* list_dtor(list * This);
int List_dump(list * This);
void* front(List * This);
void* back(List * This);
int push_front(List * This, void* val, int size);
int pop_front(List * This);
int push_back(List * This, void* val, int size);
int pop_back(List * This);
int push(List * This, void* val, int size, int num);
int pop(List * This, void* val, int size);
int search(List * This, void* val, int size);
void* find(List * This, int num);
int print(List * This);

#endif