/* Autor: Ksenija Barakovic Datum: 07.06.2022. */
#ifndef __LIVENESS_ANALYSIS__
#define __LIVENESS_ANALYSIS__

#include "IR.h"

#include <algorithm>

/**
 * Performs liveness analysis on list of instructions
*/
void doLivenessAnalysis(Instructions instructions);

/**
 * Checks if variable from out set exists in list of def set variables
*/
bool VarExists(Variables::iterator start, Variables::iterator end, Variable* d);

#endif