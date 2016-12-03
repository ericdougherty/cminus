/*
  Filename   : Visitor.cc
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/
/********************************************************************/
// System includes

#include <iostream>

/********************************************************************/
// Local includes

#include "SymbolTableVisitor.h"

/********************************************************************/
// Using Declarations

using std::cout;
using std::endl;
using std::unique_ptr;

/********************************************************************/

void
SymbolTableVisitor::visit (ProgramNode* node)
{
  table.enterScope ();
  FunctionDeclarationNode* input  = new FunctionDeclarationNode(ValueType::VOID, "input");
  FunctionDeclarationNode* output = new FunctionDeclarationNode(ValueType::VOID, "output");
  insert(input);
  insert(output);
  for (auto child : node -> declarations)
  {
    child -> accept (this);
  }
  table.exitScope ();
}

void
SymbolTableVisitor::visit (VariableDeclarationNode* node)
{
  node -> nestLevel = table.getLevel ();
  insert (node);
}

void
SymbolTableVisitor::visit (FunctionDeclarationNode* node)
{
	node -> nestLevel = table.getLevel();
  insert (node);
	table.enterScope ();
  for (auto child : node -> parameters)
  {

    child -> accept (this);
  }
  node -> functionBody -> accept (this);
  table.exitScope ();
}

void
SymbolTableVisitor::visit (ArrayDeclarationNode* node)
{
  node -> nestLevel = table.getLevel();
  insert (node);
}

void
SymbolTableVisitor::visit (ParameterNode* node)
{
  node -> nestLevel = table.getLevel();
  insert (node);
}

void
SymbolTableVisitor::visit (CompoundStatementNode* node)
{
  for (auto child : node -> declarations)
  {
    child -> accept (this);
  }

  for (auto child : node -> statements)
  {
    child -> accept (this);
  }
}

void
SymbolTableVisitor::visit (IfStatementNode* node)
{
  table.enterScope ();
  node -> conditionalExpression -> accept (this);
  node -> thenStatement -> accept (this);
  table.exitScope ();
  table.enterScope ();
  node -> elseStatement -> accept (this);
  table.exitScope ();
}

void
SymbolTableVisitor::visit (WhileStatementNode* node)
{
  table.enterScope ();
  node -> conditionalExpression -> accept (this);
  node -> body -> accept (this);
  table.exitScope ();
}

void
SymbolTableVisitor::visit (ReturnStatementNode* node)
{
  node -> expression -> accept (this);
}

void
SymbolTableVisitor::visit (ExpressionStatementNode* node)
{
  node -> expression -> accept (this);
}

void
SymbolTableVisitor::visit (AssignmentExpressionNode* node)
{
  node -> variable -> accept (this);
  node -> expression -> accept (this);
}

void
SymbolTableVisitor::visit (VariableExpressionNode* node)
{
lookup (node, " variable has not been declared.");
}

void
SymbolTableVisitor::visit (SubscriptExpressionNode* node)
{
  lookup (node, " array has not been declared.");
  node -> index -> accept (this);
}

void
SymbolTableVisitor::visit (CallExpressionNode* node)
{

  lookup (node, " function has not been declared.");
  for (auto child : node -> arguments)
  {
    child -> accept (this);
  }
}

void
SymbolTableVisitor::visit (AdditiveExpressionNode* node)
{
  node -> left -> accept (this);
  node -> right -> accept (this);
}

void
SymbolTableVisitor::visit (MultiplicativeExpressionNode* node)
{
  node -> left -> accept (this);
  node -> right -> accept (this);
}

void
SymbolTableVisitor::visit (RelationalExpressionNode* node)
{
  node -> left -> accept (this);
  node -> right -> accept (this);
}

void
SymbolTableVisitor::error(auto node,string errorMessage)
{
  cout << node -> identifier << errorMessage << endl;
  hasErrors = true;
}

/********************************************************************/

bool
SymbolTableVisitor::getErrors()
{
  return hasErrors;
}

/********************************************************************/

void
SymbolTableVisitor::insert (auto node)
{
  if (!table.insert (node))
  {
    error(node, " on line ?? has already been declared");
  }
}

/********************************************************************/

 void
 SymbolTableVisitor::lookup (auto node, string ifError)
{
  DeclarationNode* retNode = table.lookup(node -> identifier);
  if (retNode == nullptr)
  {
    error(node, ifError);
  }
  else
  {
    node -> decl = retNode;
  }
}