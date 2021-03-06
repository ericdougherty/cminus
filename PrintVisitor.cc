/*
  Filename   : Visitor.cc
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/********************************************************************/
// System Includes

#include <iostream>

/********************************************************************/
// Local includes

#include "PrintVisitor.h"

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
	string type = (node -> valueType == ValueType::INT) ? "Int" : "Void";
	file << "Variable Declaration: " << node -> identifier ;
	file << "[" << node -> size << "]: " << type << " type" << endl;
	--level;
}

void
PrintVisitor::visit (ParameterNode* node)
{
	++level;
	printLevel ();
	string type = (node -> valueType == ValueType::INT) ? "Int" : "Void";
	string array = (node -> isArray == true) ? "[]: " : ": ";
	file << "Parameter: " << node -> identifier << array << type << " type" << endl;
	--level;
}

void
PrintVisitor::visit (CompoundStatementNode* node)
{
	++level;
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
	++level;
	printLevel ();
	file << "If" << endl;
	node -> conditionalExpression -> accept (this);
	node -> thenStatement -> accept (this);
	node -> elseStatement -> accept (this);
	--level;
}

void
PrintVisitor::visit (WhileStatementNode* node)
{
	++level;
	printLevel ();
	file << "While" << endl;
	node -> conditionalExpression -> accept (this);
	node -> body -> accept (this);
	--level;
}

void
PrintVisitor::visit (ReturnStatementNode* node)
{
	++level;
	printLevel ();
	file << "Return" << endl;
	node -> expression -> accept (this);
	--level;
}

void
PrintVisitor::visit (ExpressionStatementNode* node)
{
	++level;
	printLevel ();
	file << "Expression Statement: " << endl;
	node -> expression -> accept (this);
	--level;
}

void
PrintVisitor::visit (AssignmentExpressionNode* node)
{
	++level;
	printLevel ();
	file << "Assignment:" << endl;
	node -> variable -> accept (this);
	node -> expression -> accept (this);
	--level;
}

void
PrintVisitor::visit (VariableExpressionNode* node)
{
	++level;
	printLevel ();
	string type = (node -> decl -> valueType == ValueType::INT) ? "Int" : "Void";
	file << "Variable: " << node -> identifier << ": " << type << " type" << endl;
	--level;
}

void
PrintVisitor::visit (SubscriptExpressionNode* node)
{
	++level;
	printLevel ();
	string type = (node -> decl -> valueType == ValueType::INT) ? "Int" : "Void";
	file << "Subscript: " << node -> identifier << ": " << type << " type" << endl;
	++level;
	printLevel ();
	file << "Index:" << endl;
	node -> index -> accept (this);
	level -= 2;
}

void
PrintVisitor::visit (CallExpressionNode* node)
{
	++level;
	printLevel ();
	string type = (node -> decl -> valueType == ValueType::INT) ? "Int" : "Void";
	file << "Function Call: " << node -> identifier << ": " << type << " type" << endl;
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
	++level;
	printLevel ();
	string op = (node -> addOperator == AdditiveOperatorType::PLUS) ? "+" : "-";
	printArithmeticInfo ("Additive Expression", op, node);
}

void
PrintVisitor::visit (MultiplicativeExpressionNode* node)
{
	++level;
	printLevel ();
	string op = (node -> multOperator == MultiplicativeOperatorType::TIMES) ? "*" : "/";
	printArithmeticInfo ("Multiplicative Expression", op, node);
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
		case RelationalOperatorType::ERROR:
		break;
	}
	printArithmeticInfo ("Relational Expression", op, node);
}

void
PrintVisitor::visit (IntegerLiteralExpressionNode* node)
{
	++level;
	printLevel ();
	file << "Integer: "<< node -> value << endl;
	--level;
}

/********************************************************************/

void
PrintVisitor::printNodeInfo (string nodeType, auto node)
{
	file << nodeType << ": " << node -> identifier << ": ";
	string type = (node -> valueType == ValueType::INT) ? "Int" : "Void";
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
	for (size_t i = 0; i < level; ++i)
	{
		file << "|  ";
	}
}