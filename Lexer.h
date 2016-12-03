/*
  Filename   : Lexer.h
  Author     : Eric Dougherty
  Course     : CSCI 435
  Assignment : Lab 9 - Can't Parse This
*/

/***********************************************************************/

#ifndef LEXER_H
#define LEXER_H

/***********************************************************************/
// System includes
  
#include <string>
#include <map>

/***********************************************************************/
// Using declarations

using std::string;
using std::map;

/***********************************************************************/

enum TokenType
{
    // Special tokens
    END_OF_FILE, ERROR,

    // Keywords
    IF, ELSE, INT, VOID, RETURN, WHILE,

    // Operators
    PLUS, MINUS, TIMES, DIVIDE, LT, LTE, GT, GTE, EQ, NEQ, ASSIGN,

    INCREMENT, DECREMENT,

    // Punctuators
    SEMI, COMMA, LPAREN, RPAREN, LBRACK, RBRACK, LBRACE, RBRACE,

    // Identifier and integer literals
    ID, NUM
};

/***********************************************************************/

struct Token
{
    Token (TokenType pType = END_OF_FILE,
        std::string pLexeme = "",
        int pNumber = 0)
    : type (pType), lexeme (pLexeme), number (pNumber)
  {  }

    TokenType   type;
    std::string lexeme;
    int         number;
};

/***********************************************************************/

class Lexer
{
public:
    Lexer (FILE* sourceFile);

    Token
    getToken ();

    size_t 
    getLines ();

    size_t  
    getColumns ();

private:
    void
    getChar ();

    void
    ungetChar (int c);

    Token
    inId ();

    Token
    inNum ();

    Token
    inErr ();

    bool
    inComment ();

    TokenType
    isKeyword (const string& s);

private:
    FILE* m_sourceFile;
    int   m_nextChar;
    size_t    m_lines;
    size_t    m_columns;
    const map<string, TokenType> m_keywords = { {"if", IF}, {"else", ELSE}, {"int", INT}, {"void", VOID}, {"return", RETURN}, {"while", WHILE} };

};

/***********************************************************************/

#endif