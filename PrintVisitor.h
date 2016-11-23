#ifndef PRINT_VISITOR_H
#define PRINT_VISITOR_H

#include "CMinusAst.h"

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
  virtual void visit (ForStatementNode* node); 
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
  virtual void visit (UnaryExpressionNode* node); 
  virtual void visit (IntegerLiteralExpressionNode* node); 

  void printLevel();

  size_t level;
};

void printNodeInfo (string nodeType, auto node);

#endif
