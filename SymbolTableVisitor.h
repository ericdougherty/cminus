//
//  SymbolTableVisitor.hpp
//  SymbolTable
//
//  Created by Ian Murry on 11/27/16.
//
//

#ifndef SymbolTableVisitor_h
#define SymbolTableVisitor_h
/********************************************************************/

#include "Ast.h"

/********************************************************************/

class SymbolTableVisitor : public IVisitor
{
public:
    virtual void visit (ProgramNode* node) = 0;
    
    virtual void visit (DeclarationNode* node) = 0;
    virtual void visit (FunctionDeclarationNode* node) = 0;
    virtual void visit (VariableDeclarationNode* node) = 0;
    virtual void visit (ArrayDeclarationNode* node) = 0;
    virtual void visit (ParameterNode* node) = 0;
    
    virtual void visit (StatementNode* node) = 0;
    virtual void visit (CompoundStatementNode* node) = 0;
    virtual void visit (IfStatementNode* node) = 0;
    virtual void visit (WhileStatementNode* node) = 0;
    virtual void visit (ReturnStatementNode* node) = 0;
    virtual void visit (ExpressionStatementNode* node) = 0;
    
    virtual void visit (ExpressionNode* node) = 0;
    virtual void visit (AssignmentExpressionNode* node) = 0;
    virtual void visit (VariableExpressionNode* node) = 0;
    virtual void visit (SubscriptExpressionNode* node) = 0;
    virtual void visit (CallExpressionNode* node) = 0;
    virtual void visit (AdditiveExpressionNode* node) = 0;
    virtual void visit (MultiplicativeExpressionNode* node) = 0;
    virtual void visit (RelationalExpressionNode* node) = 0;
    virtual void visit (IntegerLiteralExpressionNode* node) = 0;
};


/********************************************************************/

#endif /* SymbolTableVisitor_h */
