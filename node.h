#include <string>
#include "token.h"
#include <vector>

#ifndef NODE_H
#define NODE_H

struct node
{
	std::string label;
	std::vector<node *> children;
	std::vector<token> tokens;
};

#endif
