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
    :m_lexer (Lexer(file)), m_debug(debug), m_matchedID(false), m_indent(-1),
    root (ProgramNode(declarations))
{ }

/***********************************************************************/

//program	-->		declaration-list
void
Parser::parse ()
{
	m_token = m_lexer.getToken ();
	declarationList ();
	match (END_OF_FILE, "EOF", "program");
	cout << "Program is valid!" << endl;
	PrintVisitor visitor;
	root.accept (&visitor);
}

/***********************************************************************/

//declaration-list 	 -->	declaration {declaration}
void
Parser::declarationList ()
{
	if (m_debug) enter("declarationList");
	while (m_token.type == INT || m_token.type == VOID)
	{
		declaration();
	}
	if (m_debug) leave("declarationList");
}

//declaration 	-->		type-specifier ID (variable-declaration | function-declaration)
void 
Parser::declaration ()
{
	if (m_debug) enter("declaration");
	ValueType valueType = (m_token.lexeme == "int") ? ValueType::INT : ValueType::VOID;
	typeSpecifier ();
	string id = m_token.lexeme;

	match (ID, "ID", "declaration");
	if (m_token.type == SEMI || m_token.type == LBRACK) 
	{
		variableDeclaration ();
		VariableDeclarationNode* varDecl = new VariableDeclarationNode(valueType, id);
		root.declarations.push_back (varDecl);
	}
	else if (m_token.type == LPAREN)
	{
		vector<ParameterNode*> paramVec;

		functionDeclaration (paramVec);
		FunctionDeclarationNode* funDecl = new FunctionDeclarationNode(valueType, id, paramVec);
		root.declarations.push_back (funDecl);
	}
	else
	{
		error (";', '[' or '(", "declaration");
	}
	if (m_debug) leave("declaration");
}

//variable-declaration 	-->		["[" NUM "]"] ";"
void
Parser::variableDeclaration ()
{
	if (m_debug) enter("variableDeclaration");
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
	if (m_debug) leave("variableDeclaration");
}

//type-specifier	-->		"int" | "void"
void
Parser::typeSpecifier ()
{
	if (m_debug) enter("typeSpecifier");
	if (m_token.type == INT )
	{
		match (INT, "int' or 'void", "typeSpecifier");
	}
	else
	{
		match (VOID, "int' or 'void", "typeSpecifier");
	}
	if (m_debug) leave("typeSpecifier");
}

//function-declaration 	-->		"(" params ")" compound-stmt
void
Parser::functionDeclaration (vector<ParameterNode*> & paramVec)
{
	if (m_debug) enter("functionDeclaration");
	match (LPAREN, "(", "functionDeclaration");
	params (paramVec);
	match (RPAREN, ")", "functionDeclaration");
	compoundStmt ();
	if (m_debug) leave("functionDeclaration");
}

//params	-->		param-list | "void"
void
Parser::params (vector<ParameterNode*> & paramVec)
{

	if (m_debug) enter("params");
	if (m_token.type == VOID)
	{
		match (VOID, "void", "params");
	}
	else
	{
		paramList (paramVec);
	}
	if (m_debug) leave("params");
}

//param-list	-->		param ["," param-list]
void
Parser::paramList (vector<ParameterNode*> & paramVec)
{
	if (m_debug) enter("paramList");
	param (paramVec);
	if (m_token.type == COMMA)
	{
		match (COMMA, ",", "paramList");
		paramList (paramVec);
	}
	if (m_debug) leave("paramList");
}

//param 	-->		type-specifier ID ["[" "]"]
void
Parser::param (vector<ParameterNode*> & paramVec)
{
	if (m_debug) enter("param");
	ValueType valueType = (m_token.lexeme == "int") ? ValueType::INT : ValueType::VOID;
	typeSpecifier ();
	string id = m_token.lexeme;
	bool isArray = false;

	match (ID, "ID", "param");
	if (m_token.type == LBRACK)
	{
		isArray = true;
		match (LBRACK, "[", "param");
		match (RBRACK, "]", "param");
	}

	ParameterNode* parameter = new ParameterNode(valueType, id, isArray);
	paramVec.push_back (parameter);
	if (m_debug) leave("param");
}

//compound-stmt		-->		"{" local-declarations statement-list "}"
void
Parser::compoundStmt ()
{
	if (m_debug) enter("compoundStmt");
	match (LBRACE, "{", "compoundStmt");
	localDeclarations ();
	statementList ();
	match (RBRACE, "}", "compoundStmt");
	if (m_debug) leave("compoundStmt");
}

//local-declarations	-->		{variable-declaration}
void
Parser::localDeclarations ()
{
	if (m_debug) enter("localDeclarations");
	while (m_token.type == INT || m_token.type == VOID)
	{
		typeSpecifier ();
		match (ID, "ID", "localDeclarations");
		variableDeclaration ();
	}
	if (m_debug) leave("localDeclarations");
}

//statement-list	-->		{statement}
void
Parser::statementList ()
{
	if (m_debug) enter("statementList");
	while (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN || m_token.type == SEMI || m_token.type == LBRACE || m_token.type == IF || m_token.type == WHILE || m_token.type == RETURN)
	{
		statement ();
	}
	if (m_debug) leave("statementList");
}

//statement -->	expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt
void
Parser::statement ()
{
	if (m_debug) enter("statement");
	switch (m_token.type)
	{
		case ID:
		case NUM:
		case LPAREN:
		case SEMI:
			expressionStmt ();
			break;
		case LBRACE:
			compoundStmt ();
			break;
		case IF:
			selectionStmt ();
			break;
		case WHILE:
			iterationStmt ();
			break;
		case RETURN:
			returnStmt ();
			break;
		default:
			error (";', '{',  'if', 'while' or 'return", "statement");
	}
	if (m_debug) leave("statement");
}

//expression-stmt	-->		[expression] ";"
void
Parser::expressionStmt ()
{
	if (m_debug) enter("expressionStmt");
	if (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN)
	{
		expression ();
	}
	match (SEMI, ";", "expressionStmt");
	if (m_debug) leave("expressionStmt");
}

//selection-stmt	-->		"if" "(" expression ")" statement ["else" statement]
void
Parser::selectionStmt ()
{
	if (m_debug) enter("selectionStmt");
	match (IF, "if", "selectionStmt");
	match (LPAREN, "(", "selectionStmt");
	expression ();
	match (RPAREN, ")", "selectionStmt");
	statement ();
	if (m_token.type == ELSE)
	{
		match (ELSE, "else", "selectionStmt");
		statement ();
	}
	if (m_debug) leave("selectionStmt");
}

//iteration-stmt	-->		"while" "(" expression ")" statement
void
Parser::iterationStmt ()
{
	if (m_debug) enter("iterationStmt");
	match (WHILE, "while", "iterationStmt");
	match (LPAREN, "(", "iterationStmt");
	expression ();
	match (RPAREN, ")", "iterationStmt");
	statement ();
	if (m_debug) leave("iterationStmt");
}

//return-stmt	-->		"return" [expression] ";"
void
Parser::returnStmt ()
{
	if (m_debug) enter("returnStmt");
	match (RETURN, "return", "returnStmt");
	if (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN)
	{
		expression ();
	}
	match (SEMI, "expression or ;", "returnStmt");
	if (m_debug) leave("returnStmt");
}

//expression 	-->		var "=" expression | simple-expression
//only place we need to diverge from one token look-ahead
void
Parser::expression ()
{
	if (m_debug) enter("expression");
	//if we see ID, we could have either production
	if (m_token.type == ID)
	{
		match (ID, "ID", "expression");
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
	if (m_debug) leave("expression");
}

//var 	-->		ID ["[" expression "]"]
void
Parser::var ()
{
	//ID is always checked by function that calls var, so no 'match (ID)' here
	if (m_debug) enter("var");
	if (m_token.type == LBRACK)
	{
		match (LBRACK, "[", "var");
		expression ();
		match (RBRACK, "]", "var");
	}
	if (m_debug) leave("var");
}

//simple-expression		-->		additive-expression [relop additive-expression]
void
Parser::simpleExpression ()
{
	if (m_debug) enter("simpleExpression");
	additiveExpression ();
	if (m_token.type == LT || m_token.type == LTE || m_token.type == GT || m_token.type == GTE || m_token.type == EQ || m_token.type == NEQ)
	{
		relop ();
		additiveExpression ();
	}
	if (m_debug) leave("simpleExpression");
}

//relop		-->		"<=" | "<" | ">" | ">=" | "==" | "!="
void
Parser::relop ()
{
	if (m_debug) enter("relop");
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
	if (m_debug) leave("relop");
}

//additive-expression 	-->		term {addop term}
void
Parser::additiveExpression ()
{
	if (m_debug) enter("additiveExpression");
	term ();
	while (m_token.type == PLUS || m_token.type == MINUS)
	{
		addop ();
		term ();
	}
	if (m_debug) leave("additiveExpression");
}


//addop		-->		"+" | "-"
void
Parser::addop ()
{
	if (m_debug) enter("addop");
	if (m_token.type == PLUS)
	{
		match (PLUS, "+", "addop");
	}
	else
	{
		match (MINUS, "-", "addop");
	}
	if (m_debug) leave("addop");
}

//term 		-->		factor {mulop factor}
void
Parser::term ()
{
	if (m_debug) enter("term");
	factor ();
	while (m_token.type == TIMES || m_token.type == DIVIDE)
	{
		mulop ();
		factor ();
	}
	if (m_debug) leave("term");
}

//mulop		-->		"*" | "/"
void
Parser::mulop ()
{
	if (m_debug) enter("mulop");
	if (m_token.type == TIMES)
	{
		match (TIMES, "*", "mulop");
	}
	else
	{
		match (DIVIDE, "/", "mulop");
	}
	if (m_debug) leave("mulop");
}

//factor	-->		"(" expression ")" | var | call | NUM
void
Parser::factor ()
{
	if (m_debug) enter("factor");
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
	if (m_debug) leave("factor");
}

//call		-->		ID "(" args ")"
void
Parser::call ()
{
	if (m_debug) enter("call");
	match (LPAREN, "(", "call");
	args ();
	match (RPAREN, ")", "call");
	if (m_debug) leave("call");
}

//args		-->		arg-list | empty
void
Parser::args ()
{
	if (m_debug) enter("args");
	if (m_token.type == ID || m_token.type == NUM || m_token.type == LPAREN)
	{
		argList ();
	}
	if (m_debug) leave("args");
}

//arg-list	   -->		expression {"," arg-list }
void 
Parser::argList ()
{
	if (m_debug) enter("argList");
	expression ();
	while (m_token.type == COMMA)
	{
		match (COMMA, "=", "expression");
		argList ();
	}
	if (m_debug) leave("argList");
}

/***********************************************************************/

void 
Parser::match (Token expectedToken, string possibleTokens, string caller)
{
	 if (m_token.type == expectedToken.type)
	{
		if (m_debug)
		{
			printSpaces(m_indent);
			cout << "|   " << "matching " << m_tokenList[m_token.type] << endl;	
		}
		
		m_token = m_lexer.getToken ();
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

/***********************************************************************/

//debugging functions to see call trace
void
Parser::enter(string functionName)
{
	printSpaces(++m_indent);
	cout << ">>>" <<functionName << endl;
}

void
Parser::leave(string functionName)
{
	printSpaces(m_indent--);
	cout << "<<<" <<functionName << endl;
}

void
Parser::printSpaces(int spaces)
{
	for (int j = 0; j < spaces; ++j)
	{
		cout << "|   ";
	}
}
