/*
  Filename   : Driver.cc
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>
#include <cstring>

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
	string inputFile;
	string outputFile;
	string gccCommand;
	
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

		inputFile = name + "s";
		outputFile = name;
		outputFile.pop_back();
	}
	else
	{
		file = stdin;
		name = "Default.";
	}
	Parser p(file);
	if (p.parse())
	{
		cout << "Program is valid!" << endl;
		if (p.symbolTable())
		{
			cout << "Writing AST to \"" << name << "ast\"" << endl;
			p.print (name);
			p.codeGen(name);
			
			gccCommand = "gcc -m32 -o " + outputFile + " " + inputFile;
			char runGcc [gccCommand.length() + 1];
			std::strcpy (runGcc, gccCommand.c_str());

			cout << "Writing executable to \"" << outputFile << "\""<< endl;
			system(runGcc);
		}
		
	}
	return EXIT_SUCCESS;
}