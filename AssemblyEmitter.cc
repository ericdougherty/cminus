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
const uint COMMENT_INDENT_WIDTH = INDENT_WIDTH
																+ INSTRUCTION_WIDTH
																+ OPERANDS_WIDTH;
const uint FULL_WIDTH = 80;

/********************************************************************/

AssemblyEmitter::AssemblyEmitter (std::ofstream &strm)
	:m_outFile(strm)
{
	//was just outFile???
	m_outFile << std::left;
}


// ...

/********************************************************************/
// Utilities

string
AssemblyEmitter::createUniqueLabel ()
{
	return ".L" + std::to_string (m_labelCounter++);
}

/********************************************************************/
// Generic Emission methods

void
AssemblyEmitter::emitComment (const string& comment)
{
	m_outFile << setw (COMMENT_INDENT_WIDTH) << " "
			<< "# " << comment << endl;
}

/********************************************************************/

void
AssemblyEmitter::emitLabel (const string& label, const string& comment)
{
	m_outFile << setw (COMMENT_INDENT_WIDTH) << label + ":"
		<< "# " << comment << endl;
}

/********************************************************************/

void
AssemblyEmitter::emitInstruction (const string& operation,
					const string& operands,
					const string& comment)
{
	m_outFile << setw (INDENT_WIDTH) << " "
			<< setw (10) << operation << setw (22) << operands
			<< "# " << comment << endl;
}

// ...

/********************************************************************/
// Concrete emission methods

void
AssemblyEmitter::emitSeparator ()
{
	m_outFile << setfill ('=')
			<< setw (FULL_WIDTH) << "#" << endl
			<< setfill (' ');
}

// ...