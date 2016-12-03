/*
  Filename   : CMinusDriver.h
  Author     : Ian T. Murry
  Course     : CSCI 435-01
*/

/*********************************************************************/
// Macro guard

#ifndef CMINUS_PARSER_H
#define CMINUS_PARSER_H

/*********************************************************************/
// System includes

#include <vector>
#include <string>

/***********************************************************************/
// Local includes

#include "CMinusToken.h"
#include "Ast.h"

/***********************************************************************/
// Using declarations

using std::vector;
using std::string;

/*********************************************************************/

class Parser
{
public:

  Parser (const std::vector<Token> & tokenBuffer);

  bool
  parse ();
  
  bool
  symbolTable (string name);
  
  void
  print (string name);

  /*********************************************************************/

private:

  void
  getToken ();

  bool
  startDeclaration ();

  void
  declarations (auto parent);
  
  void
  varDeclaration (ValueType type, string id, auto parent);
  
  ValueType
  typeSpec ();
  
  void
  funcDeclaration (ValueType type, string id);
  
  CompoundStatementNode*
  compoundStatement ();
  
  void
  params (std::vector<ParameterNode*> & paramVec);

  void
  paramList (std::vector<ParameterNode*> & paramVec);

  void
  param (std::vector<ParameterNode*> & paramVec);

  void
  localDeclarations (auto parent);

  void
  whileIsStatement (auto parent);

  void
  statementList (auto parent);

  StatementNode*
  statement ();

  ExpressionStatementNode*
  expressionStatement ();

  IfStatementNode*
  selectionStatement ();

  WhileStatementNode*
  iterationStatement ();

  ReturnStatementNode*
  returnStatement ();

  ExpressionNode*
  expression ();

  ExpressionNode*
  simpleExpression ();

  VariableExpressionNode*
  variable (string id);

  bool
  isRelop ();

  RelationalOperatorType
  relop ();

  ExpressionNode*
  additiveExpression ();

  AdditiveOperatorType
  addop ();

  ExpressionNode*
  term ();

  MultiplicativeOperatorType
  mulop ();

  ExpressionNode*
  factor ();

  CallExpressionNode*
  call (string id);

  vector<ExpressionNode*>
  args ();

  void
  argList (vector<ExpressionNode*> & args);

  bool
  isArgAnExpression ();

  void
  match (const std::string state, const TokenType expectedToken,
	 const std::string lexeme);
  bool
  isStatement ();

  bool
  isExpressionStmt ();

  void
  printErrorBeg (const std::string stateLabel);

  void
  printErrorEnd (const std::string lexeme);

  void
  printError (const std::string stateLabels, const std::string lexeme);

private:

  Token m_token;
  const std::vector<Token> m_tokenBuffer;
  size_t m_bufferPos;
  bool m_matchedID;
  string m_callID;
  VariableExpressionNode* m_varNode;
  ProgramNode astRoot;
};

/*********************************************************************/

#endif

/*********************************************************************/
