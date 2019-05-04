#ifndef PARSER_H
#define PARSER_H
#endif

#include "node.h"
#include "token.h"

node * parse(std::vector<token>);
void traverseTree(node *, int);
void deleteTree(node *);
