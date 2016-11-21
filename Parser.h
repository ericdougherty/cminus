/*
  Filename   : Parser.h
  Author     : Eric Dougherty
  Course     : CSCI 435
  Assignment : Lab 9 - Can't Parse This
*/

/***********************************************************************/

#ifndef PARSER_H
#define PARSER_H

/***********************************************************************/
// System includes

#include <string>
#include <vector>
#include "CMinusAst.h"
#include "Lexer.h"

/***********************************************************************/
// Using declarations
  
using std::string;
using std::vector;

/***********************************************************************/

class Parser
{
public:
    Parser (FILE* file, bool debug, vector<DeclarationNode*> declarations);

    void
    parse ();

private:
    void
    declarationList ();
    
    void
    declaration ();
    
    void
    variableDeclaration ();
    
    void
    typeSpecifier ();
    
    void
    functionDeclaration (vector<ParameterNode*> & paramVec);
    
    void
    params (vector<ParameterNode*> & paramVec);
    
    void
    paramList (vector<ParameterNode*> & paramVec);
    
    void
    param (vector<ParameterNode*> & paramVec);
    
    void
    compoundStmt ();
    
    void
    localDeclarations ();
    
    void
    statementList ();
    
    void
    statement ();
    
    void
    expressionStmt ();

    void
    selectionStmt ();
    
    void
    iterationStmt ();
    
    void
    returnStmt ();
    
    void
    expression ();
    
    void
    var ();
    
    void
    simpleExpression ();
    
    void
    relop ();
    
    void
    additiveExpression ();
    
    void
    addop ();
    
    void
    term ();
    
    void
    mulop ();
    
    void
    factor ();
    
    void
    call ();
    
    void
    args ();
    
    void
    argList ();

    void
    match (Token expectedToken, string possibleTokens, string caller);
    
    void
    error (string possibleTokens, string caller);

    void 
    enter (string functionName);

    void 
    leave (string functionName);

    void 
    printSpaces (int spaces);

    

private:
    //current token
    Token   m_token;
    //Lexer object
    Lexer   m_lexer;
    //Run in debug mode
    bool    m_debug;
    //Flag for an already matched ID
    bool    m_matchedID;
    //Indent level for debugging mode
    int     m_indent;
    ProgramNode root;
    //Vector of token types
    const vector<string> m_tokenList = {"EOF", "ERROR", "IF", "ELSE", "INT", "VOID", "RETURN", "WHILE", "PLUS", "MINUS", "TIMES", "DIVIDE", "LT", "LTE", "GT", "GTE", "EQ", "NEQ", "ASSIGN", "INCREMENT", "DECREMENT", "SEMI", "COMMA", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "ID", "NUM"};
};

/***********************************************************************/

#endif