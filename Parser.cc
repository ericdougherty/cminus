/*
  Filename   : Parser.cc
  Author     : Eric Dougherty
  Course     : CSCI 435
*/

/********************************************************************/
// System includes

#include <iostream>
#include <fstream>

/********************************************************************/
// Local includes

#include "Parser.h"
#include "PrintVisitor.h"
#include "SymbolTableVisitor.h"
#include "CodeGenVisitor.h"
#include "AssemblyEmitter.h"

/********************************************************************/
// Using declarations

using std::string;
using std::cout;
using std::endl;
using std::ofstream;

/********************************************************************/
// Function prototypes/global vars/typedefs

Parser::Parser (FILE* file)
    :m_lexer (Lexer(file)), m_matchedID(false), ast (ProgramNode())
{ }

/********************************************************************/

bool
Parser::parse ()
{
	m_token = m_lexer.getToken ();
	declarations (&ast);
	match (END_OF_FILE, "EOF", "program");
	return true;
}

/********************************************************************/

void
Parser::print (string name)
{
	PrintVisitor visitor;
	visitor.file = ofstream {name + "ast"};
	ast.accept (&visitor);
	visitor.file.close ();
}

/********************************************************************/

bool
Parser::symbolTable ()
{
	SymbolTableVisitor visitor;
	ast.accept (&visitor);
	return !visitor.getErrors();
}

/********************************************************************/

void
Parser::codeGen (string name)
{
	CodeGenVisitor visitor;
	visitor.file = ofstream {name + "s"};
	visitor.filename = name + "s";
	ast.accept (&visitor);
	visitor.file.close ();
	
}

/********************************************************************/

void 
Parser::declarations (auto parent)
{
	while (m_token.type == INT || m_token.type == VOID)
	{
		ValueType type = typeSpecifier ();
		string id = match (ID, "ID", "declaration");
		if (m_token.type == SEMI || m_token.type == LBRACK) 
		{
			variableDeclaration (type, id, parent);
		}
		else if (m_token.type == LPAREN)
		{

			functionDeclaration (type, id);
		}
		else
		{
			error (";', '[' or '(", "declaration");
		}
	}
}

void
Parser::variableDeclaration (ValueType type, string id, auto parent)
{
	VariableDeclarationNode* varDecl = nullptr;
	if (m_token.type == SEMI)
	{
		match (SEMI, ";' or '[", "variableDeclaration");
		varDecl = new VariableDeclarationNode(type, id, m_lexer.getLines (), m_lexer.getColumns ());
	}
	else
	{
		match (LBRACK, "[' or ';", "variableDeclaration");
		size_t size = m_token.number;
		match (NUM, "num", "variableDeclaration");
		match (RBRACK, "]", "variableDeclaration");
		match (SEMI, ";", "variableDeclaration");
		varDecl = new ArrayDeclarationNode(type, id, m_lexer.getLines (), m_lexer.getColumns (), size);
	}
	parent -> declarations.push_back (varDecl);
}

ValueType
Parser::typeSpecifier ()
{
	if (m_token.type == INT )
	{
		match (INT, "int' or 'void", "typeSpecifier");
		return ValueType::INT;
	}
	else
	{
		match (VOID, "int' or 'void", "typeSpecifier");
		return ValueType::VOID;
	}
}

void
Parser::functionDeclaration (ValueType type, string id)
{
	vector<ParameterNode*> paramVec;
	match (LPAREN, "(", "functionDeclaration");
	params (paramVec);
	match (RPAREN, ")", "functionDeclaration");
	CompoundStatementNode* body = compoundStmt ();
	FunctionDeclarationNode* funDecl = new FunctionDeclarationNode(type, id, m_lexer.getLines (), m_lexer.getColumns (), paramVec, body);
	ast.declarations.push_back (funDecl);
}

void
Parser::params (vector<ParameterNode*> & paramVec)
{
	if (m_token.type == VOID)
	{
		match (VOID, "void", "params");
	}
	else
	{
		paramList (paramVec);
	}
}

void
Parser::paramList (vector<ParameterNode*> & paramVec)
{
	param (paramVec);
	while (m_token.type == COMMA)
	{
		match (COMMA, ",", "paramList");
		param (paramVec);
	}
}

void
Parser::param (vector<ParameterNode*> & paramVec)
{
	ValueType valueType = typeSpecifier ();
	bool isArray = false;
	string id = match (ID, "ID", "param");
	if (m_token.type == LBRACK)
	{
		match (LBRACK, "[", "param");
		match (RBRACK, "]", "param");
		isArray = true;
	}
	ParameterNode* parameter = new ParameterNode(valueType, id, m_lexer.getLines (), m_lexer.getColumns (), isArray);
	paramVec.push_back (parameter);
}

CompoundStatementNode*
Parser::compoundStmt ()
{	vector<VariableDeclarationNode*> declarations;
	vector<StatementNode*> statements;
	CompoundStatementNode* compStmtNode = new CompoundStatementNode(declarations, statements);
	match (LBRACE, "{", "compoundStmt");
	localDeclarations (compStmtNode);
	statementList (compStmtNode);
	match (RBRACE, "}", "compoundStmt");
	return compStmtNode;
}

void
Parser::localDeclarations (auto parent)
{
	while (m_token.type == INT || m_token.type == VOID)
	{
		ValueType type = typeSpecifier ();
		string id = match (ID, "ID", "localDeclarations");
		variableDeclaration (type, id, parent);
	}
}

void
Parser::statementList (auto parent)
{
	while (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN || m_token.type == SEMI || m_token.type == LBRACE || m_token.type == IF || m_token.type == WHILE || m_token.type == RETURN)
	{
		parent -> statements.push_back (statement ());
	}
}

StatementNode*
Parser::statement ()
{
	switch (m_token.type)
	{
		case ID:
		case NUM:
		case LPAREN:
		case SEMI:
			return expressionStmt ();
		case LBRACE:
			return compoundStmt ();
		case IF:
			return selectionStmt ();
		case WHILE:
			return iterationStmt ();
		case RETURN:
			return returnStmt ();
		default:
			error (";', '{',  'if', 'while' or 'return", "statement");
	}
	//for warning
	return nullptr;
}

ExpressionStatementNode*
Parser::expressionStmt ()
{
	ExpressionNode* expr = nullptr;
	if (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN)
	{
		expr = expression ();
	}
	match (SEMI, ";", "expressionStmt");
	return new ExpressionStatementNode(expr);
}

IfStatementNode*
Parser::selectionStmt ()
{

	match (IF, "if", "selectionStmt");
	match (LPAREN, "(", "selectionStmt");
	ExpressionNode* expr = expression ();
	match (RPAREN, ")", "selectionStmt");
	StatementNode* thenStmt = statement ();
	StatementNode* elseStmt = nullptr;
	if (m_token.type == ELSE)
	{
		match (ELSE, "else", "selectionStmt");
		elseStmt = statement ();
	}
	return new IfStatementNode(expr, thenStmt, elseStmt);
}

WhileStatementNode*
Parser::iterationStmt ()
{
	match (WHILE, "while", "iterationStmt");
	match (LPAREN, "(", "iterationStmt");
	ExpressionNode* expr = expression ();
	match (RPAREN, ")", "iterationStmt");
	StatementNode* stmt = statement ();
	return new WhileStatementNode(expr, stmt);
}

ReturnStatementNode*
Parser::returnStmt ()
{
	match (RETURN, "return", "returnStmt");
	ReturnStatementNode* retNode = nullptr;
	if (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN)
	{
		ExpressionNode* exprNode = expression ();
		retNode = new ReturnStatementNode(exprNode);
	}
	else
	{
		retNode = new ReturnStatementNode();
	}
	match (SEMI, "expression or ;", "returnStmt");
	return retNode;
}

ExpressionNode*
Parser::expression ()
{
	if (m_token.type == ID)
	{
		m_callID = match (ID, "ID", "expression");
		if (m_token.type == LPAREN)
		{
			m_matchedID = true;
			return simpleExpression ();
		}
		else 
		{
			VariableExpressionNode* varNode = m_varNode = var (m_callID);

			if (m_token.type == ASSIGN)
			{
				match (ASSIGN, "=", "expression");
				ExpressionNode* exprNode = expression ();
				return new AssignmentExpressionNode (varNode, exprNode);
			}
			else
			{
				m_matchedID = true;
				return simpleExpression ();
			}
		}
	}
	else {
		return simpleExpression ();
	}
}

VariableExpressionNode*
Parser::var (string id)
{
	VariableExpressionNode* varNode = nullptr;
	if (m_token.type == LBRACK)
	{
		match (LBRACK, "[", "var");
		ExpressionNode* expr = expression ();
		match (RBRACK, "]", "var");
		varNode = new SubscriptExpressionNode (id, expr, m_lexer.getLines (), m_lexer.getColumns ());
	}
	else
	{
		varNode = new VariableExpressionNode (id, m_lexer.getLines (), m_lexer.getColumns ());
	}
	return varNode;
}

ExpressionNode*
Parser::simpleExpression ()
{
	ExpressionNode* lhs = additiveExpression ();
	if (m_token.type == LT || m_token.type == LTE || m_token.type == GT || m_token.type == GTE || m_token.type == EQ || m_token.type == NEQ)
	{
		RelationalOperatorType op = relop ();
		ExpressionNode* rhs = additiveExpression ();
		RelationalExpressionNode* relNode = new RelationalExpressionNode(op, lhs, rhs);
		return relNode;
	}
	return lhs;
}

RelationalOperatorType
Parser::relop ()
{
	switch (m_token.type)
	{
		case LT:
			match (LT, "<", "relop");
			return RelationalOperatorType::LT;
		case LTE:
			match (LTE, "<=", "relop");
			return RelationalOperatorType::LTE;
		case GT:
			match (GT, ">", "relop");
			return RelationalOperatorType::GT;
		case GTE:
			match (GTE, ">=", "relop");
			return RelationalOperatorType::GTE;
		case EQ:
			match (EQ, "==", "relop");
			return RelationalOperatorType::EQ;
		case NEQ:
			match (NEQ, "!=", "relop");
			return RelationalOperatorType::NEQ;
		default: 
			//for warning
			return RelationalOperatorType::NEQ;
	}

}

ExpressionNode*
Parser::additiveExpression ()
{
	ExpressionNode* lhs = term ();
	while (m_token.type == PLUS || m_token.type == MINUS)
	{
		AdditiveOperatorType op = addop ();
		ExpressionNode* rhs = term ();
		AdditiveExpressionNode* addNode = new AdditiveExpressionNode(op, lhs, rhs);
		return addNode;
	}
	return lhs;
	
}

AdditiveOperatorType
Parser::addop ()
{
	if (m_token.type == PLUS)
	{
		match (PLUS, "+", "addop");
		return AdditiveOperatorType::PLUS;
	}
	else
	{
		match (MINUS, "-", "addop");
		return AdditiveOperatorType::MINUS;
	}
}

ExpressionNode*
Parser::term ()
{
	ExpressionNode* lhs = factor ();
	while (m_token.type == TIMES || m_token.type == DIVIDE)
	{
		MultiplicativeOperatorType op = mulop ();
		ExpressionNode* rhs = factor ();
		MultiplicativeExpressionNode* multNode = new MultiplicativeExpressionNode(op, lhs, rhs);
		return multNode;
	}
	return lhs;
}

MultiplicativeOperatorType
Parser::mulop ()
{
	if (m_token.type == TIMES)
	{
		match (TIMES, "*", "mulop");
		return MultiplicativeOperatorType::TIMES;
	}
	else
	{
		match (DIVIDE, "/", "mulop");
		return MultiplicativeOperatorType::DIVIDE;
	}
}

ExpressionNode*
Parser::factor ()
{
	if (m_matchedID)
	{
		m_matchedID = false;
		if (m_token.type == LPAREN)
		{
			return call (m_callID);
		}
		return m_varNode;
	}
	else
	{
		if (m_token.type == LPAREN)
		{
			match (LPAREN, "(", "factor");
			ExpressionNode* expr =  expression ();
			match (RPAREN, ")", "factor");
			return expr;
		}
		else if (m_token.type == ID)
		{
			string id = match (ID, "ID", "factor");
			if (m_token.type == LPAREN)
			{
				return call (id);
			}
			else
			{
				return var (id);
			}
		}
		else if (m_token.type == NUM)
		{
			int num = m_token.number;
			match (NUM, "num", "factor");
			return new IntegerLiteralExpressionNode(num);
		}
		//for warning
		return nullptr;
	}
}

CallExpressionNode*
Parser::call (string id)
{
	match (LPAREN, "(", "call");
	vector<ExpressionNode*>  argNodeList = args ();
	match (RPAREN, ")", "call");
	return new CallExpressionNode(id, argNodeList, m_lexer.getLines (), m_lexer.getColumns ());
}

vector<ExpressionNode*> 
Parser::args ()
{
	vector<ExpressionNode*> arguments;
	if (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN)
	{
		argList (arguments);
	}
	return arguments;
}

void 
Parser::argList (vector<ExpressionNode*> & args)
{
	args.push_back (expression ());
	while (m_token.type == COMMA)
	{
		match (COMMA, "=", "expression");
		args.push_back (expression ());
	}
}

/********************************************************************/

string 
Parser::match (Token expectedToken, string possibleTokens, string caller)
{
	if (m_token.type == expectedToken.type)
	{
		string lexeme = m_token.lexeme;
		m_token = m_lexer.getToken ();
		return lexeme;
	}
	else
	{
		error (possibleTokens, caller);
		return "";
	}
}

void
Parser::error (string possibleTokens, string caller)
{
	cout << "Error while parsing '" << caller << "' on line " << m_lexer.getLines() << ", column " << m_lexer.getColumns() << endl << " Encountered '" << m_token.lexeme << "' but expected '" << possibleTokens << "'"<< endl;
	exit(0);
}