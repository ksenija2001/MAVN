/* Autor: Ksenija Barakovic Datum: 10.06.2022. */
#ifndef __RESOURCE_ALLOCATION__
#define __RESOURCE_ALLOCATION__

#include "InterferenceGraph.h"
#include "IR.h"

/**
 * Performs resource allocation algorithm, returns true if no spill is detected, false otherwise
 */
bool doResourceAllocation(std::stack<Variable*>* simplificationStack, InterferenceGraph* ig);

#endif