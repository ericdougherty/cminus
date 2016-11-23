/********************************************************************/


/********************************************************************/
// System Includes

#include <iostream>

/***********************************************************************/
// Local includes

#include "Visitor.h"

/********************************************************************/
// Using Declarations

using std::cout;
using std::endl;

/********************************************************************/

void
PrintVisitor::visit (ProgramNode* node)
{
  level = 0;
  cout << "Program Node:" << endl;
  for (auto child : node -> declarations)
    {
      child -> accept (this);
    }
}

void
PrintVisitor::visit (VariableDeclarationNode* node)
{
  ++level;
  printLevel ();
  printNodeInfo ("Variable Declaration", node);
  --level;
}

void
PrintVisitor::visit (FunctionDeclarationNode* node)
{
  ++level;
  printLevel ();
  printNodeInfo ("Function", node);

  for (auto child : node -> parameters)
    {
      child -> accept (this);
    }

  node -> functionBody -> accept (this);
  --level;
}

void
PrintVisitor::visit (ArrayDeclarationNode* node)
{
  ++level;
  printLevel ();
  string type = (node -> valueType == ValueType::INT) ? "Int" : "void";
  
  cout << "Variable Declaration: " << node -> identifier ;
  cout << "[" << node -> size << "]: " << type << " type" << endl;
  --level;
}

void
PrintVisitor::visit (ParameterNode* node)
{
  ++level;
  printLevel ();
  string type = (node -> valueType == ValueType::INT) ? "Int" : "void";
  string array = (node -> isArray == true) ? "[]: " : ": ";
  cout << "Parameter: " << node -> identifier << array << type << " type" << endl;
  --level;
}

void
PrintVisitor::visit (CompoundStatementNode* node)
{
  ++level;
  printLevel ();
  cout << "Compound Statement:" << endl;
  for (auto child : node -> declarations)
    {
      child -> accept (this);
    }
    
  for (auto child : node -> statements)
    {
      child -> accept (this);
    }
  --level;
}

void
PrintVisitor::visit (IfStatementNode* node)
{
  ++level;
  printLevel ();
  cout << "If" << endl;
  node -> conditionalExpression -> accept (this);
  node -> thenStatement -> accept (this);
  node -> elseStatement -> accept (this);
  --level;
}

void
PrintVisitor::visit (ForStatementNode* node)
{
  ++level;
  printLevel ();
  cout << "For" << endl;
  node -> initializer -> accept (this);
  node -> condition -> accept (this);
  node -> updater -> accept (this);
  node -> body -> accept (this);
  --level;
}

void
PrintVisitor::visit (WhileStatementNode* node)
{
  ++level;
  printLevel ();
  cout << "While" << endl;
  node -> conditionalExpression -> accept (this);
  node -> body -> accept (this);
  --level;
}

void
PrintVisitor::visit (ReturnStatementNode* node)
{
  ++level;
  printLevel ();
  cout << "Return" << endl;
  node -> expression -> accept (this);
  --level;
}

void
PrintVisitor::visit (ExpressionStatementNode* node)
{
  ++level;
  printLevel ();
  cout << "Expression Statement: " << endl;
  node -> expression -> accept (this);
  --level;
}

void
PrintVisitor::visit (AssignmentExpressionNode* node)
{
  ++level;
  printLevel ();
  cout << "Assignment:" << endl;
  node -> variable -> accept (this);
  node -> expression -> accept (this);
  --level;
}

void
PrintVisitor::visit (VariableExpressionNode* node)
{
  ++level;
  printLevel ();
  cout << "Variable: " << node -> identifier << endl;
  --level;
}

void
PrintVisitor::visit (SubscriptExpressionNode* node)
{
  ++level;
  printLevel ();
  cout << "Subscript: " << node -> identifier << endl;
  ++level;
  printLevel ();
  cout << "Index:" << endl;
  node -> index -> accept (this);
  level -= 2;
}

void
PrintVisitor::visit (CallExpressionNode* node)
{
  ++level;
  printLevel ();
  cout << "Function Call: " << node -> identifier << endl;
  ++level;
  printLevel ();
  cout << "Arguments:" << endl;
  for (auto child : node -> arguments)
  {
    child -> accept (this);
  }
  level -= 2;
}

void
PrintVisitor::visit (AdditiveExpressionNode* node)
{
  ++level;
  printLevel ();
  string op = (node -> addOperator == AdditiveOperatorType::PLUS) ? "+" : "-";
  ++level;
  cout << "Additive Expression: "<< op << endl;
  printLevel ();
  cout << "Left:" << endl;
  node -> left -> accept (this);
  printLevel ();
  cout << "Right:" << endl;
  node -> right -> accept (this);
  level -= 2;
}

void
PrintVisitor::visit (MultiplicativeExpressionNode* node)
{
  ++level;
  printLevel ();
  string op = (node -> multOperator == MultiplicativeOperatorType::TIMES) ? "*" : "/";
  cout << "Multiplicative Expression: "<< op << endl;
  ++level;
  printLevel ();
  cout << "Left:" << endl;
  node -> left -> accept (this);
  printLevel ();
  cout << "Right:" << endl;
  node -> right -> accept (this);
  level -= 2;
}

void
PrintVisitor::visit (RelationalExpressionNode* node)
{
  ++level;
  printLevel ();
  string op = "";
  switch (node -> relationalOperator)
    {
    case RelationalOperatorType::LT:
      op = "<";
      break;
    case RelationalOperatorType::LTE:
      op = "<=";
      break;
    case RelationalOperatorType::GT:
      op = ">";
      break;
    case RelationalOperatorType::GTE:
      op = ">=";
      break;
    case RelationalOperatorType::EQ:
      op = "==";
      break;
    case RelationalOperatorType::NEQ:
      op = "!=";
      break;
    }
  cout << "Relational Expression: " << op << endl;
  ++level;
  printLevel ();
  cout << "Left:" << endl;
  node -> left -> accept (this);
  printLevel ();
  cout << "Right:" << endl;
  node -> right -> accept (this);
  level -= 2;
}

void
PrintVisitor::visit (UnaryExpressionNode* node)
{
  ++level;
  printLevel ();
  //cout << node -> unaryOperator;
  cout << node -> variable << endl;
  --level;
}

void
PrintVisitor::visit (IntegerLiteralExpressionNode* node)
{
  ++level;
  printLevel ();
  cout << "Integer: "<< node -> value << endl;
  --level;
}

void
PrintVisitor::printNodeInfo (string nodeType, auto node)
{
  cout << nodeType << ": " << node -> identifier << ": ";
  string type = (node -> valueType == ValueType::INT) ? "Int" : "void";
  cout << type << " type" << endl;
}

void
PrintVisitor::printLevel ()
{
  for (size_t i = 0; i < level; ++i)
    {
      cout << "|  ";
    }
}