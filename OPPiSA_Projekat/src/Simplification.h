/* Autor: Ksenija Barakovic Datum: 10.06.2022. */

#ifndef __SIMPLIFICATION__
#define __SIMPLIFICATION__

#include "IR.h"
#include "InterferenceGraph.h"

/**
 * Performs simplification algorithm and returns simplification stack
 */
std::stack<Variable*>* doSimplification(InterferenceGraph* ig, int degree);

#endif