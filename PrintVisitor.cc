
#include "PrintVisitor.h"
#include <iostream>

using std::cout;
using std::endl;

void
PrintVisitor::visit (ProgramNode* node)
{
  level = 0;
  printLevel ();
  cout << "Program" << endl;
  ++level;
  for (auto child : node -> declarations)
    {
      child -> accept (this);
    }
  --level;
  cout << "AST Complete!" << endl;
}

void
PrintVisitor::visit (VariableDeclarationNode* node)
{
  printLevel ();
  printNodeInfo ("Variable Declaration", node);
}

void
PrintVisitor::visit (FunctionDeclarationNode* node)
{
  printLevel ();
  printNodeInfo ("Function", node);

  ++level;
  for (auto child : node -> parameters)
    {
      child -> accept (this);
    }
  --level;

  node -> functionBody -> accept (this);
}

void
PrintVisitor::visit (ArrayDeclarationNode* node)
{
  printLevel ();
  printNodeInfo ("Variable Declaration", node);
}
void
PrintVisitor::visit (ParameterNode* node)
{
  printLevel ();
  printNodeInfo ("Parameter", node);

}

void
PrintVisitor::visit (CompoundStatementNode* node)
{
  printLevel ();
  cout << "Compound Statement:" << endl;

  ++level;
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
  printLevel ();
  cout << "If" << endl;
  ++level;
  node -> conditionalExpression -> accept (this);
  node -> thenStatement -> accept (this);
  node -> elseStatement -> accept (this);
  --level;
}

void
PrintVisitor::visit (ForStatementNode* node)
{
  printLevel ();
  cout << "For" << endl;
  node -> initializer -> accept (this);
  node -> condition -> accept (this);
  node -> updater -> accept (this);
  node -> body -> accept (this);
}

void
PrintVisitor::visit (WhileStatementNode* node)
{
  printLevel ();
  cout << "While" << endl;
  node -> conditionalExpression -> accept (this);
  node -> body -> accept (this);
}

void
PrintVisitor::visit (ReturnStatementNode* node)
{
  printLevel ();
  cout << "Return" << endl;
  node -> expression -> accept (this);
}

void
PrintVisitor::visit (ExpressionStatementNode* node)
{
  printLevel ();
  cout << "Expression Statement: " << endl;
  ++level;
  node -> expression -> accept (this);
  --level;
}

void
PrintVisitor::visit (AssignmentExpressionNode* node)
{
  printLevel ();
  cout << "Assignment:" << endl;
  ++level;
  node -> variable -> accept (this);
  node -> expression -> accept (this);
  --level;
}

void
PrintVisitor::visit (VariableExpressionNode* node)
{
  printLevel ();
  cout << "Variable: " << node -> identifier << endl;
}

void
PrintVisitor::visit (SubscriptExpressionNode* node)
{
  printLevel ();
  node -> index -> accept (this);
}

void
PrintVisitor::visit (CallExpressionNode* node)
{
  printLevel ();
  cout << "Call" << endl;
  cout << node -> identifier << endl;
    for (auto child : node -> arguments)
    {
      child -> accept (this);
    }
}

void
PrintVisitor::visit (AdditiveExpressionNode* node)
{
  printLevel ();
  string op = "";
  switch (node -> addOperator)
    {
    case AdditiveOperatorType::PLUS:
      op = "+";
      break;
    case AdditiveOperatorType::MINUS:
      op = "-";
      break;
    }
  cout << "Additive Expression: "<< op << endl;
  node -> left -> accept (this);
  node -> right -> accept (this);
}

void
PrintVisitor::visit (MultiplicativeExpressionNode* node)
{
  printLevel ();
  string op = "";
  switch (node -> multOperator)
    {
    case MultiplicativeOperatorType::TIMES:
      op = "*";
      break;
    case MultiplicativeOperatorType::DIVIDE:
      op = "/";
      break;
    }
  cout << "Multiplicative Expression: "<< op << endl;
  ++level;
  node -> left -> accept (this);
  node -> right -> accept (this);
  --level;
}

void
PrintVisitor::visit (RelationalExpressionNode* node)
{
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
  ++level;
  node -> left -> accept (this);
  --level;
  printLevel ();
  cout << "Right:" << endl;
  ++level;
  node -> right -> accept (this);
  --level;
  --level;
}

void
PrintVisitor::visit (UnaryExpressionNode* node)
{
  printLevel ();
  //cout << node -> unaryOperator;
  cout << node -> variable << endl;
}

void
PrintVisitor::visit (IntegerLiteralExpressionNode* node)
{
    printLevel ();
    cout << "Integer: "<< node -> value << endl;
}

void
PrintVisitor::printLevel ()
{
  for (size_t i = 0; i < level; ++i)
    {
      cout << "|  ";
    }
}

void
printNodeInfo (string nodeType, auto node)
{
  cout << nodeType << ": " << node -> identifier << ": ";
  string type = (node -> valueType == ValueType::INT) ? "Int" : "void";
  cout << type << " type" << endl;
}
