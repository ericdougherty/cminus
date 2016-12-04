/*
  Filename   : CMinusDriver.cc
  Author     : Ian T. Murry
  Course     : CSCI 435-01
  Note       : Some modifications were made to the original. Code was adopted 
               from Eric's parser, to make our code closer in structure. 
               Notably Expression, Statement, and Factor.
*/

/*********************************************************************/
// System inculdes

#include <iostream>

/*********************************************************************/
// Local includes

#include "CMinusParser.h"
#include "PrintVisitor.h"
#include "SymbolTableVisitor.h"

/*********************************************************************/
// Using declarations
               
using std::vector;
using std::cout;
using std::endl;
using std::string;

/*********************************************************************/

Parser::Parser (const vector<Token> & tokenBuffer)
  : m_token (tokenBuffer[0]), m_tokenBuffer (tokenBuffer),
    m_bufferPos (0),m_matchedID(false),
    astRoot (ProgramNode ())

{}

/*********************************************************************/

bool
Parser::parse ()
{
  if (m_token.type != END_OF_FILE)
  {
    declarations (&astRoot);
  }
  else
  {
    cout << "Empty File." << endl;
    exit (0);
  }

  if (m_token.type == END_OF_FILE)
  {
    return true;
  }
  else
  {
    printError ("Parse", "END_OF_FILE");
    return false;
  }

}

/*********************************************************************/

bool
Parser::symbolTable (string name)
{
  SymbolTableVisitor visitor;
  visitor.fileName = name;
  astRoot.accept (&visitor);
  return !visitor.getErrors();
}
/***********************************************************************/

void
Parser::print (string name)
{
  PrintVisitor visitor;
  visitor.file = ofstream {name + "ast"};
  astRoot.accept (&visitor);
  visitor.file.close ();
}

/*********************************************************************/

void
Parser::getToken ()
{
  if (m_token.type != END_OF_FILE)
  {
    ++m_bufferPos;
    m_token = m_tokenBuffer[m_bufferPos];
  }
}

/*********************************************************************/

void
Parser::declarations (auto parent)
{
  string state = "Variable or Function Declaration";

  while (startDeclaration ())
  {
    
    ValueType type = typeSpec ();
    string id = m_token.lexeme;
    match (state, ID, m_token.lexeme);
    
    if (m_token.type == SEMI || m_token.type == LBRACK)
    {
      varDeclaration (type, id, parent);	
    } 
    else if (m_token.type == LPAREN)
    {
      funcDeclaration (type, id);
    }
    
  }
}

/*********************************************************************/

void
Parser::varDeclaration (ValueType type, string id, auto parent)
{
  string state = "VariableDeclaration";
  VariableDeclarationNode* varDecl = nullptr;
  
  if (m_token.type == SEMI)
  {
    match (state, SEMI, ";");
    varDecl = new VariableDeclarationNode(type, id, m_token.lineNum, m_token.colNum);
  }
  else
  {
    match (state, LBRACK, "[");
    size_t size = m_token.number;
    match (state, NUM, m_token.lexeme);
    match (state, RBRACK, "]");
    match (state, SEMI, ";");
    varDecl = new ArrayDeclarationNode(type, id, m_token.lineNum, m_token.colNum, size);
  }

  parent -> declarations.push_back (varDecl);
}

/*********************************************************************/

ValueType
Parser::typeSpec ()
{
  string state = "TypeSpec";
  if (m_token.type == INT)
  {
    match (state, INT, "int");
    return ValueType::INT;
  }
  else
  {
    match (state, VOID, "void");
    return ValueType::VOID;
  }
}

/*********************************************************************/

void
Parser::funcDeclaration (ValueType type, string id)
{
  string state = "FuncDeclaration";
  vector<ParameterNode*> paramVec;

  match (state, LPAREN, "(");
  params (paramVec);
  match (state, RPAREN, ")");

  auto body = compoundStatement ();
  auto funDecl = new FunctionDeclarationNode(type, id, m_token.lineNum, m_token.colNum, paramVec, body);
  astRoot.declarations.push_back (funDecl);
}

/*********************************************************************/

void
Parser::params (vector<ParameterNode*> & paramVec)
{
  string state = "Params";
  if (m_token.type == VOID)
  {
    match (state, VOID, "void");
  }

  else
  {
    paramList (paramVec);
  }
}

/*********************************************************************/

void
Parser::paramList (vector<ParameterNode*> & paramVec)
{
  string state = "ParamList";
  param (paramVec);
  
  while (m_token.type == COMMA)
  {
    match (state, COMMA, ",");
    param (paramVec);
  }
}

/*********************************************************************/

void
Parser::param (vector<ParameterNode*> & paramVec)
{
  string state = "Param";
  auto valueType = typeSpec ();
  string id = m_token.lexeme;
  bool isArray = false;
  match (state, ID, id);

  if (m_token.type == LBRACK)
  {
    isArray = true;
    match (state, LBRACK, "[");
    match (state, RBRACK, "]");
  }
  
  ParameterNode* parameter = new ParameterNode(valueType, id, m_token.lineNum, m_token.colNum, isArray);
  paramVec.push_back (parameter);
}

/*********************************************************************/

CompoundStatementNode*
Parser::compoundStatement ()
{
  vector<VariableDeclarationNode*> declarations;
  vector<StatementNode*> statements;
  auto compStmtNode = new CompoundStatementNode(declarations, statements);
  
  string state = "CompoundStatement";

  match (state, LBRACE, "{");
  localDeclarations (compStmtNode);
  whileIsStatement (compStmtNode);
  match (state, RBRACE, "}");

  return compStmtNode;
}

/*********************************************************************/

void
Parser::localDeclarations (auto parent)
{
  string state = "LocalDeclarations";
  while (startDeclaration ())
  {
    ValueType type = typeSpec ();
    string id = m_token.lexeme;
    match (state, ID, id);
    varDeclaration (type, id, parent);
  }
}

/*********************************************************************/

void
Parser::whileIsStatement (auto parent)
{
  string state = "While Is Statement";
  while (isStatement ())
  {
    parent -> statements.push_back (statement ());
  }
}

/*********************************************************************/

StatementNode*
Parser::statement ()
{
  string state = "Statement";
  
  switch (m_token.type)
  {
  case ID:
  case NUM:
  case LPAREN:
  case SEMI:
    return expressionStatement ();
  case LBRACE:
    return compoundStatement ();
  case IF:
    return selectionStatement ();
  case WHILE:
    return iterationStatement ();
  case RETURN:
    return returnStatement ();
  default:
    printError (";', '{',  'if', 'while' or 'return", "statement");
  }
  return nullptr;
}


/*********************************************************************/

ExpressionStatementNode*
Parser::expressionStatement ()
{
  string state = "Expression Statement";
  ExpressionNode* expr = nullptr;
  if (isExpressionStmt ())
  {
    expr = expression ();
  }
  match (state, SEMI, ";");
  return new ExpressionStatementNode(expr);
}

/*********************************************************************/

IfStatementNode*
Parser::selectionStatement ()
{
  string state = "Selection Statement";
  match (state, IF, "if");
  match (state, LPAREN, "(");
  ExpressionNode* expr = expression ();
  match (state, RPAREN, ")");
  StatementNode* thenStmt = statement ();
  StatementNode* elseStmt = nullptr;

  if (m_token.type == ELSE)
  {
    match (state, ELSE, "else");
    elseStmt = statement ();
  }
  
  return new IfStatementNode(expr, thenStmt, elseStmt);
}

/*********************************************************************/

WhileStatementNode*
Parser::iterationStatement ()
{
  string state = "IterationStatement";
  match (state, WHILE, "while");
  match (state, LPAREN, "(");
  ExpressionNode* expr = expression ();
  match (state, RPAREN, ")");
  StatementNode* stmt = statement ();

  return new WhileStatementNode (expr, stmt);
}

/*********************************************************************/

ReturnStatementNode*
Parser::returnStatement ()
{
  string state = "ReturnStatement";
  match (state, RETURN, "return");
  ReturnStatementNode* retNode = nullptr;
  
  if (isExpressionStmt ())
  {
    ExpressionNode* exprNode = expression ();
    retNode = new ReturnStatementNode(exprNode);
  }
  else
  {
    retNode = new ReturnStatementNode();
  }
  match (state, SEMI, ";");
  
  return retNode;
}

/*********************************************************************/
ExpressionNode*
Parser::expression ()
{
  string state = "Expression";

  if (m_token.type == ID)
  {
    m_callID = m_token.lexeme;
    match (state, ID, "ID");
    if (m_token.type == LPAREN)
    {
      m_matchedID = true;
      return simpleExpression ();
    }
    else 
    {
      VariableExpressionNode* varNode  = m_varNode = variable (m_callID);
     

      if (m_token.type == ASSIGN)
      {
	match (state, ASSIGN, "=");
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

/*********************************************************************/

// var --> ID [ '[' expression ']' ]
VariableExpressionNode*
Parser::variable (string id)
{
  string state = "Variable";
  
  VariableExpressionNode* varNode = nullptr;
  if (m_token.type == LBRACK)
  {
    match (state, LBRACK, "[");
    ExpressionNode* expr = expression ();
    match (state, RBRACK, "]");
    varNode = new SubscriptExpressionNode(id, expr, m_token.lineNum, m_token.colNum);
  }
  else
  {
    varNode = new VariableExpressionNode(id, m_token.lineNum, m_token.colNum);
  }
  return varNode;
}

/*********************************************************************/

// simple-expression --> additive-expression [ relop additive-expression ]
ExpressionNode*
Parser::simpleExpression ()
{
  string state = "SimpleExpression";
  
  ExpressionNode* lhs = additiveExpression ();
  if (isRelop ())
  {
    RelationalOperatorType op = relop ();
    ExpressionNode* rhs = additiveExpression ();
    auto relNode = new RelationalExpressionNode(op, lhs, rhs);
    return relNode;
  }
  return lhs;
}

/*********************************************************************/

// relop --> <= | < | > | >= | == | !=
RelationalOperatorType
Parser::relop ()
{
  string state = "RelationOperation";
  switch (m_token.type)
  {
  case LTE:
    match (state, LTE, "<=");
    return   RelationalOperatorType::LTE;

  case LT:
    match (state, LT, "<");
    return   RelationalOperatorType::LT;

  case GT:
    match (state, GT, ">");
    return   RelationalOperatorType::GT;

  case GTE:
    match (state, GTE, ">");
    return   RelationalOperatorType::GTE;

  case EQ:
    match (state, EQ, "==");
    return   RelationalOperatorType::EQ;

  case NEQ:
    return   RelationalOperatorType::NEQ;

  default:
    break;
  }
  return RelationalOperatorType::ERROR;
}

/***********************************************************************/

// additive-expression --> term { addop term }
ExpressionNode*
Parser::additiveExpression ()
{
  string state = "Additive Expresion";
  
  ExpressionNode* lhs = term ();
  while (m_token.type == PLUS || m_token.type == MINUS)
  {
    AdditiveOperatorType op = addop ();
    ExpressionNode* rhs = term ();
    auto addNode = new AdditiveExpressionNode(op, lhs, rhs);
    return addNode;
  }
  return lhs;
	
}

/*********************************************************************/

// addop --> + | -
AdditiveOperatorType
Parser::addop ()
{
  string state = "Add Operation";
  if (m_token.type == PLUS)
  {
    match (state, PLUS, "+");
    return  AdditiveOperatorType::PLUS;
  }
  else
  {
    match (state, MINUS, "-");
    return  AdditiveOperatorType::MINUS;
  }
}

/*********************************************************************/

// term --> factor { mulop factor }
ExpressionNode*
Parser::term ()
{
  string state = "Term";
  
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

/*********************************************************************/

// mulop --> * | /
MultiplicativeOperatorType
Parser::mulop ()
{
  string state = "Multiply Operation";
  if (m_token.type == TIMES)
  {
    match (state, TIMES, "*");
    return   MultiplicativeOperatorType::TIMES;
  }
  else
  {
    match (state, DIVIDE, "/");
    return   MultiplicativeOperatorType::DIVIDE;
  }
}

/*********************************************************************/

ExpressionNode*
Parser::factor ()
{
  string state = "Factor";
  
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
      match (state, LPAREN, "(");
      ExpressionNode* expr =  expression ();
      match (state, RPAREN, ")");
      return expr;
    }
    else if (m_token.type == ID)
    {
      string id = m_token.lexeme;
      
      match (state, ID, "ID");
      if (m_token.type == LPAREN)
      {
	return call (id);
      }
      else
      {
	return variable (id);
      }
    }
    else if (m_token.type == NUM)
    {
      int num = m_token.number;
      match (state, NUM, "num");
      return new IntegerLiteralExpressionNode(num);
    }
  }
  return nullptr;
}

/*********************************************************************/

CallExpressionNode*
Parser::call (string id)
{
  string state = "Call";
  
  match (state, LPAREN, "(");
  vector<ExpressionNode*>  argNodeList = args ();
  match (state, RPAREN, ")");

  return new CallExpressionNode(id, argNodeList, m_token.lineNum, m_token.colNum);
}

/*********************************************************************/

vector<ExpressionNode*>
Parser::args ()
{
  string state = "Arguments";
  
  vector<ExpressionNode*> arguments;
  if (isArgAnExpression ())
  {
    argList (arguments);
  }
  
  return arguments;
}

/*********************************************************************/

void
Parser::argList (vector<ExpressionNode*> & args)
{
  string state = "Argument List";

  args.push_back (expression ());
  while (m_token.type == COMMA)
  {
    match (state, COMMA, ",");
    args.push_back (expression ());
  }
}

/*********************************************************************/

void
Parser::match (const string state, const TokenType expectedToken,
	       const string lexeme)
{

  if (m_token.type == expectedToken)
  {
    getToken ();
  }
  else
  {
    printError (state, lexeme);
  }
}

/*********************************************************************/

void
Parser::printErrorBeg (const string stateLabel)
{
  cout << "Error while parsing ";
  cout << "'" << stateLabel << "'";
  cout << " on line ";
  cout << m_token.lineNum;
  cout << ", column " << m_token.colNum << "\n";
}

/*********************************************************************/

void
Parser::printErrorEnd (const string lexeme)
{
  cout << "Encountered ";
  cout << "'" << m_token.lexeme << "'\n";
  cout << "  while expecting ";
  cout << "'" << lexeme << "'";
  cout << endl;
}

/*********************************************************************/

void
Parser::printError (const string stateLabel, const string lexeme)
{
  printErrorBeg (stateLabel);
  printErrorEnd (lexeme);
  exit (1);
}

/*********************************************************************/


bool
Parser::startDeclaration ()
{
  return (m_token.type == INT || m_token.type == VOID);
}


bool
Parser::isStatement ()
{
  return (m_token.type == IF     ||
	  m_token.type == WHILE  ||
	  m_token.type == RETURN ||
	  m_token.type == SEMI   ||
	  m_token.type == INT    ||
	  m_token.type == VOID   ||
	  m_token.type == LBRACE ||
	  m_token.type == ID);
}

/*********************************************************************/

bool
Parser::isExpressionStmt ()
{
  return (m_token.type == ID      ||
	  m_token.type == NUM     ||
	  m_token.type == LPAREN);
}

/*********************************************************************/

bool
Parser::isRelop ()
{
  return (m_token.type == LTE ||
	  m_token.type == LT  ||
	  m_token.type == GT  ||
	  m_token.type == GTE ||
	  m_token.type == EQ  ||
	  m_token.type == NEQ);
}

/*********************************************************************/

bool
Parser::isArgAnExpression ()
{
  string state = "IsArgAnExpression";
  return (m_token.type == LPAREN || m_token.type == ID || m_token.type == NUM);
}


/*********************************************************************/
