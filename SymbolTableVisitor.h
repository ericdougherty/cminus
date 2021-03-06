/*
  Filename   : SymbolTableVisitor.h
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/********************************************************************/

#ifndef SYMBOLTABLEVISITOR_H
#define SYMBOLTABLEVISITOR_H

#include "SymbolTable.h"

class SymbolTableVisitor : public IVisitor
{
public:
  	//Virtual Functions
	virtual void visit (ProgramNode* node);

	virtual void visit (DeclarationNode* node){};
	virtual void visit (FunctionDeclarationNode* node); 
	virtual void visit (VariableDeclarationNode* node); 
	virtual void visit (ArrayDeclarationNode* node); 
	virtual void visit (ParameterNode* node); 

	virtual void visit (StatementNode* node){}; 
	virtual void visit (CompoundStatementNode* node); 
	virtual void visit (IfStatementNode* node); 
	virtual void visit (WhileStatementNode* node);
	virtual void visit (ReturnStatementNode* node); 
	virtual void visit (ExpressionStatementNode* node); 

	virtual void visit (ExpressionNode* node){}; 
	virtual void visit (AssignmentExpressionNode* node); 
	virtual void visit (VariableExpressionNode* node); 
	virtual void visit (SubscriptExpressionNode* node); 
	virtual void visit (CallExpressionNode* node); 
	virtual void visit (AdditiveExpressionNode* node); 
	virtual void visit (MultiplicativeExpressionNode* node);
	virtual void visit (RelationalExpressionNode* node);
	virtual void visit (IntegerLiteralExpressionNode* node){};

  	// Non-Derived Functions
	int
	getLevel();

	void
	declError(auto node, string errorMessage);

	void
	useError(auto node,string errorMessage);

	bool
	getErrors() ;

	void
	insert (auto node);

	void
	lookup (auto node, string ifError);

	void
	setUpIOFunctions ();

	void
	startBold ();

	void 
	endBold ();

	void
	errorIsInGlobalScope ();

	void
	errorIsInAFunctionsScope ();

	void
	printLineAndColumnInfo (auto node);

	void
	printDeclErrorInfo (auto node, bool ifPrevious, string errorMessage);

  	//Non Derived Member Variables
	int      		level;
	string 			currentFunctionName;
	ValueType 		currentFunctionType;
	SymbolTable 	table;
	bool     		hasErrors;
	std::string 	fileName;
	int				varCount;
};

#endif