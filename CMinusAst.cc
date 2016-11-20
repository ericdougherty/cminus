/********************************************************************/


/********************************************************************/
// System Includes

#include <iostream>
#include "CMinusAst.h"

/********************************************************************/
// Using Declarations

using std::cout;
using std::endl;

/********************************************************************/

void
PrintVisitor::visit (ProgramNode* node)
{	
	level = 0;
	printLevel ();
	cout << "Program" << endl;
	++level;
	for (auto child : node -> declarations)
	{
		child -> accept(this);
	}
	--level;
}

void
PrintVisitor::visit (VariableDeclarationNode* node)
{	
	printLevel ();
	printNodeInfo("Variable", node);
}

void
PrintVisitor::visit (FunctionDeclarationNode* node)
{	
	printLevel ();
	printNodeInfo("Function", node);

	++level;
	for (auto child : node -> parameters)
	{
		child -> accept(this);
	}
	--level;
}


void
PrintVisitor::visit (ParameterNode* node)
{
	printLevel ();
	printNodeInfo("Parameter", node);

}

void
PrintVisitor::printLevel ()
{
	for (size_t i = 0; i < level; ++i)
	{
		cout << "|   ";
	}
	

}
/********************************************************************/

Node::~Node ()
{}

/********************************************************************/

ProgramNode::ProgramNode (vector<DeclarationNode*> declarations)
	:declarations(declarations)
	{}


ProgramNode::~ProgramNode()
{
	for (auto decl : declarations)
		delete decl;
	declarations.clear ();
}
void
ProgramNode::accept (IVisitor* visitor)
{
	visitor -> visit(this);
}

/********************************************************************/


DeclarationNode::DeclarationNode (ValueType t, string id)
	:valueType(t), identifier(id), nestLevel(0)
	{} 

DeclarationNode::~DeclarationNode ()
{}
/********************************************************************/


VariableDeclarationNode::VariableDeclarationNode (ValueType t, string id)
	:DeclarationNode(t, id)
	{}


VariableDeclarationNode::~VariableDeclarationNode()
{}

void
VariableDeclarationNode::accept (IVisitor* visitor)
{
	visitor -> visit(this);
}

/********************************************************************/


FunctionDeclarationNode::FunctionDeclarationNode (ValueType t, string id, 
			    vector<ParameterNode*> params) //, CompoundStatementNode* body)
	:DeclarationNode(t, id), parameters(params)//, functionBody(body)	
	{}


FunctionDeclarationNode::~FunctionDeclarationNode()
{
	for (auto param : parameters)
		delete param;
	parameters.clear ();
}

void
FunctionDeclarationNode::accept (IVisitor* visitor)
{
	visitor -> visit(this);
}

/********************************************************************/

ParameterNode::ParameterNode (ValueType t, string id, bool isArray)
	:DeclarationNode(t, id), isArray (isArray)
	{}


ParameterNode::~ParameterNode()
{}

void
ParameterNode::accept (IVisitor* visitor)
{
	visitor -> visit(this);
}

/********************************************************************/
void
printNodeInfo (string nodeType, auto node)
{
	cout << nodeType << ": " << node -> identifier << ": ";
	string type = (node -> valueType == ValueType::INT) ? "int" : "void"; 
	cout << type << " type" << endl;
}
/********************************************************************/