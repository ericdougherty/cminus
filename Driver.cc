/*
  Filename   : Driver.cc
  Author     : Eric Dougherty
  Course     : CSCI 435
  Assignment : Lab 9 - Can't Parse This
*/

/************************************************************/
// System includes

#include <cstdlib>

/************************************************************/
// Local includes

#include "Parser.h"

/************************************************************/

int
main (int argc, char* argv[])
{
    FILE* file;
    ++argv;
    --argc;  
    if (argc > 0)
    {
        file = fopen (argv[0], "r");
    }
    else
    {
        file = stdin;
    }
  vector<DeclarationNode*> declarations;
	Parser p(file, declarations);
	p.parse();

    return EXIT_SUCCESS;
}