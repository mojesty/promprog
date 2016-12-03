#ifndef GRAMMAR_H

#define GRAMMR_H
#include "bintree.h"

const int PARSING_ERROR = -1;

node *  GetG0(char * str);
node  * GetE();
node * GetT();
node *  GetP();
node *  GetN();

#endif