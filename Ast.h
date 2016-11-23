
/********************************************************************/
// C- AST Header File
// Design by CSCI 435: Compilers class
// Fall 2014
// Fall 2016

#ifndef C_MINUS_AST_H
#define C_MINUS_AST_H

/********************************************************************/
// System Includes

#include <string>
#include <vector>

/********************************************************************/
// Using Declarations

using std::string;
using std::vector;

/********************************************************************/
// Forward Class Declarations

// Visitor
struct IVisitor;

// Abstract Node
struct Node;

// Root Node
struct ProgramNode;

// Declaration Nodes
struct DeclarationNode;
struct FunctionDeclarationNode;
struct ParameterNode;
struct VariableDeclarationNode;
struct ArrayDeclarationNode;

// Statement Nodes
struct StatementNode;
struct CompoundStatementNode;
struct IfStatementNode;
struct WhileStatementNode;
struct ForStatementNode;
struct ReturnStatementNode;
struct ExpressionStatementNode;

// Expression Nodes
struct ExpressionNode;
struct AssignmentExpressionNode;
struct VariableExpressionNode;
struct SubscriptExpressionNode;
struct CallExpressionNode;
struct AdditiveExpressionNode;
struct MultiplicativeExpressionNode;
struct RelationalExpressionNode;
struct UnaryExpressionNode;
struct IntegerLiteralExpressionNode;

/********************************************************************/
//  Enum Classes

enum class AdditiveOperatorType
{
  PLUS, MINUS
};

enum class MultiplicativeOperatorType
{
  TIMES, DIVIDE
};

enum class RelationalOperatorType
{
  LT, LTE, GT, GTE, EQ, NEQ
};

enum class UnaryOperatorType
{
  INCREMENT, DECREMENT
};

enum class ValueType
{
  VOID, INT, ARRAY
};

/********************************************************************/

struct Node
{
  virtual ~Node ();

  virtual void
  accept (IVisitor* visitor) = 0;
};

struct ProgramNode : Node
{
  ProgramNode ();

  ~ProgramNode ();

  void
  accept (IVisitor* visitor);

  vector<DeclarationNode*> declarations;
};

/********************************************************************/
// DECLARATION NODES

struct DeclarationNode : Node
{
  DeclarationNode (ValueType t, string id);

  ~DeclarationNode ();

  virtual void
  accept (IVisitor* visitor) = 0;

  ValueType valueType;
  string identifier;

  // Set when the symbol table is built
  // Used for code gen
  int nestLevel;
};

struct FunctionDeclarationNode : DeclarationNode
{
  FunctionDeclarationNode (ValueType t, string id,
    vector<ParameterNode*> params, CompoundStatementNode* body);

  ~FunctionDeclarationNode ();

  void
  accept (IVisitor* visitor);

  vector<ParameterNode*> parameters;
  CompoundStatementNode* functionBody;
};

struct VariableDeclarationNode : DeclarationNode
{
  VariableDeclarationNode (ValueType t, string id);

  ~VariableDeclarationNode ();

  void
  accept (IVisitor* visitor);

  // Used for code gen
  // Parameters and local variables are stored at addresses
  //   offset from the frame pointer (ebp)
  int framePointerOffset;
};

struct ArrayDeclarationNode : VariableDeclarationNode
{
  ArrayDeclarationNode (ValueType t, string id, size_t size);

  ~ArrayDeclarationNode ();

  void
  accept (IVisitor* visitor);

  size_t size;
};

struct ParameterNode : DeclarationNode
{
  ParameterNode (ValueType t, string id, bool isArray);

  ~ParameterNode ();

  void
  accept (IVisitor* visitor);

  bool isArray;
};

/********************************************************************/
// STATEMENT NODES

struct StatementNode : Node
{
  ~StatementNode ();

  void
  accept (IVisitor* visitor);
};

struct CompoundStatementNode : StatementNode
{
  CompoundStatementNode (vector<VariableDeclarationNode*> declarations,
                         vector<StatementNode*> stmts);

  ~CompoundStatementNode ();

  void
  accept (IVisitor* visitor);

  vector<VariableDeclarationNode*> declarations;
  vector<StatementNode*> statements;
};

struct IfStatementNode : StatementNode
{
  IfStatementNode (ExpressionNode* expr,
                   StatementNode* thenStmt,
                   StatementNode* elseStmt = nullptr);

  ~IfStatementNode ();

  void
  accept (IVisitor* visitor);

  ExpressionNode* conditionalExpression;
  StatementNode* thenStatement;
  StatementNode* elseStatement;
};

struct WhileStatementNode : StatementNode
{
  WhileStatementNode (ExpressionNode* expr, StatementNode* stmt);

  ~WhileStatementNode ();

  void
  accept (IVisitor* visitor);

  ExpressionNode* conditionalExpression;
  StatementNode* body;
};

struct ForStatementNode : StatementNode
{
  ForStatementNode (ExpressionNode* e1,
                    ExpressionNode* e2,
                    ExpressionNode* e3,
                    StatementNode* s);

  ~ForStatementNode ();

  void
  accept (IVisitor* visitor);

  ExpressionNode* initializer;
  ExpressionNode* condition;
  ExpressionNode* updater;
  StatementNode*  body;
};

struct ReturnStatementNode : StatementNode
{
  ReturnStatementNode (ExpressionNode* expr = nullptr);

  ~ReturnStatementNode ();

  void
  accept (IVisitor* visitor);

  ExpressionNode* expression;
};

struct ExpressionStatementNode : StatementNode
{
  ExpressionStatementNode (ExpressionNode* expr);

  ~ExpressionStatementNode ();

  void
  accept (IVisitor* visitor);

  ExpressionNode* expression;
};

/********************************************************************/
// EXPRESSION NODES

struct ExpressionNode : Node
{
  ~ExpressionNode ();

  void
  accept (IVisitor* visitor);
};

struct AssignmentExpressionNode : ExpressionNode
{
  AssignmentExpressionNode (VariableExpressionNode* var,
                            ExpressionNode* expr);

  ~AssignmentExpressionNode ();

  void
  accept (IVisitor* visitor);

  VariableExpressionNode* variable;
  ExpressionNode* expression;
};

struct VariableExpressionNode : ExpressionNode
{
  VariableExpressionNode (string id);

  ~VariableExpressionNode ();

  void
  accept (IVisitor* visitor);

  string identifier;
};

struct SubscriptExpressionNode : VariableExpressionNode
{
  SubscriptExpressionNode (string id, ExpressionNode* index);

  ~SubscriptExpressionNode ();

  void
  accept (IVisitor* visitor);

  ExpressionNode* index;
};

struct CallExpressionNode : ExpressionNode
{
  CallExpressionNode (string id, vector<ExpressionNode*> args);

  ~CallExpressionNode ();

  void
  accept (IVisitor* visitor);

  string identifier;
  vector<ExpressionNode*> arguments;
};

struct AdditiveExpressionNode : ExpressionNode
{
  AdditiveExpressionNode (AdditiveOperatorType addop,
                          ExpressionNode* lhs,
                          ExpressionNode* rhs);

  ~AdditiveExpressionNode ();

  void
  accept (IVisitor* visitor);

  AdditiveOperatorType addOperator;
  ExpressionNode* left;
  ExpressionNode* right;
};

struct MultiplicativeExpressionNode : ExpressionNode
{
  MultiplicativeExpressionNode (MultiplicativeOperatorType multop,
                                        ExpressionNode* lhs,
                                        ExpressionNode* rhs);

  ~MultiplicativeExpressionNode ();

  void
  accept (IVisitor* visitor);

  MultiplicativeOperatorType multOperator;
  ExpressionNode* left;
  ExpressionNode* right;
};

struct RelationalExpressionNode : ExpressionNode
{
  RelationalExpressionNode (RelationalOperatorType relop,
                            ExpressionNode* lhs,
                            ExpressionNode* rhs);

  ~RelationalExpressionNode ();

  void
  accept (IVisitor* visitor);

  RelationalOperatorType relationalOperator;
  ExpressionNode* left;
  ExpressionNode* right;
};


struct UnaryExpressionNode : ExpressionNode
{
  UnaryExpressionNode (UnaryOperatorType unaryOp,
                       VariableExpressionNode* var);

  ~UnaryExpressionNode ();

  void
  accept (IVisitor* visitor);

  UnaryOperatorType unaryOperator;
  VariableExpressionNode* variable;
};

struct IntegerLiteralExpressionNode : ExpressionNode
{
  IntegerLiteralExpressionNode (int value);

  ~IntegerLiteralExpressionNode ();

  void
  accept (IVisitor* visitor);

  int value;
};

#endif