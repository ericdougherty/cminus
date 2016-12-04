// Symbol Table Header File
// CSCI 435: Compilers class
// Fall 2016

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/********************************************************************/
// System Includes

#include <vector>
#include <string>
#include <unordered_map>
// For unique_ptr
#include <memory>

/********************************************************************/
// Local Includes

#include "Ast.h"

/********************************************************************/
// Using Declarations

using ScopeTable = std::unordered_map<std::string, DeclarationNode*>;

/********************************************************************/

class SymbolTable 
{
public:

  SymbolTable  ();
  ~SymbolTable ();
  
  void
  enterScope   ();

  void
  exitScope    ();

  bool 
  insert       (DeclarationNode* declarationPtr);
  
  DeclarationNode* 
  lookup       (std::string name);

  int
  getLevel();

  DeclarationNode*
  getPreviousDeclaration (string name, size_t nestLevel);


private:
  
  int  m_nestLevel;
  std::vector<std::unique_ptr<ScopeTable>> m_table;
};

#endif