/*
  Filename   : AssemblyEmitter.h
  Author     : Eric Dougherty & Ian Murry
  Course     : CSCI 435
*/

/********************************************************************/
// Assembly Emitter
//   Utility class to write assembly instructions to a file
//
// CSCI 435: Compilers class
// Fall 2014, 2016

#ifndef ASSEMBLY_EMITTER_H
#define ASSEMBLY_EMITTER_H

/********************************************************************/

#include <fstream>
#include <string>

/********************************************************************/
// Using declarations

/********************************************************************/

class AssemblyEmitter
{
public:

	AssemblyEmitter (std::ofstream& strm);
  	~AssemblyEmitter ();

	// Create a new label
	std::string
	createUniqueLabel ();

	// Generic emission methods
	void
	emitComment (const std::string& comment);

	void
	emitLabel (const std::string& label, const std::string& comment = "");

	void
	emitInstruction (const std::string& operation,
		const std::string& operands = "",
		const std::string& comment = "");

	void
	emitOperation (const std::string& operation);

	void
	emitOperands (const std::string& operands, const std::string& comment);

	void
	emitFunctionDeclaration (const std::string& name);

	void
	emitPrologue (int localVarSpace);

	void
	emitEpilogue ();

	// Concrete emission methods
	void
	emitSeparator ();

	void
	emitInputFunction ();

	void
	emitOutputFunction ();

	void
	closeFile();

private:

	std::ofstream& 	outFile;
	int            	labelCounter;
};

#endif