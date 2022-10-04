/* Autor: Ksenija Barakovic Datum: 10.06.2022. */

#include "ResourceAllocation.h"

using namespace std;

bool doResourceAllocation(std::stack<Variable*>* simplificationStack, InterferenceGraph* ig) 
{
	bool first = true;
	bool last = false;
	int activeColor = 1;
	vector<Regs> usedColors;
	usedColors.resize(ig->m_size);
	Variable* v;
	int lastPos = 0;

	while (!simplificationStack->empty()) 
	{
		v = simplificationStack->top();

		int pos = v->getPosition();
		if (first) 
		{
			v->setAssignment((Regs)activeColor);
			usedColors[pos] = (Regs)activeColor;
			first = false;
			lastPos = v->getPosition();
		}
		else 
		{
			//If an interference exists between the current and last variable the color has to be changed
			if (ig->m_values[pos][lastPos] == __INTERFERENCE__) 
			{
				while (usedColors[lastPos] == (Regs)activeColor) 
				{
					if (activeColor > __REG_NUMBER__ - 1) 
					{
						return false;
					}
					activeColor++;
				}
			}
			v->setAssignment((Regs)activeColor);
			usedColors[pos] = (Regs)activeColor;
			lastPos = v->getPosition();
		}
		simplificationStack->pop();
	}
	return true;
}