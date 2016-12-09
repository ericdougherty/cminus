/*
  Filename   : Ast.cc
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/********************************************************************/
// Local Includes

#include "Ast.h"

/********************************************************************/

Node::~Node ()
{}

/********************************************************************/

ProgramNode::ProgramNode ()
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

DeclarationNode::DeclarationNode (ValueType t, string id, size_t lineNum, size_t colNum)
:valueType (t), identifier (id), lineNum(lineNum), colNum(colNum)
{}

DeclarationNode::~DeclarationNode ()
{}

/********************************************************************/

VariableDeclarationNode::VariableDeclarationNode (ValueType t, string id, size_t lineNum, size_t colNum)
:DeclarationNode (t, id, lineNum, colNum)
{}


VariableDeclarationNode::~VariableDeclarationNode()
{}

void
VariableDeclarationNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

/********************************************************************/

FunctionDeclarationNode::FunctionDeclarationNode (ValueType t, string id,  size_t lineNum, size_t colNum,
	vector<ParameterNode*> params, CompoundStatementNode* body)
:DeclarationNode (t, id, lineNum, colNum), parameters (params), functionBody(body)
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

ArrayDeclarationNode::ArrayDeclarationNode (ValueType t, string id, size_t lineNum, size_t colNum, size_t size)
:VariableDeclarationNode (t, id, lineNum, colNum), size (size)
{}


ArrayDeclarationNode::~ArrayDeclarationNode()
{}

void
ArrayDeclarationNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

/********************************************************************/
ParameterNode::ParameterNode (ValueType t, string id, size_t lineNum, size_t colNum, bool isArray)
:DeclarationNode (t, id, lineNum, colNum ), isArray (isArray)
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

void
StatementNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

/********************************************************************/
CompoundStatementNode::CompoundStatementNode (vector<VariableDeclarationNode*> declarations,vector<StatementNode*> statements)
:declarations(declarations), statements (statements)
{}

CompoundStatementNode::~CompoundStatementNode ()
{
	for (auto decl : declarations)
		delete decl;
	declarations.clear ();
	
	for (auto stmt : statements)
		delete stmt;
	statements.clear ();
}

void
CompoundStatementNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

/********************************************************************/

IfStatementNode::IfStatementNode (ExpressionNode* expr, StatementNode* thenStmt,
	StatementNode* elseStmt)
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

WhileStatementNode::WhileStatementNode (ExpressionNode* expr, StatementNode* stmt)
: conditionalExpression (expr), body (stmt)
{}

WhileStatementNode::~WhileStatementNode ()
{}

void
WhileStatementNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

/********************************************************************/

ReturnStatementNode::ReturnStatementNode  (ExpressionNode* expr)
: expression (expr)
{}

ReturnStatementNode::~ReturnStatementNode ()
{}

void
ReturnStatementNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

/********************************************************************/
ExpressionNode::~ExpressionNode ()
{}

void
ExpressionNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

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

VariableExpressionNode::VariableExpressionNode (string id, size_t lineNum, size_t colNum)
:identifier (id), lineNum (lineNum), colNum (colNum)
{}

VariableExpressionNode::~VariableExpressionNode ()
{}

void
VariableExpressionNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

/********************************************************************/

SubscriptExpressionNode::SubscriptExpressionNode (string id, ExpressionNode* index, size_t lineNum, size_t colNum)
:VariableExpressionNode(id, lineNum, colNum), index (index)
{}

SubscriptExpressionNode::~SubscriptExpressionNode ()
{}

void
SubscriptExpressionNode::accept (IVisitor* visitor)
{
	visitor -> visit (this);
}

/********************************************************************/

CallExpressionNode::CallExpressionNode (string id, vector<ExpressionNode*> args, size_t lineNum, size_t colNum)
:identifier (id), arguments (args), lineNum (lineNum), colNum (colNum)
{}

CallExpressionNode::~CallExpressionNode ()
{
	for (auto arg : arguments)
		delete arg;
	arguments.clear ();
}

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

MultiplicativeExpressionNode::MultiplicativeExpressionNode (MultiplicativeOperatorType multop,
	ExpressionNode* lhs,
	ExpressionNode* rhs)
:multOperator (multop), left (lhs), right (rhs)
{}

MultiplicativeExpressionNode::~MultiplicativeExpressionNode ()
{}

void
MultiplicativeExpressionNode::accept (IVisitor* visitor)
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