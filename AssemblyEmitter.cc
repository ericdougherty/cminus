/********************************************************************/
// Assembly Emitter
//   Utility class to write assembly instructions to a file
//
// CSCI 435: Compilers class
// Fall 2014, 2016

/********************************************************************/
// System includes

#include <iostream>
#include <iomanip>

/********************************************************************/
// Local includes

#include "AssemblyEmitter.h"

/********************************************************************/
// Using declarations

using std::setw;
using std::setfill;
using std::endl;
using std::string;

using uint = unsigned int;

/********************************************************************/
// Constants for code formatting

const uint INDENT_WIDTH = 10;
const uint INSTRUCTION_WIDTH = 10;
const uint OPERANDS_WIDTH = 22;
const uint COMMENT_INDENT_WIDTH = INDENT_WIDTH + INSTRUCTION_WIDTH + OPERANDS_WIDTH;
const uint FULL_WIDTH = 80;

/********************************************************************/

AssemblyEmitter::AssemblyEmitter (std::ofstream &strm)
	:outFile(strm), labelCounter(0)
{
	outFile << std::left;
}

/********************************************************************/
AssemblyEmitter::~AssemblyEmitter ()
{}


// ...

/********************************************************************/
// Utilities

string
AssemblyEmitter::createUniqueLabel ()
{
	return ".L" + std::to_string (labelCounter++);
}

/********************************************************************/
// Generic Emission methods

void
AssemblyEmitter::emitComment (const string& comment)
{
	outFile << setw (COMMENT_INDENT_WIDTH) << " "
			<< "# " << comment << endl;
}

/********************************************************************/

void
AssemblyEmitter::emitLabel (const string& label, const string& comment)
{
	outFile << setw (COMMENT_INDENT_WIDTH) << label + ":"
		<< "# " << comment << endl;
}

/********************************************************************/

void
AssemblyEmitter::emitInstruction (const string& operation,
					const string& operands,
					const string& comment)
{
	outFile << setw (INDENT_WIDTH) << " "
			<< setw (10) << operation << setw (22) << operands
			<< "# " << comment << endl;
}

void
AssemblyEmitter::emitFunctionDeclaration (const std::string& name)
{
	outFile << ".globl " + name << endl; 
	outFile << name + ":" << endl;
}

// Emit a function prologue that allocates "localVarSpace" on the stack
void
AssemblyEmitter::emitPrologue (int localVarSpace)
{
	emitInstruction ("pushl", "%ebp", "Function prologue");
	emitInstruction ("movl", "%esp, %ebp", "");
	if (localVarSpace > 0)
	{
		emitInstruction ("subl", "$" + std::to_string(4 * localVarSpace) + ", %esp", "");
	}
		
}

// Emit a function epilogue that restores the stack and base pointers,
//   and returns
void
AssemblyEmitter::emitEpilogue ()
{
	emitInstruction ("movl", "%ebp, %esp", "Function epilogue");
	emitInstruction ("popl", "%ebp", "");
	emitInstruction ("ret", "", "Return to caller");
}


/********************************************************************/
// Concrete emission methods

void
AssemblyEmitter::emitSeparator ()
{
	outFile << setfill ('=')
			<< setw (FULL_WIDTH) << "#" << endl
			<< setfill (' ');
}

void
AssemblyEmitter::emitInputFunction ()
{
	emitSeparator();

	outFile << ".section       .rodata" << endl;
	outFile << ".inStr:        .string          \"%d\"" << endl;
	outFile << ".outStr:       .string          \"%d\\n\"" << endl;

	emitSeparator();

	emitInstruction(".text", "", "Processing function: input");
	emitFunctionDeclaration("input");

	emitPrologue(1);
	emitInstruction("pushl", "%esp", "");
	emitInstruction("pushl", "$.inStr", "");
	emitInstruction("call", "scanf", "");
	emitInstruction("movl", "8(%esp), %eax", "");

	emitEpilogue();
}

void
AssemblyEmitter::emitOutputFunction ()
{
	emitInstruction(".text", "", "Processing function: output");
	emitFunctionDeclaration("output");
	emitPrologue(0);

	emitInstruction("pushl", "8(%ebp)", "");
	emitInstruction("pushl", "$.outStr", "");
	emitInstruction("call", "printf", "");

	emitEpilogue();
}

void
AssemblyEmitter::closeFile()
{
	outFile.close();
}