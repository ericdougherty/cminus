/*
  Filename   : CMinusLexer.cc
  Author     : Ian T. Murry
  Course     : CSCI 435-01
*/
/************************************************************/
// System includes

#include <cctype>
#include <cstdio>
#include <string>
#include <map>

/************************************************************/
// Local includes

#include "CMinusLexer.h"

/************************************************************/
// Using declarations
  
using std::map;
using std::string;

/************************************************************/

Lexer::Lexer (FILE* inputSourceFile)
    : m_sourceFile (inputSourceFile), m_currentChar (), m_lineNum (1), m_colNum (
        -1)

{
}

/************************************************************/

Token
Lexer::getToken ()
{
  while (!feof (m_sourceFile))
  {
    getChar ();

    if (isspace (m_currentChar))
    {
      // Keeps track of line and column #'s
      eatWhitespace ();
    }

    switch (m_currentChar)
    {
    case EOF:
      continue;

      //Punctuators
    case ';':
      return Token (SEMI, ";", m_lineNum, m_colNum);

    case ',':
      return Token (COMMA, ",", m_lineNum, m_colNum);

    case '(':
      return Token (LPAREN, "(", m_lineNum, m_colNum);

    case ')':
      return Token (RPAREN, ")", m_lineNum, m_colNum);

    case '[':
      return Token (LBRACK, "[", m_lineNum, m_colNum);

    case ']':
      return Token (RBRACK, "]", m_lineNum, m_colNum);

    case '{':
      return Token (LBRACE, "{", m_lineNum, m_colNum);

    case '}':
      return Token (RBRACE, "}", m_lineNum, m_colNum);

      //Operands
    case '-':
      return minusOrDecrement ();

    case '+':
      return plusOrIncrement ();

    case '*':
      return Token (TIMES, "*");

    case '/':
      if (isDivision ())
      {
        return Token (DIVIDE, "/", m_lineNum, m_colNum);
      }

      whichTypeOfComment ();
      continue;

    case '<':
      return lessThanOrLTE ();

    case '>':
      return greaterThanOrGTE ();

    case '=':
      return equalOrAssign ();

    case '!':
      return isItNEQ ();
    }

    //Errors, IDs, Keywords, and Numbers

    Token currentToken;

    if (!isalnum (m_currentChar))
    {
      processError (currentToken);
    }

    if (isalpha (m_currentChar))
    {
      processLexId (currentToken);
      isItKeyword (currentToken);
    }

    if (isdigit (m_currentChar))
    {
      processLexNum (currentToken);
    }

    return currentToken;
  }

  return Token (END_OF_FILE);
}

/************************************************************/

int
Lexer::getLineNumber ()
{
  return m_lineNum;
}

/************************************************************/

int
Lexer::getColumnNumber ()
{
  return m_colNum;
}

/************************************************************/

void
Lexer::getChar ()
{
  m_currentChar = fgetc (m_sourceFile);
  ++m_colNum;
}

/************************************************************/

void
Lexer::ungetChar ()
{
  ungetc (m_currentChar, m_sourceFile);
  --m_colNum;
}

/************************************************************/

void
Lexer::eatWhitespace ()
{
  while (isspace (m_currentChar))
  {
    if (m_currentChar == '\n')
    {
      m_lineNum++;
      m_colNum = -1;
    }

    getChar ();
  }
}

/************************************************************/

Token
Lexer::minusOrDecrement ()
{
  getChar ();
  if (m_currentChar != '-')
  {
    ungetChar ();
    return Token (MINUS, "-", m_lineNum, m_colNum);
  }
  return Token (DECREMENT, "--", m_lineNum, m_colNum);
}

/************************************************************/

Token
Lexer::plusOrIncrement ()
{
  getChar ();
  if (m_currentChar != '+')
  {
    ungetChar ();
    return Token (PLUS, "+", m_lineNum, m_colNum);
  }
  return Token (INCREMENT, "++", m_lineNum, m_colNum);
}

/************************************************************/

bool
Lexer::isDivision ()
{
  getChar ();
  if (m_currentChar != '*' && m_currentChar != '/')
  {
    ungetChar ();
    return true;
  }
  return false;
}

/************************************************************/

void
Lexer::whichTypeOfComment ()
{
  if (m_currentChar == '*')
  {
    eatMultiLineComment ();
  }

  if (m_currentChar == '/')
  {
    eatOneLineComment ();
  }
}

/************************************************************/

void
Lexer::eatOneLineComment ()
{
  while (m_currentChar != '\n')
  {
    getChar ();
  }
  ++m_lineNum;
  m_colNum = -1;
}

/************************************************************/

void
Lexer::eatMultiLineComment ()
{
  bool isComment = true;

  while (isComment && m_currentChar != EOF)
  {
    getChar ();

    if (m_currentChar == '\n')
    {
      ++m_lineNum;
      m_colNum = -1;
    }

    while (m_currentChar == '*')
    {
      getChar ();

      if (m_currentChar == '\n')
      {
        ++m_lineNum;
        m_colNum = -1;
      }

      if (m_currentChar == '/')
      {
        isComment = false;
        break;
      }
      continue;
    }
  }
}

/************************************************************/

Token
Lexer::lessThanOrLTE ()
{
  getChar ();
  if (m_currentChar != '=')
  {
    ungetChar ();
    return Token (LT, "<", m_lineNum, m_colNum);
  }
  return Token (LTE, "<=", m_lineNum, m_colNum);
}

/************************************************************/

Token
Lexer::greaterThanOrGTE ()
{
  getChar ();
  if (m_currentChar != '=')
  {
    ungetChar ();
    return Token (GT, ">", m_lineNum, m_colNum);
  }
  return Token (GTE, ">=", m_lineNum, m_colNum);
}

/************************************************************/

Token
Lexer::equalOrAssign ()
{
  getChar ();
  if (m_currentChar != '=')
  {
    ungetChar ();
    return Token (ASSIGN, "=", m_lineNum, m_colNum);
  }
  return Token (EQ, "==", m_lineNum, m_colNum);
}

/************************************************************/

Token
Lexer::isItNEQ ()
{
  getChar ();
  if (m_currentChar != '=')
  {
    ungetChar ();
    return Token (ERROR, "!", m_lineNum, m_colNum);
  }
  return Token (NEQ, "!=", m_lineNum, m_colNum);
}

/************************************************************/

void
Lexer::processLexId (Token & currentToken)
{
  string lexeme = "";
  while (isalnum (m_currentChar))
  {
    lexeme += m_currentChar;
    getChar ();
  }
  ungetChar ();
  currentToken.type = ID;
  currentToken.lexeme = lexeme;
  currentToken.lineNum = m_lineNum;
  currentToken.colNum = m_colNum;

}

/************************************************************/

void
Lexer::isItKeyword (Token & currentToken)
{
  std::map < std::string, TokenType> mapOfKeywords =
  {
    { "if", IF},
    { "else", ELSE},
    { "int", INT},
    { "void", VOID},
    { "return", RETURN},
    { "while", WHILE}
  };

  if (mapOfKeywords.count(currentToken.lexeme))
  {
    auto keyword = mapOfKeywords.find(currentToken.lexeme);
    currentToken.type = keyword -> second;
  }
}

/************************************************************/

void
Lexer::processLexNum (Token & currentToken)
{
  string lexeme = "";
  {
    while (isdigit (m_currentChar))
    {
      lexeme += m_currentChar;
      getChar ();
    }

    ungetChar ();
  }

  currentToken.type = NUM;
  currentToken.lexeme = lexeme;
  currentToken.number = stol (lexeme);
  currentToken.lineNum = m_lineNum;
  currentToken.colNum = m_colNum;
}

/************************************************************/

void
Lexer::processError (Token & currentToken)
{
  currentToken.type = ERROR;
  currentToken.lexeme += m_currentChar;
  currentToken.lineNum = m_lineNum;
  currentToken.colNum = m_colNum;
}

/************************************************************/
