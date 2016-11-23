/*
  Filename   : Visitor.h
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/********************************************************************/
// C- Visitor Header File
// Design by CSCI 435: Compilers class
// Fall 2014
// Fall 2016

#ifndef VISITOR_H
#define VISITOR_H

/********************************************************************/
// System includes

#include <fstream>

/********************************************************************/
// Local Includes

#include "Ast.h"

/********************************************************************/
// Using declarations

using std::ofstream;

/********************************************************************/
// Abstract Classes

class IVisitor
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


class PrintVisitor : public IVisitor
{
public:
  virtual void visit (ProgramNode* node);

  virtual void visit (DeclarationNode* node){};
  virtual void visit (FunctionDeclarationNode* node); 
  virtual void visit (VariableDeclarationNode* node); 
  virtual void visit (ArrayDeclarationNode* node); 
  virtual void visit (ParameterNode* node); 

  virtual void visit (StatementNode* node){}; 
  virtual void visit (CompoundStatementNode* node); 
  virtual void visit (IfStatementNode* node); 
  virtual void visit (WhileStatementNode* node);
  virtual void visit (ReturnStatementNode* node); 
  virtual void visit (ExpressionStatementNode* node); 

  virtual void visit (ExpressionNode* node){}; 
  virtual void visit (AssignmentExpressionNode* node); 
  virtual void visit (VariableExpressionNode* node); 
  virtual void visit (SubscriptExpressionNode* node); 
  virtual void visit (CallExpressionNode* node); 
  virtual void visit (AdditiveExpressionNode* node); 
  virtual void visit (MultiplicativeExpressionNode* node);
  virtual void visit (RelationalExpressionNode* node);
  virtual void visit (IntegerLiteralExpressionNode* node); 

  void 
  printLevel();

  void
  printNodeInfo (string nodeType, auto node);

  void
  printArithmeticInfo (string nodeType, string op, auto node);

  size_t    level = 0;
  ofstream  file;
};

#endif