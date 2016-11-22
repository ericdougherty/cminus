/********************************************************************/


/********************************************************************/
// System Includes

#include <iostream>
#include "CMinusAst.h"

/********************************************************************/
// Using Declarations

using std::cout;
using std::endl;

/********************************************************************/

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
}

void
PrintVisitor::visit (VariableDeclarationNode* node)
{
  printLevel ();
  printNodeInfo ("Variable", node);
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
}

void
PrintVisitor::visit (ArrayDeclarationNode* node)
{
  printLevel ();
  printNodeInfo ("Variable", node);
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
  printNodeInfo ("Compound", node);

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
  node -> conditionalExpression -> accept (this);
  node -> thenStatement -> accept (this);
  node -> elseStatement -> accept (this);
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
PrintVisitor::visit (AssignmentExpressionNode* node)
{
  printLevel ();
  node -> variable -> accept (this);
  node -> expression -> accept (this);
}

void
PrintVisitor::visit (VariableExpressionNode* node)
{
  printLevel ();
  cout << node -> identifier << endl;
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
  cout << op << endl;
  node -> left -> accept (this);
  node -> right -> accept (this);
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
  cout << op << endl;
  node -> left -> accept (this);
  node -> right -> accept (this);
}

void
PrintVisitor::visit (UrnaryExpressionNode* node)
{
  printLevel ();
  cout << node -> unaryOperator;
  cout << node -> variable << endl;
}

void
PrintVisitor::visit (IntegerLiteralExpressionNode* node)
{
  printLevel ();
  cout << node -> value << endl;
}

void
PrintVisitor::printLevel ()
{
  for (size_t i = 0; i < level; ++i)
    {
      cout << "|   ";
    }
}
/********************************************************************/

Node::~Node ()
{}

/********************************************************************/

ProgramNode::ProgramNode (vector<DeclarationNode*> declarations)
  :declarations (declarations)
{}


ProgramNode::~ProgramNode()
{
  for (auto decl : declarations)
    delete decl;
  declarations.clear ();
}
void
ProgramNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

DeclarationNode::DeclarationNode (ValueType t, string id)
  :valueType (t), identifier (id), nestLevel (0)
{}

DeclarationNode::~DeclarationNode ()
{}

/********************************************************************/

VariableDeclarationNode::VariableDeclarationNode (ValueType t, string id)
  :DeclarationNode (t, id)
{}


VariableDeclarationNode::~VariableDeclarationNode()
{}

void
VariableDeclarationNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

FunctionDeclarationNode::FunctionDeclarationNode (ValueType t, string id,
						  vector<ParameterNode*> params, CompoundStatementNode* body)
  :DeclarationNode (t, id), parameters (params), functionBody(body)
{}


FunctionDeclarationNode::~FunctionDeclarationNode()
{
  for (auto param : parameters)
    delete param;
  parameters.clear ();
}

void
FunctionDeclarationNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

ArrayDeclarationNode::ArrayDeclarationNode (ValueType t, string id, size_t size)
  :DeclarationNode (t, id), size (size)
{}


ArrayDeclarationNode::~ArrayDeclarationNode()
{}

void
ArrayDeclarationNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/
ParameterNode::ParameterNode (ValueType t, string id, bool isArray)
  :DeclarationNode (t, id), isArray (isArray)
{}


ParameterNode::~ParameterNode()
{}

void
ParameterNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

StatementNode::~StatementNode ()
{}

/********************************************************************/
CoumpoundStatementNode::CompoundStatementNode (vector<VariableDeclarationNode*> declarations,vector<statementNode*> statements)
  :declarations(declarations), statements (statements)
{}

CoumpoundStatementNode::~CompoundStatementNode ()
{
  for (auto decls : declarations)
    delete decl;
  declarations.clear ();
  
  for (auto stmt : statements)
    delete stmt;
  statements.clear ();
}

void CoumpoundStatementNode ()
{
  visitor -> visit (this);
}

/********************************************************************/

IfStatementNode::IfStatementNode (ExpressionNode* expr, StatementNode* thenStmt,
				  StatementNode* elseStmt = nullptr)
  :conditionalExpression(expr), thenStatement (thenStmt), elseStatement (elseStmt)
{}

IfStatementNode::~IfStatementNode ()
{}

void
IfStatementNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

ForStatementNode::ForStatementNode (ExpressionNode* e1,
				    ExpressionNode* e2,
				    ExpressionNode* e3,
				    StatementNode* s)
  :initializer (e1), condition (e2), updater (e3), body (s)
{}

ForStatementNode::~ForStatementNode ()
{}

void
ForStatementNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

WhileStatementNode::WhileStatementNode (ExpressionNode* expr, StatementNode* stmt)
  : conditionalExpression (expr), body (stmt)
{}

WhileStatementNode::~WhileStatementNode ();

void
WhileStatementNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

ReturnStatementNode::ReturnStatementNode  (ExpressionNode* expr)
  : expressionNode (expr))
{}

ReturnStatementNod::~ReturnStatementNode ();

void
ReturnStatementNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/
ExpressionNode::~ExpressionNode ()
{}

/********************************************************************/

ExpressionStatementNode::ExpressionStatementNode (ExpressionNode* expr)
  :expression (expr)
{}

ExpressionStatementNode::~ExpressionStatementNode ()
{}

void
ExpressionStatementNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

AssignmentExpressionNode::AssignmentExpressionNode (VariableExpressionNode* var,
						    ExpressionNode* expr)
  :variable (var), expression (expr)
{}

AssignmentExpressionNode::~AssignmentExpressionNode ()
{}

void
AssignmentExpressionNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

VariableExpressionNode::VariableExpressionNode (string id)
  :identifier (id)
{}

VariableExpressionNode::~VariableExpressionNode ()
{}

void
VariableExpressionNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/
/*
  SubscriptExpressionNode::SubscriptExpressionNode (string id, ExpressionNode* index)
  :index (index)
  {}
  SubscriptExpressionNode::~SubscriptExpressionNode ()
  {}
  void
  SubscriptExpressionNode::accept (IVisitor* visitor)
  {
  visitor -> visit (this);
  }
*/
/********************************************************************/

CallExpressionNode::CallExpressionNode (string id, vector<ExpressionNode*> args)
  :identifier (id), arguments (args)
{}

CallExpressionNode::~CallExpressionNode ();

void
CallExpressionNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}
/********************************************************************/
AdditiveExpressionNode::AdditiveExpressionNode (AdditiveOperatorType addop,
						ExpressionNode* lhs,
						ExpressionNode* rhs)
  :addOperator (addop), left (lhs), right (rhs)
{}

AdditiveExpressionNode::~AdditiveExpressionNode ()
{}

void
AdditiveExpressionNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

RelationalExpressionNode::RelationalExpressionNode (RelationalOperatorType relop,
						    ExpressionNode* lhs,
						    ExpressionNode* rhs)
  :relationalOperator (relop), left (lhs), right (rhs)
{}

RelationalExpressionNode::~RelationalExpressionNode ()
{}

void
RelationalExpressionNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/
UnaryExpressionNode::UnaryExpressionNode (UnaryOperatorType unaryOp,
		       VariableExpressionNode* var)
  :unaryOperator (unaryOp), variable (var)
{}

UnaryExpressionNode::~UnaryExpressionNode ();

void
UnaryExpressionNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

IntegerLiteralExpressionNode::IntegerLiteralExpressionNode (int value)
  :value (value)
{}

IntegerLiteralExpressionNode::~IntegerLiteralExpressionNode ()
{}

void
IntegerLiteralExpressionNode::accept (IVisitor* visitor)
{
  visitor -> visit (this);
}

/********************************************************************/

void
printNodeInfo (string nodeType, auto node)
{
  cout << nodeType << ": " << node -> identifier << ": ";
  string type = (node -> valueType == ValueType::INT) ? "int" : "void";
  cout << type << " type" << endl;
}

/********************************************************************/


/********************************************************************/
// System Includes

#include <iostream>
#include "CMinusAst.h"

/********************************************************************/
// Using Declarations

using std::cout;
using std::endl;

/********************************************************************/

void
PrintVisitor::visit (ProgramNode* node)
{	
  level = 0;
  printLevel ();
  cout << "Program" << endl;
  ++level;
  for (auto child : node -> declarations)
    {
      child -> accept(this);
    }
  --level;
}

void
PrintVisitor::visit (VariableDeclarationNode* node)
{	
  printLevel ();
  printNodeInfo("Variable", node);
}

void
PrintVisitor::visit (FunctionDeclarationNode* node)
{	
  printLevel ();
  printNodeInfo("Function", node);

  ++level;
  for (auto child : node -> parameters)
    {
      child -> accept(this);
    }
  --level;
}


void
PrintVisitor::visit (ParameterNode* node)
{
  printLevel ();
  printNodeInfo("Parameter", node);

}

void
PrintVisitor::printLevel ()
{
  for (size_t i = 0; i < level; ++i)
    {
      cout << "|   ";
    }
	

}
/********************************************************************/

Node::~Node ()
{}

/********************************************************************/

ProgramNode::ProgramNode (vector<DeclarationNode*> declarations)
  :declarations(declarations)
{}


ProgramNode::~ProgramNode()
{
  for (auto decl : declarations)
    delete decl;
  declarations.clear ();
}

void
ProgramNode::accept (IVisitor* visitor)
{
  visitor -> visit(this);
}

/********************************************************************/


DeclarationNode::DeclarationNode (ValueType t, string id)
  :valueType(t), identifier(id), nestLevel(0)
{} 

DeclarationNode::~DeclarationNode ()
{}
/********************************************************************/


VariableDeclarationNode::VariableDeclarationNode (ValueType t, string id)
  :DeclarationNode(t, id)
{}


VariableDeclarationNode::~VariableDeclarationNode()
{}

void
VariableDeclarationNode::accept (IVisitor* visitor)
{
  visitor -> visit(this);
}

/********************************************************************/


FunctionDeclarationNode::FunctionDeclarationNode (ValueType t, string id, 
						  vector<ParameterNode*> params)
  //, CompoundStatementNode* body)
  :DeclarationNode(t, id), parameters(params)
   //, functionBody(body)	
{}


FunctionDeclarationNode::~FunctionDeclarationNode()
{
  for (auto param : parameters)
    delete param;
  parameters.clear ();
}

void
FunctionDeclarationNode::accept (IVisitor* visitor)
{
  visitor -> visit(this);
}

/********************************************************************/

ParameterNode::ParameterNode (ValueType t, string id, bool isArray)
  :DeclarationNode(t, id), isArray (isArray)
{}


ParameterNode::~ParameterNode()
{}

void
ParameterNode::accept (IVisitor* visitor)
{
  visitor -> visit(this);
}

/********************************************************************/
void
printNodeInfo (string nodeType, auto node)
{
  cout << nodeType << ": " << node -> identifier << ": ";
  string type = (node -> valueType == ValueType::INT) ? "int" : "void"; 
  cout << type << " type" << endl;
}
/********************************************************************/
