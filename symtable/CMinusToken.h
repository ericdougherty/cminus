/*
 Filename   : CMinusTokens.h
 Author     : Ian T. Murry
 Course     : CSCI 435-01
 */

/*********************************************************************/
// Macro guard to prevent multiple inclusions
#ifndef CMINUS_TOKEN_H
#define CMINUS_TOKEN_H

/*********************************************************************/

enum TokenType
{
  // Special tokens
  END_OF_FILE,
  ERROR,

  // Keywords
  IF,
  ELSE,
  INT,
  VOID,
  RETURN,
  WHILE,

  // Operators
  PLUS,
  MINUS,
  TIMES,
  DIVIDE,
  LT,
  LTE,
  GT,
  GTE,
  EQ,
  NEQ,
  ASSIGN,

  INCREMENT,
  DECREMENT,

  // Punctuators
  SEMI,
  COMMA,
  LPAREN,
  RPAREN,
  LBRACK,
  RBRACK,
  LBRACE,
  RBRACE,

  // Identifier and integer literals
  ID,
  NUM
};

/*********************************************************************/

struct Token
{
  Token (TokenType pType = END_OF_FILE, std::string pLexeme = "",
	 int pLineNum = 0, int pColNum = 0, int pNumber = 0)
  : type (pType), lexeme (pLexeme),  number (pNumber), lineNum(pLineNum), colNum(pColNum)
  {
  }

  TokenType type;
  std::string lexeme;
  int number;
  int lineNum;
  int colNum;

};

/*********************************************************************/

#endif
