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