/*
  Filename   : Driver.cc
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>

/************************************************************/
// Local includes

#include "Parser.h"

/************************************************************/
// Using declarations

using std::cout;
using std::endl;

/************************************************************/

int
main (int argc, char* argv[])
{
	string name;
	
	FILE* file;
	++argv;
	--argc;  
	if (argc > 0)
	{
		file = fopen (argv[0], "r");
		name = argv[0];
		size_t i = name.size() - 1;
		while (name[i] != '.')
		{
			name.pop_back();
			--i;
		}
	}
	else
	{
		file = stdin;
		name = "Default.";
	}
	Parser p(file);
	if (p.parse())
	{
		if (p.symbolTable())
		{
			cout << "Program is valid!" << endl;
			cout << "Writing AST to \"" << name << "ast\"" << endl;
			p.print (name);
		}
		
	}
	return EXIT_SUCCESS;
}