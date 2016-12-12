/*
  Filename   : CodeGenVisitor.cc
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/********************************************************************/
// System Includes

#include <iostream>

/********************************************************************/
// Local includes

#include "CodeGenVisitor.h"

/********************************************************************/
// Using Declarations

using std::cout;
using std::endl;

/********************************************************************/

void
CodeGenVisitor::visit (ProgramNode* node)
{
	
	AssemblyEmitter e (file);
	emitter = &e;
	emitter -> emitSeparator ();
	string comment ("File: " + filename);
	emitter -> emitComment ("C-Minus Compilation to IA-32 Code");
	emitter -> emitComment (comment);
	emitter -> emitSeparator ();

	for (auto child : node -> declarations)
	{
		child -> accept (this);
	}

	emitter -> emitInputFunction ();
	emitter -> emitOutputFunction ();
}

void
CodeGenVisitor::visit (VariableDeclarationNode* node)
{
	if (node -> nestLevel == 0)
	{
		emitter -> emitInstruction(".comm", node -> identifier + ", 4", "allocating global");
		emitter -> emitSeparator ();
	}
}

void
CodeGenVisitor::visit (FunctionDeclarationNode* node)
{
	emitter -> emitInstruction(".text", "", "Processing function: " + node -> identifier);
	emitter -> emitFunctionDeclaration(node -> identifier);
	emitter -> emitPrologue(node -> parameters.size ());

	for (auto child : node -> parameters)
	{
		child -> accept (this);
	}

	node -> functionBody -> accept (this);

	if (node -> parameters.size () > 0)
	{
		emitter -> emitInstruction ("subl", "$" + std::to_string(4 * node -> parameters.size ()) + ", %esp", "");
	}

	emitter -> emitEpilogue();
}

void
CodeGenVisitor::visit (ArrayDeclarationNode* node)
{
	
}

void
CodeGenVisitor::visit (ParameterNode* node)
{

}

void
CodeGenVisitor::visit (CompoundStatementNode* node)
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
CodeGenVisitor::visit (IfStatementNode* node)
{
	string elseLabel = emitter -> createUniqueLabel();
	string endLabel = emitter -> createUniqueLabel();

	node -> conditionalExpression -> accept (this);

	// switch(op)
	// {
	// 	case (RelationalOperatorType::GT):
		emitter -> emitInstruction("jle", elseLabel, "then");
	// 		break;
	// 	default:
	// 		break;
	// }

	node -> thenStatement -> accept (this);
	emitter -> emitInstruction("jmp", endLabel, "then");
	emitter -> emitLabel(elseLabel, "");
	node -> elseStatement -> accept (this);
	emitter -> emitLabel(endLabel, "");
}

void
CodeGenVisitor::visit (WhileStatementNode* node)
{
	node -> conditionalExpression -> accept (this);
	node -> body -> accept (this);
}

void
CodeGenVisitor::visit (ReturnStatementNode* node)
{
	node -> expression -> accept (this);
}

void
CodeGenVisitor::visit (ExpressionStatementNode* node)
{
	node -> expression -> accept (this);
}

void
CodeGenVisitor::visit (AssignmentExpressionNode* node)
{
	node -> expression -> accept (this);
	emitter -> emitInstruction ("movl", "$" + node -> variable -> identifier + ", %eax", "varExp -> eax");
	emitter -> emitInstruction ("popl", "%ebx", "");
	emitter -> emitInstruction ("movl", "%ebx, (%eax)", "assignment expression ended");
}

void
CodeGenVisitor::visit (VariableExpressionNode* node)
{
	emitter -> emitInstruction ("movl", "$" + node -> identifier +", %eax", "");
	emitter -> emitInstruction("pushl", "(%eax)", "var exp end");
}

void
CodeGenVisitor::visit (SubscriptExpressionNode* node)
{
	node -> index -> accept (this);
}

void
CodeGenVisitor::visit (CallExpressionNode* node)
{
	for (auto child : node -> arguments)
	{
		child -> accept (this);
	}
	emitter -> emitInstruction("call ", node -> identifier, "" );
	emitter -> emitInstruction("pushl", "%eax", "call expression ended");

}

void
CodeGenVisitor::visit (AdditiveExpressionNode* node)
{
	node -> right -> accept (this);
	node -> left -> accept (this);
	emitter -> emitInstruction("popl", "%eax", "");
	emitter -> emitInstruction("popl", "%edx", "");

	if (node -> addOperator == AdditiveOperatorType::PLUS)
	{
		emitter -> emitInstruction ("addl", "%edx, %eax", " add opp");
	}
	else
	{
		emitter -> emitInstruction ("subl", "%edx, %eax", " sub opp");
	}
	emitter -> emitInstruction("pushl", "%eax", "additive end");
}

void
CodeGenVisitor::visit (MultiplicativeExpressionNode* node)
{
	node -> right -> accept (this);
	node -> left -> accept (this);
	emitter -> emitInstruction("popl", "%eax", "");
	emitter -> emitInstruction("popl", "%ecx", "");

	if (node -> multOperator == MultiplicativeOperatorType::TIMES)
	{
		emitter -> emitInstruction ("imul", "%ecx, %eax", " mult opp");
	}
	else
	{
		emitter -> emitInstruction("movl", "$0, %edx", "");
		emitter -> emitInstruction ("idiv", "%ecx", " div opp");
	}
	emitter -> emitInstruction("pushl", "%eax", "additive end");
}

void
CodeGenVisitor::visit (RelationalExpressionNode* node)
{
	node -> right -> accept (this);
	node -> left -> accept (this);
	emitter -> emitInstruction("popl", "%eax", "");
	emitter -> emitInstruction("popl", "%ecx", "");

	RelationalOperatorType opType = node -> relationalOperator; 

	//	LT, LTE, GT, GTE, EQ, NEQ, ERROR
	if(opType != RelationalOperatorType::EQ && opType != RelationalOperatorType::NEQ)
	{
		emitter -> emitInstruction("cmpl", "%ecx, %eax", "reop cmp");
	}
	else
	{
		emitter -> emitInstruction("testl", "%ecx, %eax", "reop EQ or NEQ");
	}
}

void
CodeGenVisitor::visit (IntegerLiteralExpressionNode* node)
{
	emitter -> emitInstruction("movl", "$" + std::to_string(node -> value) + ", %eax", "Load constant");
	emitter -> emitInstruction("pushl", "%eax", "int literal end");
}

