/* Autor: Ksenija Barakovic Datum: 10.06.2022. */

#include "Simplification.h"

using namespace std;

//Sorts the variables by the number of links
bool compareFunc(pair<int, int> a, pair<int, int> b) 
{
	return a.second > b.second;
}

std::stack<Variable*>* doSimplification(InterferenceGraph* ig, int degree) 
{
	stack<Variable*>* simplificationStack = new stack<Variable*>();
	vector<vector<int>> matrica(ig->m_values);
	int size = ig->m_size;
	Variables* var = ig->m_variables;
	vector<pair<int, int>> pushStack; //stores the candidates from which one, with the most links, will be put on the stack
	vector<int> erased;
	
	//Repeats until all variables have been put on the stack or a spill has occured
	do 
	{
		pushStack.clear();
		for (int i = 0; i < size; i++) 
		{
			int links = 0;
			//If a variable has been put on the stack it's links are no longer examined
			if (find(erased.begin(), erased.end(), i) == erased.end()) 
			{
				for (int j = 0; j < size; j++) 
				{
					//If a variable has been put on the stack it's links are no longer examined
					if (find(erased.begin(), erased.end(), j) == erased.end()) 
					{
						if (matrica[i][j] == __INTERFERENCE__)
							links++;
					}
				}
				if (links < degree) 
				{
					pushStack.push_back(pair<int, int>(i, links));
				}
			}
		}

		//If none of the remaining variables can be put on the stack a spill has occured
		if (pushStack.empty()) {
			return nullptr;
		}

		sort(pushStack.begin(), pushStack.end(), compareFunc);

		//The Varaiable with the most links is being put on the stack and removed from the graph
		for (auto it = var->begin(); it != var->end(); it++) 
		{
			if ((*it)->getPosition() == pushStack.front().first) 
			{
				simplificationStack->push(*it);

				erased.push_back((*it)->getPosition());
				var->erase(it);

				break;
			}
		}
	} while (!var->empty());

	return simplificationStack;
}