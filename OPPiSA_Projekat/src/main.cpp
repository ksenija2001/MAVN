/* Autor: Ksenija Barakovic Datum: 06.06.2022. */

#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"
#include "Simplification.h"
#include "ResourceAllocation.h"
#include "GenerateOutput.h"

using namespace std;


int main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\simple.mavn";
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		SyntaxAnalysis syntax(lex);
		retVal = syntax.Do();

		if (retVal) {
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else {
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}

		Instructions instr = syntax.getInstructions();
		vector<Variable*> symbols = syntax.getSymbolTable();
		doLivenessAnalysis(instr);
		cout << "Liveness analysis finished." << endl;

		InterferenceGraph* ig;
		ig = doInterferenceGraph(&instr);
		cout << "Interference graph: " << endl;
		printInterferenceGraph(ig);

		cout << "Resource allocation starting..." << endl;

		stack<Variable*>* simplificationStack = doSimplification(ig, __REG_NUMBER__);
		if (simplificationStack == NULL)
		{
			printf("Spill detected!\n");
		}
		else
		{
			if (doResourceAllocation(simplificationStack, ig)) {
				cout << "Resource allocation successful." << endl;

				if (GenerateFile(fileName, instr, symbols))
				{
					cout << ".s file generated succesfully.";
				}
				else
				{
					throw runtime_error("\nException! Unable to open file!\n");
				}
			}
			else {
				throw runtime_error("\nException! Resource allocation failed!\n");
			}
		}

		if (ig != NULL)
			freeInterferenceGraph(ig);

	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
