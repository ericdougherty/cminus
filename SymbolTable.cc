//
//  SymbolTable.cpp
//  SymbolTable
//
//  Created by Ian Murry on 11/27/16.
//
//
#include <iostream>

#include "SymbolTable.h"

using std::unique_ptr;
using std::string;
using std::make_unique;
using std::cout;
using std::endl;

/********************************************************************/

SymbolTable::SymbolTable ()
    :m_nestLevel(-1)
{}

/********************************************************************/

SymbolTable::~SymbolTable ()
{}

/********************************************************************/

void
SymbolTable::enterScope ()
{
    ++m_nestLevel;
    m_table.push_back(unique_ptr<ScopeTable> (new ScopeTable));
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
    for (auto i = m_nestLevel; i >= 0; --i)
    {
     auto node = m_table[i] -> find (name);
     if (node != m_table[i] -> end ())
        {
         return node -> second;
        }
    }
    return nullptr;
}

/********************************************************************/

int
SymbolTable::getLevel()
{
    return m_nestLevel;
}

/********************************************************************/

DeclarationNode*
SymbolTable::getPreviousDeclaration (string name, size_t nestLevel)
{
    auto node = m_table[nestLevel] -> find (name);
    return node -> second;
}

/********************************************************************/

