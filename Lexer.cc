/*
  Filename   : Lexer.cc
  Author     : Eric Dougherty
  Course     : CSCI 435
  Assignment : Lab 9 - Can't Parse This
*/

/***********************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>

/***********************************************************************/
// Local includes

#include "Lexer.h"

/***********************************************************************/
// Using declarations

using std::cout;
using std::endl;
using std::string;
using std::stoi;
using std::strchr;

/***********************************************************************/
// Function prototypes/global vars/typedefs
Lexer::Lexer (FILE* sourceFile)
    :m_sourceFile (sourceFile), m_lines(1)

{  }

Token
Lexer::getToken () 
{   
    while (true)
    {
        getChar();

        char c = m_nextChar;

        if (isalpha(c))
        {
            return inId();
        }

        if (isdigit(c))
        {
            return inNum();
        }

        switch (c)
        {
            case ' ':
                break;

            case '\t':
                break;
            
            case '\n':
                ++m_lines;
                m_columns = 0;
                break;

            case EOF:
                return Token (END_OF_FILE);

            //OPERATORS
            case '+':
                getChar();
                c = m_nextChar;
                if (c != '+')
                {
                    ungetChar(m_nextChar);
                    return Token (PLUS, "+");
                }
                return Token (INCREMENT, "++");  

            case '-':
                getChar();
                c = m_nextChar;
                if (c != '-')
                {
                    ungetChar(m_nextChar);
                    return Token (MINUS, "-");
                }
                return Token (DECREMENT, "--");

            case '*':
                return Token (TIMES, "*");    

            case '/':
                getChar();
                c = m_nextChar;
                if (c != '*')
                {
                    ungetChar(m_nextChar);
                    return Token (DIVIDE, "/");
                }
                if (inComment())
                {
                    break;
                }
                return Token (ERROR, "Unclosed comment");

            case '<':
                getChar();
                c = m_nextChar;
                if (c != '=')
                {
                    ungetChar(m_nextChar);
                    return Token (LT, "<");
                }
                return Token (LTE, "<=");

            case '>':
                getChar();
                c = m_nextChar;
                if (c != '=')
                {
                    ungetChar(m_nextChar);
                    return Token (GT, "<");
                }
                return Token (GTE, ">=");

            case '=':
                getChar();
                c = m_nextChar;
                if (c != '=')
                {
                    ungetChar(m_nextChar);
                    return Token (ASSIGN, "=");
                }
                return Token (EQ, "==");

            case '!':
                getChar();
                c = m_nextChar;
                if (c != '=')
                {
                    ungetChar(m_nextChar);
                    return Token (ERROR);
                }
                return Token (NEQ, "!="); 

            //PUNCTUATORS
            case ';':
                return Token (SEMI, ";");
            
            case ',':
                return Token (COMMA, ",");

            case '(':
                return Token (LPAREN, "(");    

            case ')':
                return Token (RPAREN, ")");

            case '[':
                return Token (LBRACK, "[");    

            case ']':
                return Token (RBRACK, "]");   

            case '{':
                return Token (LBRACE, "{");    

            case '}':
                return Token (RBRACE, "}");  
            
            default:
                return inErr (); 
        }
    }
}

void
Lexer::getChar ()
{
    m_nextChar = getc (m_sourceFile);
    ++m_columns;
}

void
Lexer::ungetChar (int c)
{
    ungetc (c, m_sourceFile);
    --m_columns;
}

size_t 
Lexer::getLines ()
{
    return m_lines;
}

size_t 
Lexer::getColumns ()
{
    return m_columns;
}


Token
Lexer::inId ()
{   
    string lexeme(1, (char) m_nextChar);
    
    while (true)
    {
        getChar ();
        if (!isalnum (m_nextChar)) 
        {
            ungetChar (m_nextChar);
            break;
        }
        lexeme += (char) m_nextChar;
    }

    return Token (isKeyword(lexeme), lexeme);
}

Token
Lexer::inNum ()
{
    string lexeme(1, (char) m_nextChar);
    
    while (true)
    {
        getChar ();
        if (!isdigit (m_nextChar)) 
        {
            ungetChar (m_nextChar);
            break;
        }
        lexeme += (char) m_nextChar;
    }
    
    return Token (NUM, lexeme, stoi(lexeme));
}

Token
Lexer::inErr ()
{   
    string lexeme(1, (char) m_nextChar);
    
    while (true)
    {
        getChar ();
        char c = m_nextChar;

        if ( isspace(c) || isalnum (c) || strchr("+-*/><=!;,()[]{}", c) || c == EOF) 
        {
            ungetChar (m_nextChar);
            break;
        }
        lexeme += (char) m_nextChar;
    }

    return Token (ERROR, lexeme);
}

bool
Lexer::inComment ()
{   
    char c;
    do
    {
        getChar();
        c = m_nextChar;
        if (c == '\n')
        {
            ++m_lines;
            m_columns = 0;
        }
        else if (c == '*')
        {
            getChar();
            c = m_nextChar;
            if (c != '/')
            {
                ungetChar (m_nextChar);
            }
            else
            {
                return true;
            }
        } 
    }
    while(c != EOF);

    return false;
}

TokenType
Lexer::isKeyword (const string& s)
{
    if (m_keywords.find (s) != m_keywords.end ())
    {
        return m_keywords.at (s);
    }
    return ID;
}