/*
  Filename   : Ast.h
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

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
class IVisitor;

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
	LT, LTE, GT, GTE, EQ, NEQ, ERROR
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

class IVisitor
{
public:
	virtual void visit (ProgramNode* node) = 0;

	virtual void visit (DeclarationNode* node) = 0;
	virtual void visit (FunctionDeclarationNode* node) = 0;
	virtual void visit (VariableDeclarationNode* node) = 0;
	virtual void visit (ArrayDeclarationNode* node) = 0;
	virtual void visit (ParameterNode* node) = 0;

	virtual void visit (StatementNode* node) = 0;
	virtual void visit (CompoundStatementNode* node) = 0;
	virtual void visit (IfStatementNode* node) = 0;
	virtual void visit (WhileStatementNode* node) = 0;
	virtual void visit (ReturnStatementNode* node) = 0;
	virtual void visit (ExpressionStatementNode* node) = 0;

	virtual void visit (ExpressionNode* node) = 0;
	virtual void visit (AssignmentExpressionNode* node) = 0;
	virtual void visit (VariableExpressionNode* node) = 0;
	virtual void visit (SubscriptExpressionNode* node) = 0;
	virtual void visit (CallExpressionNode* node) = 0;
	virtual void visit (AdditiveExpressionNode* node) = 0;
	virtual void visit (MultiplicativeExpressionNode* node) = 0;
	virtual void visit (RelationalExpressionNode* node) = 0;
	virtual void visit (IntegerLiteralExpressionNode* node) = 0;
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
	DeclarationNode (ValueType t, string id, size_t lineNum, size_t colNum);

	~DeclarationNode ();

	virtual void
	accept (IVisitor* visitor) = 0;

	ValueType valueType;
	string identifier;
	size_t lineNum, colNum;

  // Set when the symbol table is built
  // Used for code gen
	int nestLevel;

  // Used for code gen
  // Parameters and local variables are stored at addresses
  //   offset from the frame pointer (ebp)
	int framePointerOffset;
	bool isArray;
};

struct FunctionDeclarationNode : DeclarationNode
{
	FunctionDeclarationNode (ValueType t, string id, size_t lineNum, size_t colNum,
		vector<ParameterNode*> params = vector<ParameterNode*>(), CompoundStatementNode* body = nullptr);

	~FunctionDeclarationNode ();

	void
	accept (IVisitor* visitor);

	vector<ParameterNode*> 	parameters;
	CompoundStatementNode* 	functionBody;
	int					localVarCount;
};

struct VariableDeclarationNode : DeclarationNode
{
	VariableDeclarationNode (ValueType t, string id, size_t lineNum, size_t colNum, bool isArray);

	~VariableDeclarationNode ();

	void
	accept (IVisitor* visitor);

	bool isArray;

};

struct ArrayDeclarationNode : VariableDeclarationNode
{
	ArrayDeclarationNode (ValueType t, string id, size_t lineNum, size_t colNum, bool isArray, size_t size);

	~ArrayDeclarationNode ();

	void
	accept (IVisitor* visitor);

	size_t size;
};

struct ParameterNode : VariableDeclarationNode
{
	ParameterNode (ValueType t, string id, size_t lineNum, size_t colNum, bool isArray);

	~ParameterNode ();

	void
	accept (IVisitor* visitor);

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
	VariableExpressionNode (string id, size_t lineNum, size_t colNum);

	~VariableExpressionNode ();

	void
	accept (IVisitor* visitor);

	string identifier;
	DeclarationNode* decl;
	size_t lineNum, colNum;
	ExpressionNode* index;

};

struct SubscriptExpressionNode : VariableExpressionNode
{
	SubscriptExpressionNode (string id, ExpressionNode* index, size_t lineNum, size_t colNum);

	~SubscriptExpressionNode ();

	void
	accept (IVisitor* visitor);
	ExpressionNode* index;
};

struct CallExpressionNode : ExpressionNode
{
	CallExpressionNode (string id, vector<ExpressionNode*> args, size_t lineNum, size_t colNum);

	~CallExpressionNode ();

	void
	accept (IVisitor* visitor);

	string identifier;
	vector<ExpressionNode*> arguments;
	DeclarationNode*  decl;
	size_t lineNum, colNum;
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

struct IntegerLiteralExpressionNode : ExpressionNode
{
	IntegerLiteralExpressionNode (int value);

	~IntegerLiteralExpressionNode ();

	void
	accept (IVisitor* visitor);

	int value;
};

#endif