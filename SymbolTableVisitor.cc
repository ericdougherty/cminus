/*
  Filename   : SymbolTableVisitor.cc
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
	setUpIOFunctions ();

	for (auto child : node -> declarations)
	{
		child -> accept (this);
	}
	table.exitScope ();
}

/********************************************************************/

void
SymbolTableVisitor::visit (VariableDeclarationNode* node)
{
	insert (node);
	++varCount;
}

/********************************************************************/

void
SymbolTableVisitor::visit (FunctionDeclarationNode* node)
{
	insert (node);

	varCount = 0;
	table.enterScope ();
	currentFunctionName = node -> identifier;
	currentFunctionType = node -> valueType;

	for (auto child : node -> parameters)
	{
		child -> accept (this);
	}
	node -> functionBody -> accept (this);
	node -> localVarCount = varCount;
	table.exitScope ();
}

/********************************************************************/

void
SymbolTableVisitor::visit (ArrayDeclarationNode* node)
{
	insert (node);
	++varCount;
}

/********************************************************************/

void
SymbolTableVisitor::visit (ParameterNode* node)
{
	insert (node);
}

/********************************************************************/

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

/********************************************************************/

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

/********************************************************************/

void
SymbolTableVisitor::visit (WhileStatementNode* node)
{
	table.enterScope ();
	node -> conditionalExpression -> accept (this);
	node -> body -> accept (this);
	table.exitScope ();
}

/********************************************************************/

void
SymbolTableVisitor::visit (ReturnStatementNode* node)
{
	node -> expression -> accept (this);
}

/********************************************************************/

void
SymbolTableVisitor::visit (ExpressionStatementNode* node)
{
	node -> expression -> accept (this);
}

/********************************************************************/

void
SymbolTableVisitor::visit (AssignmentExpressionNode* node)
{
	node -> variable -> accept (this);
	node -> expression -> accept (this);
}

/********************************************************************/

void
SymbolTableVisitor::visit (VariableExpressionNode* node)
{
	lookup (node, "variable");
}

/********************************************************************/

void
SymbolTableVisitor::visit (SubscriptExpressionNode* node)
{
	lookup (node, "array");
	node -> index -> accept (this);
}

/********************************************************************/

void
SymbolTableVisitor::visit (CallExpressionNode* node)
{

	lookup (node, "function");
	for (auto child : node -> arguments)
	{
		child -> accept (this);
	}
}

/********************************************************************/

void
SymbolTableVisitor::visit (AdditiveExpressionNode* node)
{
	node -> left -> accept (this);
	node -> right -> accept (this);
}

/********************************************************************/

void
SymbolTableVisitor::visit (MultiplicativeExpressionNode* node)
{
	node -> left -> accept (this);
	node -> right -> accept (this);
}

/********************************************************************/

void
SymbolTableVisitor::visit (RelationalExpressionNode* node)
{
	node -> left -> accept (this);
	node -> right -> accept (this);
}

/********************************************************************/

void
SymbolTableVisitor::declError(auto node,string errorMessage)
{
	if(node -> nestLevel == 0)
	{
		errorIsInGlobalScope ();
	}
	else
	{
		errorIsInAFunctionsScope ();
	}

	printDeclErrorInfo(node, false, errorMessage);
	auto prevNode = table.getPreviousDeclaration (node -> identifier, node -> nestLevel);
	printDeclErrorInfo (prevNode, true, " previously declared here\n");

	hasErrors = true;
}

/********************************************************************/

void
SymbolTableVisitor::useError(auto node, string errorMessage)
{
	printLineAndColumnInfo (node);
	
  // print error in red
	cout <<  "\e[1;31merror: \e[0m";
	cout <<  errorMessage;
	
	startBold (); 
	cout << " '" << node -> identifier << "'";
	endBold ();

	cout << " was not declared in this scope\n" << endl;
	
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
	node -> nestLevel = table.getLevel ();

	if (!table.insert (node))
	{
		declError(node, "Redeclaration of variable ");
	}
}

/********************************************************************/

void
SymbolTableVisitor::lookup (auto node, string ifError)
{
	DeclarationNode* retNode = table.lookup(node -> identifier);
	if (retNode == nullptr)
	{
		useError(node, ifError);
	}
	else
	{
		node -> decl = retNode;
	}
}

/********************************************************************/

void
SymbolTableVisitor::setUpIOFunctions ()
{
	auto input  = new FunctionDeclarationNode(ValueType::VOID, "input",0,0);
	auto output = new FunctionDeclarationNode(ValueType::VOID, "output",0,0);
	insert(input);
	insert(output);
}
/********************************************************************/

void
SymbolTableVisitor::startBold ()
{
	cout << "\e[1m";
}

/********************************************************************/

void 
SymbolTableVisitor::endBold ()
{
	cout << "\e[0m"; 
}

/********************************************************************/

void
SymbolTableVisitor::errorIsInGlobalScope ()
{
	startBold ();
	cout << fileName << ":";
	endBold ();
	cout << " In Global Scope" << endl;
}

/********************************************************************/

void
SymbolTableVisitor::printLineAndColumnInfo (auto node)
{
	startBold ();
	cout << fileName << ":" << node-> lineNum  << ":" << node-> colNum << ": ";
	endBold ();
}

/********************************************************************/

void
SymbolTableVisitor::errorIsInAFunctionsScope ()
{
	string functionType = (currentFunctionType == ValueType::INT) ? "int" : "void";
	
	startBold ();
	cout << fileName << ":" ;
	endBold ();
	
	cout << " In Function "; 

	startBold();
	cout << "'" << functionType << " " << currentFunctionName << " ()" "'";
	endBold ();
	cout << endl;
}

/********************************************************************/

void
SymbolTableVisitor::printDeclErrorInfo (auto node, bool ifPrevious, string errorMessage)
{
	string type = (node -> valueType == ValueType::INT) ? "int" : "void";

	if (!ifPrevious)
	{  
		printLineAndColumnInfo (node);
		
    // print error in red text
		cout <<  "\e[1;31merror: \e[0m";
		cout <<  errorMessage;
		
		startBold ();
		cout << "'" << type << " " << node -> identifier << "'";
		endBold ();
		cout << endl;
	}
	else
	{
		printLineAndColumnInfo (node);

  //print note in cyan
		cout <<  "\e[1;36mnote: \e[0m";
		
		startBold ();
		cout << "'" << type << " " << node -> identifier << "'";
		endBold ();

		cout << errorMessage << endl;
	}
}