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

Parser::Parser (FILE* file, bool debug, vector<DeclarationNode*> declarations)
    :m_lexer (Lexer(file)), m_matchedID(false), root (ProgramNode(declarations))
{ }

/***********************************************************************/

void
Parser::parse ()
{
	m_token = m_lexer.getToken ();
	declaration (&root);
	match (END_OF_FILE, "EOF", "program");
	cout << "Program is valid!" << endl;
	PrintVisitor visitor;
	root.accept (&visitor);
}

/***********************************************************************/

void 
Parser::declaration (auto parent)
{
	while (m_token.type == INT || m_token.type == VOID)
	{
		ValueType type = typeSpecifier ();
		string id = m_token.lexeme;

		match (ID, "ID", "declaration");
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
	VariableDeclarationNode* varDecl = new VariableDeclarationNode(type, id);
	parent -> declarations.push_back (varDecl);
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
	root.declarations.push_back (funDecl);
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
		statement (parent);
	}
}

void
Parser::statement (auto parent)
{
	switch (m_token.type)
	{
		case ID:
		case NUM:
		case LPAREN:
		case SEMI:
			expressionStmt (parent);
			break;
		case LBRACE:
			compoundStmt ();
			break;
		case IF:
			selectionStmt (parent);
			break;
		case WHILE:
			iterationStmt (parent);
			break;
		case RETURN:
			returnStmt (parent);
			break;
		default:
			error (";', '{',  'if', 'while' or 'return", "statement");
	}
}

void
Parser::expressionStmt (auto parent)
{
	if (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN)
	{
		expression ();
	}
	match (SEMI, ";", "expressionStmt");
}

void
Parser::selectionStmt (auto parent)
{

	match (IF, "if", "selectionStmt");
	match (LPAREN, "(", "selectionStmt");
	ExpressionNode* expr = expression ();
	match (RPAREN, ")", "selectionStmt");
	//StatementNode* thenStmt = statement (parent);
	statement (parent);
	//StatementNode* elseStmt = nullptr;
	if (m_token.type == ELSE)
	{
		match (ELSE, "else", "selectionStmt");
		//elseStmt = statement (parent);
	}
	//IfStatementNode* ifNode = new IfStatementNode(expr, thenStmt, elseStmt);
	//parent -> statements.push_back(ifNode);
}

void
Parser::iterationStmt (auto parent)
{
	match (WHILE, "while", "iterationStmt");
	match (LPAREN, "(", "iterationStmt");
	expression ();
	match (RPAREN, ")", "iterationStmt");
	statement (parent);
}

void
Parser::returnStmt (auto parent)
{
	match (RETURN, "return", "returnStmt");
	if (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN)
	{
		expression ();
	}

	match (SEMI, "expression or ;", "returnStmt");
}

ExpressionNode*
Parser::expression ()
{
	//if we see ID, we could have either production
	if (m_token.type == ID)
	{
		m_IDLexeme = match (ID, "ID", "expression");
		//PAREN means we have a simple-expression
		if (m_token.type == LPAREN)
		{
			//m_matchedID is a flag for simple-expression - we know we have a call, but already matched the ID
			m_matchedID = true;
			simpleExpression ();
		}
		else 
		{
			//we definitely have a var, but still could be either production
			var ();

			//if we see an assignment, we're in expression
			if (m_token.type == ASSIGN)
			{
				match (ASSIGN, "=", "expression");
				expression ();
			}
			//otherwise we're in simple-expression
			else
			{
				//set flag so we know we have already matched the ID
				m_matchedID = true;
				simpleExpression ();
			}
		}
	}
	//var must start with ID, so we go to simple-expression
	else {
		simpleExpression ();
	}
}

//var 	-->		ID ["[" expression "]"]
void
Parser::var ()
{
	//ID is always checked by function that calls var, so no 'match (ID)' here
	if (m_token.type == LBRACK)
	{
		match (LBRACK, "[", "var");
		expression ();
		match (RBRACK, "]", "var");
	}
}

//simple-expression		-->		additive-expression [relop additive-expression]
void
Parser::simpleExpression ()
{
	additiveExpression ();
	if (m_token.type == LT || m_token.type == LTE || m_token.type == GT || m_token.type == GTE || m_token.type == EQ || m_token.type == NEQ)
	{
		relop ();
		additiveExpression ();
	}
}

//relop		-->		"<=" | "<" | ">" | ">=" | "==" | "!="
void
Parser::relop ()
{
	switch (m_token.type)
	{
		case LT:
			match (LT, "<", "relop");
			break;
		case LTE:
			match (LTE, "<=", "relop");
			break;
		case GT:
			match (GT, ">", "relop");
			break;
		case GTE:
			match (GTE, ">=", "relop");
			break;
		case EQ:
			match (EQ, "==", "relop");
			break;
		case NEQ:
			match (NEQ, "!=", "relop");
			break;
		default: 
			break;
	}
}

//additive-expression 	-->		term {addop term}
void
Parser::additiveExpression ()
{
	term ();
	while (m_token.type == PLUS || m_token.type == MINUS)
	{
		addop ();
		term ();
	}
}


//addop		-->		"+" | "-"
void
Parser::addop ()
{
	if (m_token.type == PLUS)
	{
		match (PLUS, "+", "addop");
	}
	else
	{
		match (MINUS, "-", "addop");
	}
}

//term 		-->		factor {mulop factor}
void
Parser::term ()
{
	factor ();
	while (m_token.type == TIMES || m_token.type == DIVIDE)
	{
		mulop ();
		factor ();
	}
}

//mulop		-->		"*" | "/"
void
Parser::mulop ()
{
	if (m_token.type == TIMES)
	{
		match (TIMES, "*", "mulop");
	}
	else
	{
		match (DIVIDE, "/", "mulop");
	}
}

//factor	-->		"(" expression ")" | var | call | NUM
void
Parser::factor ()
{
	if (m_matchedID)
	{
		//reset flag
		m_matchedID = false;
		//if PAREN we have a call
		if (m_token.type == LPAREN)
		{
			call ();
		}
		//else we have just a var and already called var ()
	}
	else
	{
		switch (m_token.type)
		{
			case LPAREN:
				match (LPAREN, "(", "factor");
				expression ();
				match (RPAREN, ")", "factor");
				break;
			case ID:
				match (ID, "ID", "factor");
				if (m_token.type == LPAREN)
				{
					call ();
				}
				else
				{
					var ();
				}
				break;
			case NUM:
				match (NUM, "num", "factor");
				break;
			default:
				error ("( or ID or num", "factor");
		}
	}
}

CallExpressionNode*
Parser::call ()
{
	match (LPAREN, "(", "call");
	vector<ExpressionNode*>  argNodeList = args ();
	match (RPAREN, ")", "call");
	CallExpressionNode* callExpr = new CallExpressionNode(m_IDLexeme, argNodeList);
	return callExpr;
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