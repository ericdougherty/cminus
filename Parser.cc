/*
  Filename   : Parser.cc
  Author     : Eric Dougherty
  Course     : CSCI 435
  Assignment : Lab 9 - Can't Parse This
*/

/***********************************************************************/
// System includes

#include <iostream>

/***********************************************************************/
// Local includes

#include "Parser.h"
#include "CMinusAst.h"

/***********************************************************************/
// Using declarations

using std::cout;
using std::endl;
using std::string;

/***********************************************************************/
// Function prototypes/global vars/typedefs

Parser::Parser (FILE* file, vector<DeclarationNode*> declarations)
    :m_lexer (Lexer(file)), m_matchedID(false), ast (ProgramNode(declarations))
{ }

/***********************************************************************/

void
Parser::parse ()
{
	m_token = m_lexer.getToken ();
	declaration (&ast);
	match (END_OF_FILE, "EOF", "program");
	cout << "Program is valid!" << endl;
	PrintVisitor visitor;
	ast.accept (&visitor);
}

/***********************************************************************/

void 
Parser::declaration (auto parent)
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
	if (m_token.type == SEMI)
	{
		match (SEMI, ";' or '[", "variableDeclaration");
	}
	else
	{
		match (LBRACK, "[' or ';", "variableDeclaration");
		match (NUM, "num", "variableDeclaration");
		match (RBRACK, "]", "variableDeclaration");
		match (SEMI, ";", "variableDeclaration");
	}
	VariableDeclarationNode* varDecl = new VariableDeclarationNode(type, id);
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
	FunctionDeclarationNode* funDecl = new FunctionDeclarationNode(type, id, paramVec, body);
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
		isArray = true;
		match (LBRACK, "[", "param");
		match (RBRACK, "]", "param");
	}

	ParameterNode* parameter = new ParameterNode(valueType, id, isArray);
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
	//if we see ID, we could have either production
	if (m_token.type == ID)
	{
		string id = m_lexemeID = match (ID, "ID", "expression");
		//PAREN means we have a simple-expression
		if (m_token.type == LPAREN)
		{
			//m_matchedID is a flag for simple-expression - we know we have a call, but already matched the ID
			m_matchedID = true;
			return simpleExpression ();
		}
		else 
		{
			//we definitely have a var, but still could be either production
			VariableExpressionNode* varNode = m_varNode = var (id);

			//if we see an assignment, we're in expression
			if (m_token.type == ASSIGN)
			{
				match (ASSIGN, "=", "expression");
				ExpressionNode* exprNode = expression ();
				return new AssignmentExpressionNode (varNode, exprNode);
			}
			//otherwise we're in simple-expression
			else
			{
				//set flag so we know we have already matched the ID
				m_matchedID = true;
				return simpleExpression ();
			}
		}
	}
	//var must start with ID, so we go to simple-expression
	else {
		return simpleExpression ();
	}
}

VariableExpressionNode*
Parser::var (string id)
{
	//ID is always checked by function that calls var, so no 'match (ID)' here
	VariableExpressionNode* varNode = new VariableExpressionNode(id);
	if (m_token.type == LBRACK)
	{
		match (LBRACK, "[", "var");
		expression ();
		match (RBRACK, "]", "var");
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
			break;
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
			return call (m_lexemeID);
		}
		//else we have just a var and already called var ()
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
	}
}

CallExpressionNode*
Parser::call (string id)
{
	match (LPAREN, "(", "call");
	vector<ExpressionNode*>  argNodeList = args ();
	match (RPAREN, ")", "call");
	return new CallExpressionNode(id, argNodeList);
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

/***********************************************************************/

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
	}
}

void
Parser::error (string possibleTokens, string caller)
{
	cout << "Error while parsing '" << caller << "' on line " << m_lexer.getLines() << ", column " << m_lexer.getColumns() << endl << " Encountered '" << m_token.lexeme << "' but expected '" << possibleTokens << "'"<< endl;
	exit(0);
}