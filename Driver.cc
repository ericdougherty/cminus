/*
  Filename   : Driver.cc
  Author     : Eric Dougherty
  Course     : CSCI 435
  Assignment : Lab 9 - Can't Parse This
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
    Parser p(file);
    if (p.parse())
    {
      p.print ();
      cout << "Program is valid!" << endl;
    }
    return EXIT_SUCCESS;
  }