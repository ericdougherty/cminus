//
//  SymbolTable.cpp
//  SymbolTable
//
//  Created by Ian Murry on 11/27/16.
//
//

#include "SymbolTable.h"

using std::unique_ptr;
using std::string;
/********************************************************************/

SymbolTable::SymbolTable ()
    : m_nestLevel (0), m_table(vector<unique_ptr<ScopeTable>)
    {}
/********************************************************************/

SymbolTable::~SymbolTable()
    {
    }

/********************************************************************/

void
SymbolTable::enterScope()
    {
    ++m_nestLevel;
    unique_ptr<ScopeTable> newScope;
    m_table.push_back(newScope);
    }

/********************************************************************/

void
SymbolTable::exitScope ()
    {
    --m_nestLevel;
    m_table.pop_back();
    }

/********************************************************************/

bool
SymbolTable::insert (DeclarationNode* declarationPtr)
    {
    auto iter = m_table[m_nestLevel]->insert({declarationPtr->identifier, declarationPtr});
    return iter.second;
    }

/********************************************************************/

DeclarationNode*
SymbolTable::lookup(string name)
    {
    for (auto i = m_nestLevel; i != 0; --i)
	{
	 auto node = m_table[i] -> find(name);
	 if (node != end(m_table))
	     {
		 return node;
	     }
	}
    return nullptr;
    }
