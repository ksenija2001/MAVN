/* Autor: Ksenija Barakovic Datum: 11.06.2022. */
#ifndef __GENERATE_OUTPUT__
#define __GENERATE_OUTPUT__

#include "IR.h"

#include <fstream>
#include <iostream>
#include <iomanip>

/**
 * Function that generates the mips32 file from instructions and tokens analized in the mavn file
*/
bool GenerateFile(std::string fileName, Instructions instructions, std::vector<Variable*> symbolTable);

/**
 * Searches the symbol table for a variable on given position and of given type
*/
Variable* findSymbol(int pos, Variable::VariableType type);

#endif
