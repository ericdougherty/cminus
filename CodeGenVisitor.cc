/*
  Filename   : CodeGenVisitor.cc
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/********************************************************************/
// System Includes

#include <iostream>
#include <vector>

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
  	else
  	{
  		int offset = node -> framePointerOffset;
  		string offsetStr = std::to_string(offset);
  		emitter -> emitInstruction ("movl", "$0, " + offsetStr + "(%ebp)", " localVar load");
  	}
  }

  void
  CodeGenVisitor::visit (FunctionDeclarationNode* node)
  {
  	emitter -> emitSeparator ();
  	currentEndLabel = emitter -> createUniqueLabel ();
  	emitter -> emitInstruction(".text", "", "Processing function: " + node -> identifier);
  	emitter -> emitFunctionDeclaration (node -> identifier);
  	emitter -> emitPrologue (node -> localVarCount);

  	int parameterOffset = 4;
  	localVarOffset = 0;

  	for (auto child : node -> parameters)
  	{
  		child -> accept (this);
  		child -> framePointerOffset = parameterOffset += 4;
  	}

  	node -> functionBody -> accept (this);
  	emitter -> emitLabel (currentEndLabel, "");
  	emitter -> emitEpilogue();
  }

  void
  CodeGenVisitor::visit (ArrayDeclarationNode* node)
  {
  	if (node -> nestLevel == 0)
  	{
  		string arraySize = std::to_string(4 * node -> size);

  		emitter -> emitInstruction(".comm", node -> identifier + ", " + arraySize, "allocating global array");
  		emitter -> emitSeparator ();
  	}
  }

  void
  CodeGenVisitor::visit (ParameterNode* node)
  {
  	
  }

  void
  CodeGenVisitor::visit (CompoundStatementNode* node)
  {
  	emitter -> emitComment ("Compound Statement");
  	emitter -> emitSeparator ();
  	emitter -> emitComment ("Variable Declarations");
  	emitter -> emitSeparator ();
  	for (auto child : node -> declarations)
  	{
  		child -> framePointerOffset = localVarOffset -= 4;
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
  	emitter -> emitComment ("If Statement");
  	emitter -> emitSeparator ();
  	string elseLabel = emitter -> createUniqueLabel();
  	string endLabel = emitter -> createUniqueLabel();

  	node -> conditionalExpression -> accept (this);

  	emitter -> emitOperands(elseLabel, "");
  	node -> thenStatement -> accept (this);
  	emitter -> emitInstruction("jmp", endLabel, "then");
  	emitter -> emitLabel(elseLabel, "");
  	node -> elseStatement -> accept (this);
  	emitter -> emitLabel(endLabel, "");
  }

  void
  CodeGenVisitor::visit (WhileStatementNode* node)
  {
  	emitter -> emitComment ("While Statement");
  	emitter -> emitSeparator ();
  	string testLabel = emitter -> createUniqueLabel();
  	string endLabel = emitter -> createUniqueLabel();

  	emitter -> emitLabel(testLabel, "test");
  	node -> conditionalExpression -> accept (this);
  	emitter -> emitOperands(endLabel, "");
  	node -> body -> accept (this);
  	emitter -> emitInstruction("jmp", testLabel, "jump to test");

  	emitter -> emitLabel(endLabel, "end while");
  }

  void
  CodeGenVisitor::visit (ReturnStatementNode* node)
  {
  	node -> expression -> accept (this);
  	emitter -> emitInstruction("jmp", currentEndLabel, "go to end");
  }

  void
  CodeGenVisitor::visit (ExpressionStatementNode* node)
  {
  	node -> expression -> accept (this);
  }

  void
  CodeGenVisitor::visit (AssignmentExpressionNode* node)
  {
  	isArray = false;
  	node -> expression -> accept (this);
  	lhs = true;
  	node -> variable -> accept (this);
  	lhs = false;
  }

  void
  CodeGenVisitor::visit (VariableExpressionNode* node)
  {
  	int offset = node -> decl -> framePointerOffset;
  	string offsetStr = std::to_string(offset);
  	if (!lhs)
  	{
  		
  		if (offset > 4)
  		{
  			emitter -> emitInstruction ("movl", offsetStr + "(%ebp), %eax", " parameter load");
  			emitter -> emitInstruction("pushl", "%eax", "param var exp end");
  		}
  		else if (offset < 0)
  		{
  			emitter -> emitInstruction("pushl", offsetStr + "(%ebp)", "local var exp end");
  		}
  		else 
  		{
  			emitter -> emitInstruction ("movl", "$" + node -> identifier +", %eax", "global load");
  			emitter -> emitInstruction("pushl", "(%eax)", "global var exp end");
  		}
  	}
  	else
  	{
  		if (offset < 0)
  		{
  			emitter -> emitInstruction ("popl", offsetStr + "(%ebp)", "local var assignment");
  		}
  		else if (offset > 4)
  		{
  			emitter -> emitInstruction ("popl", offsetStr + "(%ebp)", "parameter assignment");
  		}
  		else
  		{
  			emitter -> emitInstruction ("movl", "$" + node -> identifier + ", %ecx", "Assignment");
  			emitter -> emitInstruction ("movl", "%eax, (%ecx)", "assignment expression ended");		
  		}
  	}
  }

  void
  CodeGenVisitor::visit (SubscriptExpressionNode* node)
  {
  	if (lhs)
  	{
  		lhs = false;
  		node -> index -> accept (this);
  		emitter -> emitInstruction ("popl", "%ecx", "");
  		emitter -> emitInstruction ("popl", "%eax", "");
  		emitter -> emitInstruction ("movl", "$" + node -> identifier + ", %edx", "Assignment");
  		emitter -> emitInstruction ("leal", "(%edx, %ecx, 4), %ecx", "");
  		emitter -> emitInstruction ("movl", "%eax, (%ecx)", "assignment expression ended");	
  		isArray = true;
  	}
  	else
  	{
  		node -> index -> accept (this);
  		emitter -> emitInstruction ("popl", "%ecx", "");
  		emitter -> emitInstruction ("movl", "$" + node -> identifier +", %edx", "");
  		emitter -> emitInstruction ("movl", "(%edx, %ecx, 4), %eax", "");
  		emitter -> emitInstruction ("pushl", "%eax", "sub var exp end");
  		isArray = false;
  	}

  }

  void
  CodeGenVisitor::visit (CallExpressionNode* node)
  {
  	std::vector<ExpressionStatementNode> buffer;
  	for (auto child : node -> arguments)
  	{
  		buffer.push_back(child);
  	}
  	for (auto i = buffer.rbegin(); i != buffer.rend(); ++i)
  	{
  		i -> accept (this);
  	}

  	emitter -> emitInstruction("call ", node -> identifier, "" );

  	for (auto child : node -> arguments)
  	{
  		if (child)
  		{
  			emitter -> emitInstruction ("popl", "%ecx", "");
  		}
  	}

  	emitter -> emitInstruction ("pushl", "%eax", "");
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

  	emitter -> emitInstruction("cmpl", "%ecx, %eax", "reop cmp");

  	switch(opType)
  	{
  		case (RelationalOperatorType::GT):
  		emitter -> emitOperation("jle");
  		break;
  		case (RelationalOperatorType::GTE):
  		emitter -> emitOperation("jl");
  		break;
  		case (RelationalOperatorType::LT):
  		emitter -> emitOperation("jge");
  		break;
  		case (RelationalOperatorType::LTE):
  		emitter -> emitOperation("jg");
  		break;
  		case (RelationalOperatorType::EQ):
  		emitter -> emitOperation("jne");
  		break;
  		case (RelationalOperatorType::NEQ):
  		emitter -> emitOperation("je");
  		break;
  		default:
  		break;
  	}
  }

  void
  CodeGenVisitor::visit (IntegerLiteralExpressionNode* node)
  {
  	emitter -> emitInstruction("movl", "$" + std::to_string(node -> value) + ", %eax", "Load constant");
  	emitter -> emitInstruction("pushl", "%eax", "int literal end");
  }

