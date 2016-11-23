/********************************************************************/


/********************************************************************/
// System Includes

#include <iostream>

/***********************************************************************/
// Local includes

#include "Visitor.h"

/********************************************************************/
// Using Declarations

using std::endl;

/********************************************************************/

void
PrintVisitor::visit (ProgramNode* node)
{
  file << "Program Node:" << endl;
  for (auto child : node -> declarations)
    {
      child -> accept (this);
    }
}

void
PrintVisitor::visit (VariableDeclarationNode* node)
{
  printLevel ();
  printNodeInfo ("Variable Declaration", node);
  --level;
}

void
PrintVisitor::visit (FunctionDeclarationNode* node)
{
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
  printLevel ();
  string type = (node -> valueType == ValueType::INT) ? "Int" : "void";
  file << "Variable Declaration: " << node -> identifier ;
  file << "[" << node -> size << "]: " << type << " type" << endl;
  --level;
}

void
PrintVisitor::visit (ParameterNode* node)
{
  printLevel ();
  string type = (node -> valueType == ValueType::INT) ? "Int" : "void";
  string array = (node -> isArray == true) ? "[]: " : ": ";
  file << "Parameter: " << node -> identifier << array << type << " type" << endl;
  --level;
}

void
PrintVisitor::visit (CompoundStatementNode* node)
{
  printLevel ();
  file << "Compound Statement:" << endl;
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
  file << "If" << endl;
  node -> conditionalExpression -> accept (this);
  node -> thenStatement -> accept (this);
  node -> elseStatement -> accept (this);
  --level;
}

void
PrintVisitor::visit (ForStatementNode* node)
{
  printLevel ();
  file << "For" << endl;
  node -> initializer -> accept (this);
  node -> condition -> accept (this);
  node -> updater -> accept (this);
  node -> body -> accept (this);
  --level;
}

void
PrintVisitor::visit (WhileStatementNode* node)
{
  printLevel ();
  file << "While" << endl;
  node -> conditionalExpression -> accept (this);
  node -> body -> accept (this);
  --level;
}

void
PrintVisitor::visit (ReturnStatementNode* node)
{
  printLevel ();
  file << "Return" << endl;
  node -> expression -> accept (this);
  --level;
}

void
PrintVisitor::visit (ExpressionStatementNode* node)
{
  printLevel ();
  file << "Expression Statement: " << endl;
  node -> expression -> accept (this);
  --level;
}

void
PrintVisitor::visit (AssignmentExpressionNode* node)
{
  printLevel ();
  file << "Assignment:" << endl;
  node -> variable -> accept (this);
  node -> expression -> accept (this);
  --level;
}

void
PrintVisitor::visit (VariableExpressionNode* node)
{
  printLevel ();
  file << "Variable: " << node -> identifier << endl;
  --level;
}

void
PrintVisitor::visit (SubscriptExpressionNode* node)
{
  printLevel ();
  file << "Subscript: " << node -> identifier << endl;
  ++level;
  printLevel ();
  file << "Index:" << endl;
  node -> index -> accept (this);
  level -= 2;
}

void
PrintVisitor::visit (CallExpressionNode* node)
{
  printLevel ();
  file << "Function Call: " << node -> identifier << endl;
  ++level;
  printLevel ();
  file << "Arguments:" << endl;
  for (auto child : node -> arguments)
  {
    child -> accept (this);
  }
  level -= 2;
}

void
PrintVisitor::visit (AdditiveExpressionNode* node)
{
  printLevel ();
  string op = (node -> addOperator == AdditiveOperatorType::PLUS) ? "+" : "-";
  printArithmeticInfo ("AdditiveExpression", op, node);
}

void
PrintVisitor::visit (MultiplicativeExpressionNode* node)
{
  printLevel ();
  string op = (node -> multOperator == MultiplicativeOperatorType::TIMES) ? "*" : "/";
  printArithmeticInfo ("MultiplicativeExpression", op, node);
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
  printArithmeticInfo ("RelationalExpression", op, node);
}

void
PrintVisitor::visit (UnaryExpressionNode* node)
{
  printLevel ();
  //file << node -> unaryOperator;
  file << node -> variable << endl;
  --level;
}

void
PrintVisitor::visit (IntegerLiteralExpressionNode* node)
{
  printLevel ();
  file << "Integer: "<< node -> value << endl;
  --level;
}

void
PrintVisitor::printNodeInfo (string nodeType, auto node)
{
  file << nodeType << ": " << node -> identifier << ": ";
  string type = (node -> valueType == ValueType::INT) ? "Int" : "void";
  file << type << " type" << endl;
}

void
PrintVisitor::printArithmeticInfo (string nodeType, string op, auto node)
{
  ++level;
  file << nodeType << ": "<< op << endl;
  printLevel ();
  file << "Left:" << endl;
  node -> left -> accept (this);
  printLevel ();
  file << "Right:" << endl;
  node -> right -> accept (this);
  level -= 2;
}

void
PrintVisitor::printLevel ()
{
  ++ level;
  for (size_t i = 0; i < level; ++i)
    {
      file << "|  ";
    }
}
