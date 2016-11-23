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
    }
    Parser p(file);
    if (p.parse())
    {
      p.print (name);
      cout << "Program is valid!" << endl;
    }
    return EXIT_SUCCESS;
  }