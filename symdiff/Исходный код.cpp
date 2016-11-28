#define _CRT_SECURE_NO_WARNINGS

#include "bintree.h"
#include <stdlib.h>
#include <stdio.h>

const int IOERROR = -1;

int main() {
	bt * tree = ReadFile("test2.txt");
	printf("Hello tree\n");
	print_tree(tree);
	/*reduce_tree(tree);
	simplify_tree(tree);*/
	full_simplify(tree);
	printf("Tree after full simplification\n");
	print_tree(tree);
	bt_diff(tree, 'x' - 'a');
	printf("Tree after derivative evaluation\n");
	print_tree(tree);
	full_simplify(tree);
	printf("Derivative tree after full simplification\n");
	print_tree(tree);
	return 0;
}