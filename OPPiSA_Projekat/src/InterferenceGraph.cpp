/* Autor: Ksenija Barakovic Datum: 10.06.2022. */

#include "InterferenceGraph.h"

using namespace std;


//Compares positions of two variables
bool compareFunc(Variable* a, Variable* b) {
	return a->getPosition() < b->getPosition();
}

InterferenceGraph* doInterferenceGraph(Instructions* instructions) 
{
	Variables* var = new Variables();

	//Forming the variables list and finding all intereferences
	for (auto rit = instructions->begin(); rit != instructions->end(); rit++) 
	{
		for (auto dit = (*rit)->m_def.begin(); dit != (*rit)->m_def.end(); dit++) 
		{
			var->push_back(*dit);
		}
	}

	var->sort(compareFunc);
	var->unique();

	//Making the interference matrix
	vector<vector<int>> matrica;
	matrica.resize(var->size());
	for (auto& i : matrica) {
		i.resize(var->size());
	}

	//Filling the interferemce matrix
	for (auto rit = instructions->begin(); rit != instructions->end(); rit++) 
	{
		for (auto dit = (*rit)->m_def.begin(); dit != (*rit)->m_def.end(); dit++) 
		{
			for (auto oit = (*rit)->m_out.begin(); oit != (*rit)->m_out.end(); oit++) 
			{
				int i = (*dit)->getPosition();
				int j = (*oit)->getPosition();
				if (i != j) 
				{
					matrica[i][j] = __INTERFERENCE__;
					matrica[j][i] = __INTERFERENCE__;
				}
				else 
				{
					matrica[i][j] = __EMPTY__;
					matrica[j][i] = __EMPTY__;
				}
			}
		}
	}

	InterferenceGraph* ig = new InterferenceGraph(var->size(), var);
	ig->m_values = matrica;

	return ig;
}

void freeInterferenceGraph(InterferenceGraph* ig) 
{
	for (auto& i : ig->m_values)
		i.clear();
	ig->m_values.clear();

	delete ig->m_variables;
	delete ig;
}

void printInterferenceGraph(InterferenceGraph* ig) 
{
	int size = ig->m_size;
	vector<vector<int>> matrica(ig->m_values);
	for (int i = 0; i < size; i++) {
		if (i == 0) 
		{
			cout << "|  |";
			for (auto it = ig->m_variables->begin(); it != ig->m_variables->end(); it++) 
			{
				 cout << (*it)->getName() << "|";
			}
			cout << endl;
		}
		cout << "|";

		auto it = next(ig->m_variables->begin(), i);
		cout << (*it)->getName() << "|";

		for (int j = 0; j < size; j++) 
		{
			if (matrica[i][j] == __EMPTY__)
				cout << "  |";
			else if (matrica[i][j] == __INTERFERENCE__)
				cout << matrica[i][j] << " |";
			else
				cout << matrica[i][j] << "|";
		}
		cout << endl;
	}

}