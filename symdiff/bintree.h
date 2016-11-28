#ifndef TREE_H

#define TREE_H

#include <stdio.h>
#include <stdlib.h>

#define bt bintree

const int MAX_SIMPLIFICATIONS = 5;

enum OPERATIONS {
	MUL = '*',
	ADD = '+',
	DIV = '/',
	SUB = '-'
};

enum TYPES {
	NUM = 1,
	VAR = 2,
	OP = 3,
	FUNC = 4
};

typedef struct node {
	int type;
	int data;
	node * left;
	node * right;
};

typedef struct bintree {
	int size;
	node * root;
};


bt * bt_ctor();
node * node_ctor(int type, int data, node * left, node *right);
int node_dtor(node * This);
int bt_dtor(bt * This);
bt * ReadFile(char * path);

void simplify_tree(bt * tree);
void reduce_tree(bt * tree);
void full_simplify(bt * tree);
void print_tree(bt * tree);
void bt_diff(bt * tree, int var);

#endif