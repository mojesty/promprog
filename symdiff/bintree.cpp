#define _CRT_SECURE_NO_WARNINGS

#include "bintree.h"
#include <stdio.h>
#include <stdlib.h>

//DSL macros
#define IS(TYPE) n->type == OP && n->data == TYPE
#define cL n->left
#define cR n->right
#define LEFT(TYPE, VALUE) cL->type == TYPE && cL->data == VALUE
#define RIGHT(TYPE, VALUE) cR->type == TYPE && cR->data == VALUE

int CHANGE = 0;		//flag which rises when tree was changed during simplification procedures


bt * bt_ctor(){
	bt * ptr = (bt*)malloc(sizeof(bt));
	if (!ptr) return NULL;
	ptr->size = 0;
}

node * node_ctor(int type, int data, node * left, node *right) {
	node * ptr = (node*)malloc(sizeof(node));
	if (!ptr) return NULL;
	ptr->type = type;
	ptr->data = data;
	ptr->left = left;
	ptr->right = right;
	return ptr;
}

int node_dtor(node * This) {
	if (This->left) node_dtor(This->left);
	if (This->right) node_dtor(This->right);
	This->data = 0;
	This->type = 0;
	return 0;
}

int bt_dtor(bt * This){
	This->size = 0;
	return node_dtor(This->root);
}

node * copy(node * n) {
	node * n1 = NULL;
	node * n2 = NULL;
	if (cL) n1 = copy(cL);
	if (cR) n2 = copy(cR);
	return node_ctor(n->type, n->data, n1, n2);
}

int node_ok(node * n) {
	static int numcalls = 0;
	if (n) numcalls++;
	switch (n->type) {
	case VAR: {
				  if (n->data < 0 || n->data > 100) printf("Invalid variable encountered: %d. Possible variables are integer numbers between 0 and 100.\n");
				  break;
	}
	case OP: {
				 if (n->data != ADD && n->data != SUB && n->data != MUL && n->data != DIV) printf("Invalid operator encountered %c. Possible operators are + - * / \n");
				 if (!cL || !cR) printf("Null pointer encountered in inner level of a tree\n");
				 break;
	}
	case NUM: {
				  break;
	}
	case FUNC: {
				   break;
	}
	default: {
				 printf("Invalid type encountered in tree: %d \n", n->type);
	}
	}
	if (cL) node_ok(cL);
	if (cR) node_ok(cR);
	return numcalls;
}


//currentky not working
int tree_ok(bt * tree) {
	int nodeok = node_ok(tree->root);
	return (tree->size >= 0 && tree->size == nodeok);
}

//reads the expression and transforms it to the tree format
node * ReadNode(FILE * f){
	char c = ' ';
	fscanf(f, "%c ", &c);
	switch (c) {
		case '(': {		//operator
						node * res = node_ctor(OP, '+', ReadNode(f), NULL);
						fscanf(f, "%c ", &c);
						res->data = c;
						res->right = ReadNode(f);
						fscanf(f, "%c ", &c);
						if (c != ')') {
							free(res);
							return NULL;
						}
						return res;
		}

		case '[': {		//numerical constant
					  double num = 0;
					  fscanf(f, "%lf ", &num);
					  node * res = node_ctor(NUM, num, NULL, NULL);
					  fscanf(f, "%c ", &c);
					  if (c != ']') {
						  free(res);
						  return NULL;
					  }
					  return res;
		}
		case '{': {		//variable 
					 fscanf(f, "%c ", &c);
					 node * res = node_ctor(VAR, c-'a', NULL, NULL);
					 fscanf(f, "%c ", &c);
					 if (c != '}') {
						 free(res);
						 return NULL;
					 }
					 return res;
		}
	}
	
}

bt * ReadFile(char * path) {
	FILE * f = fopen(path, "r");
	if (!f) return NULL;
	bt * ptr = bt_ctor();
	ptr->root = ReadNode(f);
	fclose(f);
	if (!ptr->root) {
		printf("Error while reading file");
		return NULL;
	}
	return ptr;
}



node * Simplify(node * n) {
	if (!cL || !cR) return n;
	if (IS(MUL) && (LEFT(NUM, 0) || RIGHT(NUM, 0))) {	//simplification for 0*expr or expr * 0
		node * tmp = node_ctor(NUM, 0, NULL, NULL);
		CHANGE = 1;
		node_dtor(n);									//remove this part of tree and replace it with [0]
		return tmp;
	}
	if (IS(ADD) && LEFT(NUM, 0) && RIGHT(NUM, 0)) {		//simplification for (0 + 0)
		node * tmp = node_ctor(NUM, 0, NULL, NULL);
		CHANGE = 1;
		node_dtor(n);									//remove this part of tree and replace it with [0]
		return tmp;
	}
	if (IS(ADD) && LEFT(NUM, 0) && !RIGHT(NUM, 0)){		//simplifications for (expr + 0) or (0 + expr)
		node * tmp = cR;
		cR = NULL;
		CHANGE = 1;
		node_dtor(n);
		return tmp;
	}
	if (IS(ADD) && !LEFT(NUM, 0) && RIGHT(NUM, 0)){
		node * tmp = cL;
		cL = NULL;
		CHANGE = 1;
		node_dtor(n);
		return tmp;
	}
	if (IS(MUL) && LEFT(NUM, 1)) {						//simplification for 1*x
		node * tmp = cR;
		cR = NULL;
		CHANGE = 1;
		node_dtor(n);									//remove this part of tree and replace it with [0]
		return tmp;
	}
	if (IS(MUL) && RIGHT(NUM, 1)) {						//simplification for 1*x
		node * tmp = cL;
		cL = NULL;
		CHANGE = 1;
		node_dtor(n);									//remove this part of tree and replace it with [0]
		return tmp;
	}
	if (IS(DIV) && RIGHT(NUM, 1)){
		node * tmp = cL;
		cL = NULL;
		CHANGE = 1;
		node_dtor(n);									//remove this part of tree and replace it with [0]
		return tmp;
	}
	if (cL) cL = Simplify(cL);
	if (cR) cR = Simplify(cR);
	return n;
}

void simplify_tree(bt * tree) {
	if (tree->root) tree->root = Simplify(tree->root);
}


//constnt reduce function. Should be applied first before simplification function.
node * reduce_constants(node * n) {
	node * tmp = NULL;
	if (n->type == NUM || n->type == VAR) return n;
	if (cL) cL = reduce_constants(cL);
	if (cR) cR = reduce_constants(cR);
	if (n->type == OP && cL->type == NUM && cR->type == NUM) {
		int res = 0;
		switch (n->data) {
		case ADD: {
					  res = cL->data + cR->data;
					  tmp = node_ctor(NUM, res, NULL, NULL);
					  node_dtor(n);
					  CHANGE = 1;
					  return tmp;
		}
		case SUB: {
					  res = cL->data - cR->data;
					  tmp = node_ctor(NUM, res, NULL, NULL);
					  node_dtor(n);
					  CHANGE = 1;
					  return tmp;
		}
		case MUL: {
					  res = cL->data * cR->data;
					  tmp = node_ctor(NUM, res, NULL, NULL);
					  node_dtor(n);
					  CHANGE = 1;
					  return tmp;
		}
		case DIV: {
					  res = cL->data / cR->data;
					  tmp = node_ctor(NUM, res, NULL, NULL);
					  node_dtor(n);
					  CHANGE = 1;
					  return tmp;
		}
		}		
	}
	return n;
}

void reduce_tree(bt * tree) {
	if (tree->root) tree->root = reduce_constants(tree->root);
}


void full_simplify(bt * tree) {
	int i = 0;
	for (i = 0; i < MAX_SIMPLIFICATIONS; i++) {
		CHANGE = 0;
		if (tree->root) {
			tree->root = reduce_constants(tree->root);
			tree->root = Simplify(tree->root);
		}
		if (CHANGE == 0) break;
	} 
	return;
}

void print_node(node * n) {
	switch (n->type) {
	case OP:{
				printf("( ");
				print_node(cL);
				printf(" %c ", n->data);
				print_node(cR);
				printf(") ");
				break;
	}
	case NUM:{
				printf(" %d ", n->data);
				break;
	}
	case VAR:{
				printf(" %c ", n->data + 'a');
	}
	}
}

void print_tree(bt * tree) {
	if (tree->root) print_node(tree->root);
	printf("\n");
}

node * diff(node * n, int var) {
	switch (n->type) {
	case OP: {
				 switch (n->data) {
				 case MUL: {
							   node * cl = copy(cL);
							   node * dl = diff(cL, var);
							   node * cr = copy(cR);
							   node * dr = diff(cR, var);
							   node * n1 = node_ctor(OP, MUL, dl, cr);
							   node * n2 = node_ctor(OP, MUL, dr, cl);
							   return node_ctor(OP, ADD, n1, n2);
				 }
				 case ADD: {
							   node * dl = diff(cL, var);
							   node * dr = diff(cR, var);
							   return node_ctor(OP, ADD, dl, dr);
				 }
				 case SUB: {
							   node * dl = diff(cL, var);
							   node * dr = diff(cR, var);
							   return node_ctor(OP, SUB, dl, dr);
				 }
				 case DIV: {
							   node * cl = copy(cL);
							   node * dl = diff(cL, var);
							   node * cr = copy(cR);
							   node * cr2 = copy(cR);
							   node * dr = diff(cR, var);
							   node * n1 = node_ctor(OP, MUL, dl, cr);
							   node * n2 = node_ctor(OP, MUL, dr, cl);
							   node * n3 = node_ctor(OP, SUB, n1, n2);
							   node * n4 = node_ctor(OP, MUL, cr2, cr2);
							   return node_ctor(OP, DIV, n3, n4);
				 }
				 }
	}
	case NUM: {
				  return node_ctor(NUM, 0, NULL, NULL);
	}
	case VAR: {
				  if (n->data == var) return node_ctor(NUM, 1, NULL, NULL);
				  else return node_ctor(NUM, 0, NULL, NULL);
	}
	}
}

void bt_diff(bt * tree, int var) {
	if (tree->root) tree->root = diff(tree->root, var);
}