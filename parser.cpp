#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include "token.h"
#include "node.h"

using namespace std;

//Global variables
static token tk;
static vector<token> tokens;

//BNF prototypes
node * parse();
static node * program();
static node * block();
static node * vars();
static node * expr();
static node * A();
static node * K();
static node * N();
static node * M();
static node * R();
static node * stats();
static node * mStat();
static node * stat();
static node * assign();
static node * in();
static node * out();
static node * If ();
static node * loop();
static node * assign();
static node * RO();

//gets next token from scanner
static token fetchToken();

node * parse(vector<token> tokenList) {
	
	tokens = tokenList;
	tk = fetchToken();
	node * root = program();
	if (tk.tokenInstance == "EOF") {
		//cout << "Successfully parsed\n";
	}
	else {
		cout << "EOF expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}

	return root;
}

static node * program() {
	node * root = new node;
	root->label = "program";
	root->children.push_back(vars());
	root->children.push_back(block());
	return root;
}
static node * block() 
{
	node * n = new node;
	n->label = "block";
	if (tk.tokenInstance == "Begin")
	{
		tk = fetchToken();
		n->children.push_back(vars());
		n->children.push_back(stats());
		if (tk.tokenInstance == "End")
		{
			tk = fetchToken();
		}
		else
		{
			cout << "End token expected (" << tk.lineNumber << ")\n";
			exit(-1);
		}
	}
	else
	{
		cout << "Begin token expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * vars()
{
	node * n = new node;
	n->label = "vars";
	if (tk.tokenInstance == "INT")
	{
		tk = fetchToken();
		if (tk.tokenID == "IDToken") 
		{
			n->tokens.push_back(tk);
			tk = fetchToken();
			if (tk.tokenID == "digitToken")
 			{
				n->tokens.push_back(tk);
				tk = fetchToken();
				n->children.push_back(vars());	
			}
			else
			{
				cout << "Integer expected (" << tk.lineNumber << ")\n";
				exit(-1);
			}
		}
		else
		{
			cout << "Identifier expected (" << tk.lineNumber << ")\n";
			exit(-1);
		}
	}
	
	return n;

}
static node * expr()
{
	node * n = new node;
	n->label = "expr";
	n->children.push_back(A());
	n->children.push_back(K());
	return n;
}
static node * A() 
{
	node * n = new node;
	n->label = "A";
	n->children.push_back(N());

	if (tk.tokenInstance == "*")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
		n->children.push_back(A());
	}
	return n;
}
static node * K()
{
	node * n = new node;
	n->label = "K";
	if (tk.tokenInstance == "+")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
		n->children.push_back(expr());
	}
	else if (tk.tokenInstance == "-")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
		n->children.push_back(expr());
	}
	return n;
}
static node * N() 
{
	node * n = new node;
	n->label = "N";
	n->children.push_back(M());
	if (tk.tokenInstance == "/")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
		n->children.push_back(N());
	}
	return n;
}
static node * M() 
{
	node * n = new node;
	n->label = "M";
	if (tk.tokenInstance == "-")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
		n->children.push_back(M());
	}
	else
	{
		n->children.push_back(R());
	}
	return n;
}
static node * R() 
{
	node * n = new node;
	n->label = "R";
	if (tk.tokenInstance == "[") 
	{
		tk = fetchToken();
		n->children.push_back(expr());
		if (tk.tokenInstance == "]") 
		{
			tk = fetchToken();
		}
		else
		{
			cout << "] expected (" << tk.lineNumber << ")\n";
			exit(-1);
		}
	}
	else if (tk.tokenID == "IDToken")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
	}
	else if (tk.tokenID == "digitToken")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
	}
	else
	{
		cout << "[, Identifier or Integer expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * stats()
{
	node * n = new node;
	n->label = "stats";
	n->children.push_back(stat());
	if (tk.tokenInstance == ":")
	{
		tk = fetchToken();
		n->children.push_back(mStat());
	}
	else
	{
		cout << "Statement expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * mStat()
{
	node * n = new node;
	n->label = "mStat";
	if (tk.tokenInstance == "Read")
	{
		n->children.push_back(in());
		if (tk.tokenInstance == ":")
		{
			tk = fetchToken();
			n->children.push_back(mStat());
		}
		else
		{
			cout << ": expected (" << tk.lineNumber << ")\n";
			exit(-1);
		}
	}
	else if (tk.tokenInstance == "Output")
	{
		n->children.push_back(out());
		if (tk.tokenInstance == ":")
		{
			tk = fetchToken();
			n->children.push_back(mStat());
		}
		else
		{
			cout << ": expected (" << tk.lineNumber << ")\n";
                        exit(-1);
		}
	}
	else if (tk.tokenInstance == "Begin")
	{
		n->children.push_back(block());
		if (tk.tokenInstance == ":")
                {
                        tk = fetchToken();
                        n->children.push_back(mStat());
                }
                else
                {
                        cout << ": expected (" << tk.lineNumber << ")\n";
                        exit(-1);
                }
	}
	else if (tk.tokenInstance == "IFF")
	{
		n->children.push_back(If());
		if (tk.tokenInstance == ":")
                {
                        tk = fetchToken();
                        n->children.push_back(mStat());
                }
                else
                {
                        cout << ": expected (" << tk.lineNumber << ")\n";
                        exit(-1);
                }
	}
	else if (tk.tokenInstance == "Loop")
	{
		n->children.push_back(loop());
		if (tk.tokenInstance == ":")
                {
                        tk = fetchToken();
                        n->children.push_back(mStat());
                }
                else
                {
                        cout << ": expected (" << tk.lineNumber << ")\n";
                        exit(-1);
                }
	}
	else if (tk.tokenID == "IDToken")
	{
		n->children.push_back(assign());
		if (tk.tokenInstance == ":")
                {
                        tk = fetchToken();
                        n->children.push_back(mStat());
                }
                else
                {
                        cout << ": expected (" << tk.lineNumber << ")\n";
                        exit(-1);
                }
	}
	return n;
}
static node * stat() 
{
	node * n = new node;
	n->label = "stat";
	if (tk.tokenInstance == "Read")
	{
		n->children.push_back(in());
	}
	else if (tk.tokenInstance == "Output")
	{
		n->children.push_back(out());
	}
	else if (tk.tokenInstance == "Begin")
	{
		n->children.push_back(block());
	}
	else if (tk.tokenInstance == "IFF")
	{
		n->children.push_back(If());
	}
	else if (tk.tokenInstance == "Loop")
	{
		n->children.push_back(loop());
	}
	else if (tk.tokenID == "IDToken")
	{
		n->children.push_back(assign());
	}
	else
	{
		cout << "Keyword expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * in()
{
	node * n = new node;
	n->label = "in";
	if (tk.tokenInstance == "Read")
	{
		tk = fetchToken();
		if (tk.tokenInstance == "[")
		{
			tk = fetchToken();
			if (tk.tokenID == "IDToken")
			{
				n->tokens.push_back(tk);
				tk = fetchToken();
				if (tk.tokenInstance == "]")
				{
					tk = fetchToken();
				}
				else
				{
					cout << "] expected (" << tk.lineNumber << ")\n";
					exit(-1);
				}
			}
			else
			{
				cout << "Identifier expected (" << tk.lineNumber << ")\n";
				exit(-1);
			}
		}
		else
		{
			cout << "[ expected (" << tk.lineNumber << ")\n";
		}
	}
	else
	{
		cout << "Read expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * out() 
{
	node * n = new node;
	n->label = "out";
	if (tk.tokenInstance == "Output")
	{
		tk = fetchToken();
		if (tk.tokenInstance == "[")
		{
			tk = fetchToken();
			n->children.push_back(expr());
			if (tk.tokenInstance == "]")
			{
				tk = fetchToken();
			}
			else
			{
				cout << "] expected (" << tk.lineNumber << ")\n";
				exit(-1);
			}
		}
		else
		{
			cout << "[ expected (" << tk.lineNumber << ")\n";
			exit(-1);
		}
	}
	else
	{
		cout << "Output expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * If()
{
	node * n = new node;
	n->label = "If";
	if (tk.tokenInstance == "IFF")
	{
		tk = fetchToken();
		if (tk.tokenInstance == "[")
		{
			tk = fetchToken();
			n->children.push_back(expr());
			n->children.push_back(RO());
			n->children.push_back(expr());
			if (tk.tokenInstance == "]")
			{
				tk = fetchToken();
				n->children.push_back(stat());
			}
			else
			{
				cout << "] expected (" << tk.lineNumber << ")\n";
				exit(-1);		
			}
		}
		else
		{
			cout << "[ expected (" << tk.lineNumber << ")\n";
			exit(-1);
		}
	}
	else
	{
		cout << "IFF expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * loop()
{
	node * n = new node;
	n->label = "loop";
	if (tk.tokenInstance == "Loop")
	{
		tk = fetchToken();
		if (tk.tokenInstance == "[")
		{
			tk = fetchToken();
			n->children.push_back(expr());
			n->children.push_back(RO());
			n->children.push_back(expr());
			if (tk.tokenInstance == "]")
			{
				tk = fetchToken();
				stat();
			}
			else
			{
				cout << "] expected (" << tk.lineNumber << ")\n";
                                exit(-1);
			}
		}
		else
		{
			cout << "[ expected (" << tk.lineNumber << ")\n";
                        exit(-1);

		}
	}
	else
	{
		cout << "Loop expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * assign()
{
	node * n = new node;
	n->label = "assign";
	if (tk.tokenID == "IDToken")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
		if (tk.tokenInstance == "=")
		{
			n->tokens.push_back(tk);
			tk = fetchToken();
			n->children.push_back(expr());
		}
		else
		{
			cout << "= expected (" << tk.lineNumber << ")\n";
			exit(-1);
		}
	}
	else
	{
		cout << "Identifier expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}
static node * RO()
{
	node * n = new node;
	n->label = "RO";
	if (tk.tokenInstance == "<")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
	}
	else if (tk.tokenInstance == "=")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
		if ((tk.tokenInstance == "<") || (tk.tokenInstance == ">") || (tk.tokenInstance == "="))
		{
			n->tokens.push_back(tk);
			tk = fetchToken();
		}
	}
	else if (tk.tokenInstance == ">")
	{
		n->tokens.push_back(tk);
		tk = fetchToken();
	}
	else
	{
		cout << "Operator expected (" << tk.lineNumber << ")\n";
		exit(-1);
	}
	return n;
}

static token fetchToken() {
	token t = tokens[0];
	tokens.erase( tokens.begin() );
	return t;
}

void traverseTree(node * n, int level) {

	for (int i = 0; i < n->children.size(); i++)
		traverseTree(n->children[i], level + 1);		
	
	for (int i = 0; i != level; i++)
		cout << "  ";
	cout << n->label << endl;
	for (int i = 0; i != level; i++)
        	cout << "  ";
	cout << "Tokens: ";
	for (int i = 0; i < n->tokens.size(); i++)
		cout <<  n->tokens[i].tokenInstance << " ";
	cout << endl << endl;
}

void deleteTree(node * n)
{
	for (int i = 0; i < n->children.size(); i++)
                deleteTree(n->children[i]);

	delete n;
}
