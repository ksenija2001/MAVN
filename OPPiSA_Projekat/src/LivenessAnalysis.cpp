/* Autor: Ksenija Barakovic Datum: 08.06.2022. */

#include "LivenessAnalysis.h"

using namespace std;

bool VarExists(Variables::iterator start, Variables::iterator end, Variable* d) 
{
	for (auto i = start; i != end; i ++ ) 
	{
		if ((*i)->getName() == d->getName()) 
		{
			return true;
		}
	}
	return false;
}

void doLivenessAnalysis(Instructions instructions) 
{
	auto it = instructions.begin();
	auto it1 = it; 
	it1++; //iterator to the next instruction
	auto it2 = it; //itertor to the last instruction
	auto ending = instructions.end();
	ending--;

	//Defining successors and predecessors for each instruction
	for (; it != instructions.end(); it++) 
	{
		if ((*it)->m_type != I_BEQ && (*it)->m_type != I_B && (*it)->m_type != I_BLTZ) 
		{
			//if it is equal to instructions.end(), the last instruction has no succcessors
			if (it != ending) 
			{
				(*it)->m_succ.push_back(*it1);
				it1++;
			}

			//if it is at the beginning the first instruction has no predeccessors
			if (it != instructions.begin()) 
			{
				it2 = it;
				it2--;
				(*it)->m_pred.push_back(*it2);
			}
				
			for (auto i = (*it)->m_dst.begin(); i != (*it)->m_dst.end(); i++) 
			{
				if ((*i)->getType() == Variable::REG_VAR) 
				{
					(*it)->m_def.push_back(*i);
				}
			}

			for (auto i = (*it)->m_src.begin(); i != (*it)->m_src.end(); i++) 
			{
				if ((*i)->getType() == Variable::REG_VAR) 
				{
					(*it)->m_use.push_back(*i);
				}
			}
		}
		else if ((*it)->m_type == I_BEQ || (*it)->m_type == I_BLTZ) 
		{
			Variable dst = (**(*it)->m_dst.begin());
			int pos = dst.getPosition();
			auto it2 = next(instructions.begin(), pos);

			//adding instruction to where the program will jump to if 
			//the condition is fulfilled 
			(*it)->m_succ.push_back(*it2);

			//adding next instruction to successors
			if (it != ending) 
			{
				(*it)->m_succ.push_back(*it1);
				it1++;
			}

			//adding preceding instruction to preseccessors
			if (it != instructions.begin())
			{
				auto i = (*it2);
				i->m_pred.push_back(*it);
				it2 = it;
				it2--; 
				(*it)->m_pred.push_back(*it2);
			}

			
			
			
			for (auto i = (*it)->m_src.begin(); i != (*it)->m_src.end(); i++) 
			{
				if ((*i)->getType() == Variable::REG_VAR) 
				{
					(*it)->m_use.push_back(*i);
				}
			}
		}
		else if ((*it)->m_type == I_B) 
		{
			Variable dst = (**(*it)->m_dst.begin());
			int pos = dst.getPosition();
			auto it2 = next(instructions.begin(), pos);
			//adding instruction to where the program will jump to 
			(*it)->m_succ.push_back(*it2);
			auto i = (*it2);
			i->m_pred.push_back(*it);
			//adding preceding instruction to preseccessors
			if (it != instructions.begin())
			{
				it2 = it;
				it2--;
				(*it)->m_pred.push_back(*it2);
			}
		}
	}

	//Iterates through all the instructions until no more changes occur
	int numEqual;
	do 
	{
		numEqual = 0;
		for (auto rit = instructions.rbegin(); rit != instructions.rend(); rit++) 
		{
			Variables in_temp;
			Variables out_temp;

			//Forming the out set
			for (auto i = (*rit)->m_succ.begin(); i != (*rit)->m_succ.end(); i++) 
			{
				for (auto j = (*i)->m_in.begin(); j != (*i)->m_in.end(); j++) 
				{
						out_temp.push_back(*j);
				}
			}

			out_temp.sort();
			out_temp.unique();

			//Forming the in set
			for (auto j = (*rit)->m_out.begin(); j != (*rit)->m_out.end(); j++) 
			{
				if (!VarExists((*rit)->m_def.begin(), (*rit)->m_def.end(), *j)) 
				{
					in_temp.push_back(*j);
				}
			}
			
			in_temp.insert(in_temp.begin(), (*rit)->m_use.begin(), (*rit)->m_use.end());

			in_temp.sort();
			in_temp.unique();

			//Checks if any changes have occured since the last iteration
			if ((out_temp == (*rit)->m_out) && (in_temp == (*rit)->m_in))
				numEqual++;

			(*rit)->m_out = out_temp;
			(*rit)->m_in = in_temp;
		}
	} while (numEqual != instructions.size());


}