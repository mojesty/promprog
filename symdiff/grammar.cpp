#include "grammar.h"
#include <stdio.h>
#include <stdlib.h>

char * s = NULL;

node *  GetN() {
	int res = 0;
	int noOverflow = 1;
	if ('a' <= *s && *s <= 'z') {
		//printf("Variable found: %c\n", *s);
		char tmp = *s;
		s++;
		return node_ctor(VAR, tmp - 'a', NULL, NULL);
	}
	while ('0' <= *s && *s <= '9') {
		res = res * 10 + *s - '0';
		s++;
		/*__asm {
			jno NO_OVERFLOW
				mov noOverflow, 0
			NO_OVERFLOW:
		}
		if (!noOverflow) {
			printf("Overflow encountered during number parsing. Current value %d\n", res);
		}*/
	}
	return node_ctor(NUM, res, NULL, NULL);
}

node * GetE() {
	node * left = GetT();
	node * right = NULL;
	int op;
	while (true) {
		if (*s == '+') op = ADD;
		if (*s == '-') op = SUB;
		if (*s != '+' && *s != '-') break;
		s++;
		right = GetT();
		left = node_ctor(OP, op, left, right);
	}
	return left;
}

node * GetT() {
	node * left = GetP();
	node * right = NULL;
	int op = 0;
	if (*s == '*') op = MUL;
	if (*s == '/') op = DIV;
	while (true) {
		if (*s == '*') op = MUL;
		if (*s == '/') op = DIV;
		if (*s != '*' && *s != '/') break;
		s++;
		right = GetT();
		left = node_ctor(OP, op, left, right);
	}
	return left;
}

node * GetP() {
	node * tmp = NULL;
	if (*s == '(') {
		s++;
		tmp = GetE();
		if (*s != ')') printf("Invalid closing bracket: %c instead of )\n", *s);
		s++;
	}
	else tmp = GetN();
	return tmp;
}

node * GetG0(char * str) {
	node * res = NULL;
	s = str;
	res = GetE();
	if (*s != '\0') printf("Unable to find end of the string. Stop.\n");
	return res;
}