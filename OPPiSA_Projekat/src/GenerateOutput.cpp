/* Autor: Ksenija Barakovic Datum: 11.06.2022. */
#include "GenerateOutput.h"

using namespace std;

vector<Variable*> g_symbols;

Variable* findSymbol(int pos, Variable::VariableType type) 
{
	for (auto it = g_symbols.begin(); it != g_symbols.end(); it++) {
		if ( (*it)->getPosition() == pos && (*it)->getType() == type) {
			return *it;
		}
	}
	return nullptr;
}

bool GenerateFile(string fileName, Instructions instructions, vector<Variable*> symbolTable) 
{
 	g_symbols = symbolTable;
	fileName.replace(fileName.find("mavn"), 4, "s");
	ofstream output(fileName);

	if (output.is_open()) {

		output << ".globl " << findSymbol(0, Variable::NO_TYPE)->getName() << endl;
		output << endl;

		output << ".data" << endl;
		for (auto it = g_symbols.begin(); it != g_symbols.end(); it++) {
			if ((*it)->getType() == Variable::MEM_VAR) {
				output << (*it)->getName() << ":" << setw(RIGHT_ALIGN) << ".word " << (*it)->getValue() << endl;
			}
		}
		output << endl;

		output << ".text" << endl;

		for (auto it = instructions.begin(); it != instructions.end(); it++) {
			//Handles labels
			Variable* id = findSymbol((*it)->m_position, Variable::NO_TYPE);
			if (id != NULL) {
				output << (*id).getName() << ":" << endl;
			}

			//Handles instructions
			Variables::iterator its = (*it)->m_src.begin();
			Variables::iterator itd = (*it)->m_dst.begin();
			switch ((*it)->m_type) 
			{
			case I_ADD:
				output << setw(RIGHT_ALIGN/2) << "add" << setw(RIGHT_ALIGN/2) << "$t" << (*itd)->getAssignment() << ", ";
				output << "$t" << (*its)->getAssignment() << ", ";
				its++;
				output << "$t" << (*its)->getAssignment() << endl;

				break;

			case I_ADDI:
				output << setw(RIGHT_ALIGN/2) << "addi" << setw(RIGHT_ALIGN/2) << "$t" << (*itd)->getAssignment() << ", ";
				output << "$t" << (*its)->getAssignment() << ", ";
				its++;
				output << (*its)->getName() << endl;

				break;

			case I_SUB:
				output << setw(RIGHT_ALIGN/2) << "sub" << setw(RIGHT_ALIGN/2) << "$t" << (*itd)->getAssignment() << ", ";
				output << "$t" << (*its)->getAssignment() << ", ";
				its++;
				output << "$t" << (*its)->getAssignment() << endl;

				break;

			case I_DIV:
				output << setw(RIGHT_ALIGN/2) << "div" << setw(RIGHT_ALIGN/2) << "$t" << (*itd)->getAssignment() << ", ";
				output << "$t" << (*its)->getAssignment() << endl;

				break;

			case I_MULT:
				output << setw(RIGHT_ALIGN/2) << "mult" << setw(RIGHT_ALIGN/2) << "$t" << (*itd)->getAssignment() << ", ";
				output << "$t" << (*its)->getAssignment() << endl;

				break;

			case I_LA:
				output << setw(RIGHT_ALIGN/2) << "la" << setw(RIGHT_ALIGN/2) << "$t" << (*itd)->getAssignment() << ", ";
				output << (*its)->getName() << endl;

				break;

			case I_LW:
				output << setw(RIGHT_ALIGN/2) << "lw" << setw(RIGHT_ALIGN/2) << "$t" << (*itd)->getAssignment() << ", ";
				output << (*its)->getName() << "(";
				its++;
				output << "$t" << (*its)->getAssignment() << ")" << endl;

				break;

			case I_LI:
				output << setw(RIGHT_ALIGN/2) << "div" << setw(RIGHT_ALIGN/2) << "$t" << (*itd)->getAssignment() << ", ";
				output << (*its)->getName() << endl;

				break;

			case I_SW:
				output << setw(RIGHT_ALIGN/2) << "sw" << setw(RIGHT_ALIGN/2) << "$t" << (*its)->getAssignment() << ", ";
				its++;
				output << (*its)->getName() << "(";
				its++;
				output << "$t" << (*its)->getAssignment() << ")" << endl;

				break;

			case I_BLTZ:
				output << setw(RIGHT_ALIGN/2) << "bltz" << setw(RIGHT_ALIGN/2) << "$t" << (*its)->getAssignment() << ", ";
				output << (*itd)->getName() << endl;

				break;

			case I_B:
				output << setw(RIGHT_ALIGN/2) << "b" << setw(RIGHT_ALIGN/2) << (*itd)->getName() << endl;

				break;

			case I_BEQ:
				output << setw(RIGHT_ALIGN/2) << "beq" << setw(RIGHT_ALIGN/2) << "$t" << (*its)->getAssignment() << ", ";
				its++;
				output << "$t" << (*its)->getAssignment() << ", ";
				output << (*itd)->getName() << endl;

				break;

			case I_NOP:
				output << setw(RIGHT_ALIGN/2) << "nop" << endl;

			default:
				break;

			}
		}
		output.close();
	}
	else {
		return false;
	}
	return true;
}
