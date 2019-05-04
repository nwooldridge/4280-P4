#include "node.h"
#include "token.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

vector< vector<string> > scopes;

void statSem(node * n, int currentScope)
{	
	if (n->label == "block")
	{
		currentScope += 1;
	}

	if (scopes.empty())
	{
		vector <string> v;
		scopes.push_back(v);
	}

	if (currentScope > (scopes.size() - 1))
	{
		vector <string> v;
		scopes.push_back(v);
	}

	if (n->tokens.size() > 0)
	{
		for (int i = 0; i < n->tokens.size(); i++)
		{
			if (n->tokens[i].tokenID == "IDToken")
			{
				if ((i < n->tokens.size() - 1) && (n->tokens[i + 1].tokenID == "digitToken"))
				{
					bool idExists = false;
					for (int j = 0; j < scopes[ currentScope ].size(); j++)
					{
						if (n->tokens[i].tokenInstance == scopes[currentScope][j])
						{
							idExists = true;
							break;
						}
					}
					
					if (idExists)
					{
						cout << "Redefinition of " << n->tokens[i].tokenInstance << endl;
						exit(-1);
					}
					else
					{
						scopes[currentScope].push_back(n->tokens[i].tokenInstance);
					}
				}
				else
				{
					bool idExists = false;
					for (int j = 0; j <= currentScope; j++) 
					{
						for (int k = 0; k < scopes[j].size(); k++)
						{
							if (n->tokens[i].tokenInstance == scopes[j][k])
							{
								idExists = true;
								break;
							}
						}
					}
					
					if (!idExists)
					{
						cout << "Undefined reference to " << n->tokens[i].tokenInstance << endl;
						exit(-1);					
					}
				}
			}
		} 		
	}

	for (int i = 0; i < n->children.size(); i++)
	{
		statSem(n->children[i], currentScope);
	}	

	if (n->label == "block")
	{
		scopes.pop_back();
	}
 
}

void statSem(node * root)
{
	statSem(root, 0);
}

