/*
  Filename   : CMinusDriver.cc
  Author     : Ian T. Murry
  Course     : CSCI 435-01
*/

/*********************************************************************/
//System Includes

#include <vector>
#include <string>
#include <iostream>

/*********************************************************************/
// Local Includes

#include "CMinusLexer.h"
#include "CMinusParser.h"
#include "Ast.h"

/*********************************************************************/
// Using declarations
  
using std::string;
using std::vector;
using std::cout;
using std::endl;

/************************************************************/
// Function prototypes/global vars/type definitions.
void
outputTokens (FILE* input, vector<Token> & tokenBuffer);

void
isLexError (Token const & currentToken);

/************************************************************/

int
main (int argc, char* argv[])
{
  ++argv;
  --argc;
  string name;
  FILE* input;
  vector < Token > tokenBuffer;

  if (argc > 0)
  {
    input = fopen (argv[0], "r");
    name = argv[0];
    size_t i = name.size() - 1;
    while (name[i] != '.')
    {
      name.pop_back();
      --i;
    }
    outputTokens (input, tokenBuffer);
  }

  else
  {
    input = stdin;
    name = "Default.";
    outputTokens (input, tokenBuffer);
  }

    Parser p(tokenBuffer);
    if (p.parse())
    {
      if (p.symbolTable(argv[0]))
      {
        cout << "Program is valid!" << endl;
        cout << "Writing AST to \"" << name << "ast\"" << endl;
        p.print (name);
      }
      
    }
  return EXIT_SUCCESS;
}

/************************************************************/

void
outputTokens (FILE* input, vector<Token> & tokenBuffer)
{
  Lexer lexer (input);
  Token currentToken;

  do
  {
    currentToken = lexer.getToken ();

    if (currentToken.type == ERROR)
    {
      isLexError (currentToken);
    }

    tokenBuffer.push_back (currentToken);

  } while (currentToken.type != END_OF_FILE);
}

/************************************************************/

void
isLexError (const Token & currentToken)
{
  cout << "Error while scanning on line " << currentToken.lineNum;
  cout << ", column " << currentToken.colNum << endl;
  cout << "Encountered '" << currentToken.lexeme << "'" << endl;
  cout << "  Illegal Token.\n" << endl;

  exit (1);
}
