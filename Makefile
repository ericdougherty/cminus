########################################################
# Dr. Zoppetti: Generic Makefile
# Modified by : Eric Dougherty
########################################################

########################################################
# Variable definitions
########################################################
# C++ compiler
#CXX := clang++
CXX := g++

# Include directories, prefaced with "-I"
INCDIRS  := 

# C++ compiler flags
# Use the first for debugging, the second for release
CXXFLAGS := -g -Wall -std=c++14 $(INCDIRS) 
#CXXFLAGS := -O3 -Wall -std=c++14 $(INCDIRS) 

# Linker. For C++ should be $(CXX).
LINK := $(CXX)

# Linker flags. Usually none.
LDFLAGS := 

# Library paths, prefaced with "-L". Usually none.
LDPATHS := 

# Libraries used, prefaced with "-l".
LDLIBS :=
#LDLIBS := -lpthread -lboost_mpi -lboost_serialization

OBJS := Lexer.o Parser.o Driver.o Ast.o PrintVisitor.o SymbolTable.o SymbolTableVisitor.o CodeGenVisitor.o AssemblyEmitter.o

#############################################################
# Rules
#   Rules have the form
#   target : prerequisites
#   	  recipe
#############################################################

Driver : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o cmin

Lexer.o : Lexer.cc Lexer.h

Parser.o : Parser.cc Parser.h Ast.h PrintVisitor.h SymbolTableVisitor.h CodeGenVisitor.h

Driver.o : Driver.cc Parser.h

Ast.o : Ast.cc Ast.h

PrintVisitor.o : PrintVisitor.cc PrintVisitor.h

SymbolTableVisitor.o : SymbolTableVisitor.cc SymbolTableVisitor.h

CodeGenVisitor.o: CodeGenVisitor.cc CodeGenVisitor.h

SymbolTable.o : SymbolTable.cc SymbolTable.h

AssemblyEmitter.o : AssemblyEmitter.cc AssemblyEmitter.h

#############################################################
# Type "make clean" to delete executables, object files,
#   core dumps, Emacs backup files, and generated files
#############################################################

.PHONY : clean
clean :
	$(RM) core *.o *~

#############################################################