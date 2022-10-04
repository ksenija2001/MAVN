/* Autor: Ksenija Barakovic Datum: 10.06.2022. */

#ifndef __INTERFERENCE_GRAPH__
#define __INTERFERENCE_GRAPH__

#include "IR.h"

#include <algorithm>

class InterferenceGraph 
{
public:
	std::vector<std::vector<int>> m_values;
	Variables* m_variables;
	int m_size;

	InterferenceGraph(int size, Variables* var) : m_size(size), m_variables(var) {
		m_values.resize(size);
		for (auto& i : m_values) {
			i.resize(size);
		}
	}
};

/**
* Function that releases resources of the interference graph
*/
void freeInterferenceGraph(InterferenceGraph* ig);

/**
* Function which makes an interference graph from instructions list.
*/
InterferenceGraph* doInterferenceGraph(Instructions* instructions);


/**
 * Function that prints interference graph values.
*/
void printInterferenceGraph(InterferenceGraph* ig);

#endif