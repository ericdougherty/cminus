/*
  Filename   : Parser.h
  Author     : Eric Dougherty
  Course     : CSCI 435
*/

/***********************************************************************/

#ifndef PARSER_H
#define PARSER_H

/***********************************************************************/
// System includes

#include <string>
#include <vector>

/***********************************************************************/
// Local includes
  
#include "Ast.h"
#include "Lexer.h"

/***********************************************************************/
// Using declarations
  
using std::string;
using std::vector;

/***********************************************************************/

class Parser
{
public:
    Parser (FILE* file);

    bool
    parse ();

    void
    print (string name);

    bool
    symbolTable ();

    void
    codeGen (string name);

private:    
    void
    declarations (auto parent);
    
    void
    variableDeclaration (ValueType type, string id, auto parent);
    
    ValueType
    typeSpecifier ();
    
    void
    functionDeclaration (ValueType type, string id);
    
    void
    params (vector<ParameterNode*> & paramVec);
    
    void
    paramList (vector<ParameterNode*> & paramVec);
    
    void
    param (vector<ParameterNode*> & paramVec);
    
    CompoundStatementNode*
    compoundStmt ();
    
    void
    localDeclarations (auto parent);
    
    void
    statementList (auto parent);
    
    StatementNode*
    statement ();
    
    ExpressionStatementNode*
    expressionStmt ();

    IfStatementNode*
    selectionStmt ();
    
    WhileStatementNode*
    iterationStmt ();
    
    ReturnStatementNode*
    returnStmt ();
    
    ExpressionNode*
    expression ();
    
    VariableExpressionNode*
    var (string id);
    
    ExpressionNode*
    simpleExpression ();
    
    RelationalOperatorType
    relop ();
    
    ExpressionNode*
    additiveExpression ();
    
    AdditiveOperatorType
    addop ();
    
    ExpressionNode*
    term ();
    
    MultiplicativeOperatorType
    mulop ();
    
    ExpressionNode*
    factor ();
    
    CallExpressionNode*
    call (string id);
    
    vector<ExpressionNode*> 
    args ();
    
    void
    argList (vector<ExpressionNode*> & args);

    string
    match (Token expectedToken, string possibleTokens, string caller);
    
    void
    error (string possibleTokens, string caller);

    

private:
    Lexer   m_lexer;
    Token   m_token;
    bool    m_matchedID;
    string  m_callID;
    VariableExpressionNode* m_varNode;
    ProgramNode ast;
};

/***********************************************************************/

#endif