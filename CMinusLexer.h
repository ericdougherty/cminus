/*
  Filename   : CMinusLexer.h
  Author     : Ian T. Murry
  Course     : CSCI 435-01
*/
/***********************************************************************/
// Macro guard to prevent multiple inclusions
#ifndef CMINUS_LEXER_H
#define CMINUS_LEXER_H

/***********************************************************************/
// System includes
#include <string>
#include "CMinusToken.h"

/***********************************************************************/

class Lexer
{
public:

  Lexer (FILE* sourceFile);

  Token
  getToken ();

  int
  getLineNumber ();

  int
  getColumnNumber ();

private:

  void
  getChar ();

  void
  ungetChar ();

  void
  eatWhitespace ();

  Token
  minusOrDecrement ();

  Token
  plusOrIncrement ();

  bool
  isDivision ();

  void
  whichTypeOfComment ();

  void
  eatOneLineComment ();

  void
  eatMultiLineComment ();

  Token
  lessThanOrLTE ();

  Token
  greaterThanOrGTE ();

  Token
  equalOrAssign ();

  Token
  isItNEQ ();

  void
  processLexId (Token & currentToken);

  void
  processLexNum (Token & currentToken);

  void
  isItKeyword (Token & currentToken);

  void
  processError (Token & currentToken);

private:
  FILE* m_sourceFile;
  int m_currentChar;
  int m_lineNum;
  int m_colNum;
};

/***********************************************************************/

#endif

