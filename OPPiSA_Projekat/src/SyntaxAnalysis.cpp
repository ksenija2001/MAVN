/* Autor: Ksenija Barakovic Datum: 07.06.2022. */

#include "SyntaxAnalysis.h"

using namespace std;

//Stores information about position, type and value of variables
vector<Variable*> g_positions;
int g_posInstruction = 0;
int g_posReg = 0;
int g_posMem = 0;

Instructions g_instructions;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex) : lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin())
{
}

Instructions SyntaxAnalysis::getInstructions()
{
	return g_instructions;
}

vector<Variable*> SyntaxAnalysis::getSymbolTable() 
{
	return g_positions;
}

Variable* SyntaxAnalysis::checkTable(std::string name, Variable::VariableType type) 
{
	for (auto it = g_positions.begin(); it != g_positions.end(); it++) 
	{
		if (type == T_ID) 
		{
			if ((*it)->getName() == name && ((*it)->getType() == type || (*it)->getType() == T_FUNC))
				return *it;
		}
		else if ((*it)->getName() == name && (*it)->getType() == type)
			return *it;
	}
	return nullptr;
}

bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();

	Q();

	if (!errorFound) 
	{
		cout << "Syntax OK" << endl;
	}


	return !errorFound;
}

void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}

void SyntaxAnalysis::printTokenInfo(Token token)
{
	token.printTokenInfo();
}

void SyntaxAnalysis::eat(TokenType t)
{
	if (!errorFound)
	{

		if (currentToken.getType() == t)
		{
			cout << currentToken.getValue() << endl;

			//If end of file has been reached it will return all the way back to where Q has original been called from
			if (currentToken.getType() != T_END_OF_FILE)
				currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}

Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}

//All possible grammatically correct combinationt of nonterminal and terminal characters
void SyntaxAnalysis::Q() 
{
	if (!errorFound) 
	{
		S();

		if (currentToken.getType() == T_SEMI_COL) 
		{
			eat(T_SEMI_COL);

			L();
		}
		else {
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else {
		return;
	}
}

void SyntaxAnalysis::S() 
{
	if (!errorFound) 
	{
		if (currentToken.getType() == T_MEM) 
		{
			eat(T_MEM);

			if (currentToken.getType() == T_M_ID) 
			{
				Variable* t = checkTable(currentToken.getValue(), Variable::MEM_VAR);
				if (t != NULL) {
					printSyntaxError(currentToken);
					errorFound = true;
					return;
				}

				t = new Variable(Variable::MEM_VAR, currentToken.getValue(), g_posMem);				
				eat(T_M_ID);

				if (currentToken.getType() == T_NUM) 
				{
					t->setValue(currentToken.getValue());
					eat(T_NUM);

					g_positions.push_back(t);

					g_posMem++;
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else if (currentToken.getType() == T_REG) 
		{
			eat(T_REG);

			if (currentToken.getType() == T_R_ID) 
			{
				Variable* t = checkTable(currentToken.getValue(), Variable::REG_VAR);
				if (t != NULL) {
					printSyntaxError(currentToken);
					errorFound = true;
					return;
				}
				t = new Variable(Variable::REG_VAR, currentToken.getValue(), g_posReg);
				eat(T_R_ID);

				g_positions.push_back(t);

				g_posReg++;
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else if (currentToken.getType() == T_FUNC) 
		{
			eat(T_FUNC);

			if (currentToken.getType() == T_ID) 
			{
				Variable* t = checkTable(currentToken.getValue(), Variable::NO_TYPE);
				if (t != NULL) {
					printSyntaxError(currentToken);
					errorFound = true;
					return;
				}
				t = new Variable(Variable::NO_TYPE, currentToken.getValue(), g_posInstruction);
				eat(T_ID);

				g_positions.push_back(t); 
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else if (currentToken.getType() == T_ID) 
		{
			Variable* t = checkTable(currentToken.getValue(), Variable::NO_TYPE);
			if (t != NULL) {
				printSyntaxError(currentToken);
				errorFound = true;
				return;
			}
			t = new Variable(Variable::NO_TYPE, currentToken.getValue(), g_posInstruction);
			eat(T_ID);

			if (currentToken.getType() == T_COL) 
			{
				eat(T_COL);

				g_positions.push_back(t);;

				E();
			}
			else {
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else 
		{
			E();
		}
	}
	else 
	{
		return;
	}
}

void SyntaxAnalysis::L() 
{
	if (!errorFound) 
	{
		if (currentToken.getType() == T_END_OF_FILE) 
		{
			eat(T_END_OF_FILE);
		}
		else 
		{
			Q();
		}
	}
	else 
	{
		return;
	}
}

void SyntaxAnalysis::E() 
{
	if (!errorFound) 
	{
		Variable* temp;
		Instruction* instr;
		switch (currentToken.getType()) 
		{
		case T_ADD:
			eat(T_ADD);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
					if ((currentToken.getType() == T_R_ID) && (temp != NULL)) 
					{
						Variable* src1 = temp;
						eat(T_R_ID);

						if (currentToken.getType() == T_COMMA) 
						{
							eat(T_COMMA);

							temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
							if (currentToken.getType() == T_R_ID && (temp != NULL)) 
							{
								Variable* src2 = temp;
								eat(T_R_ID);

								Variables* source = new Variables();
								Variables* destination = new Variables();

								source->push_back(src1);
								source->push_back(src2);
								destination->push_back(dst);

								instr = new Instruction(g_posInstruction, I_ADD, *destination, *source);
								g_instructions.push_back(instr);
								g_posInstruction++;

							}
							else 
							{
								printSyntaxError(currentToken);
								errorFound = true;
							}
						}
						else 
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_ADDI:
			eat(T_ADDI);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
					if (currentToken.getType() == T_R_ID && (temp != NULL)) 
					{
						Variable* src1 = temp;
						eat(T_R_ID);

						if (currentToken.getType() == T_COMMA) 
						{
							eat(T_COMMA);

							if (currentToken.getType() == T_NUM) 
							{
								Variable* src2 = new Variable(Variable::NO_TYPE, currentToken.getValue(), g_posInstruction);
								eat(T_NUM);

								Variables* source = new Variables();
								Variables* destination = new Variables();

								source->push_back(src1);
								source->push_back(src2);
								destination->push_back(dst);

								instr = new Instruction(g_posInstruction, I_ADDI, *destination, *source);
								g_instructions.push_back(instr);
								g_posInstruction++;

							}
							else 
							{
								printSyntaxError(currentToken);
								errorFound = true;
							}
						}
						else 
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_SUB:
			eat(T_SUB);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
					if (currentToken.getType() == T_R_ID && (temp != NULL)) 
					{
						Variable* src1 = temp;
						eat(T_R_ID);

						if (currentToken.getType() == T_COMMA) 
						{
							eat(T_COMMA);

							temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
							if (currentToken.getType() == T_R_ID && (temp != NULL)) 
							{
								Variable* src2 = temp; 
								eat(T_R_ID);

								Variables* source = new Variables();
								Variables* destination = new Variables();

								source->push_back(src1);
								source->push_back(src2);
								destination->push_back(dst);

								instr = new Instruction(g_posInstruction, I_SUB, *destination, *source);
								g_instructions.push_back(instr);
								g_posInstruction++;

							}
							else 
							{
								printSyntaxError(currentToken);
								errorFound = true;
							}
						}
						else 
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_DIV:
			eat(T_DIV);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
					if (currentToken.getType() == T_R_ID && (temp != NULL)) 
					{
						Variable* src1 = temp;
						eat(T_R_ID);

						Variables* source = new Variables();
						Variables* destination = new Variables();

						source->push_back(src1);
						destination->push_back(dst);

						instr = new Instruction(g_posInstruction, I_DIV, *destination, *source);
						g_instructions.push_back(instr);
						g_posInstruction++;

					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_MULT:
			eat(T_MULT);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
					if (currentToken.getType() == T_R_ID && (temp != NULL)) 
					{
						Variable* src1 = temp;
						eat(T_R_ID);

						Variables* source = new Variables();
						Variables* destination = new Variables();

						source->push_back(src1);
						destination->push_back(dst);

						instr = new Instruction(g_posInstruction, I_MULT, *destination, *source);
						g_instructions.push_back(instr);
						g_posInstruction++;

					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_LA:
			eat(T_LA);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					temp = checkTable(currentToken.getValue(), Variable::MEM_VAR);
					if ((currentToken.getType() == T_M_ID) && (temp != NULL)) 
					{
						Variable* src1 = temp;
						eat(T_M_ID);

						Variables* source = new Variables();
						Variables* destination = new Variables();

						source->push_back(src1);
						destination->push_back(dst);

						instr = new Instruction(g_posInstruction, I_LA, *destination, *source);
						g_instructions.push_back(instr);
						g_posInstruction++;

					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_LW:
			eat(T_LW);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					if (currentToken.getType() == T_NUM) 
					{
						Variable* src1 = new Variable(Variable::NO_TYPE, currentToken.getValue(), g_posInstruction);
						eat(T_NUM);

						if (currentToken.getType() == T_L_PARENT) 
						{
							eat(T_L_PARENT);

							temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
							if (currentToken.getType() == T_R_ID && (temp != NULL)) 
							{
								Variable* src2 = temp;
								eat(T_R_ID);

								if (currentToken.getType() == T_R_PARENT) 
								{
									eat(T_R_PARENT);

									Variables* source = new Variables();
									Variables* destination = new Variables();

									source->push_back(src1);
									source->push_back(src2);
									destination->push_back(dst);

									instr = new Instruction(g_posInstruction, I_LW, *destination, *source);
									g_instructions.push_back(instr);
									g_posInstruction++;

								}
								else 
								{
									printSyntaxError(currentToken);
									errorFound = true;
								}
							}
							else 
							{
								printSyntaxError(currentToken);
								errorFound = true;
							}
						}
						else 
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_LI:
			eat(T_LI);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					if (currentToken.getType() == T_NUM) 
					{
						Variable* src1 = new Variable(Variable::NO_TYPE, currentToken.getValue(), g_posInstruction);
						eat(T_NUM);

						Variables* source = new Variables();
						Variables* destination = new Variables();

						source->push_back(src1);
						destination->push_back(dst);

						instr = new Instruction(g_posInstruction, I_LI, *destination, *source);
						g_instructions.push_back(instr);
						g_posInstruction++;

					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_SW:
			eat(T_SW);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* src1 = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					if (currentToken.getType() == T_NUM) 
					{
						Variable* src2 = new Variable(Variable::NO_TYPE, currentToken.getValue(), g_posInstruction);
						eat(T_NUM);

						if (currentToken.getType() == T_L_PARENT) 
						{
							eat(T_L_PARENT);

							temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
							if (currentToken.getType() == T_R_ID && (temp != NULL)) 
							{
								Variable* src3 = temp;
								eat(T_R_ID);

								if (currentToken.getType() == T_R_PARENT) 
								{
									eat(T_R_PARENT);

									Variables* source = new Variables();
									Variables* destination = new Variables();

									source->push_back(src1);
									source->push_back(src2);
									source->push_back(src3);

									instr = new Instruction(g_posInstruction, I_SW, *destination, *source);
									g_instructions.push_back(instr);
									g_posInstruction++;

								}
								else 
								{
									printSyntaxError(currentToken);
									errorFound = true;
								}
							}
							else 
							{
								printSyntaxError(currentToken);
								errorFound = true;
							}
						}
						else 
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_B:
			eat(T_B);

			temp = checkTable(currentToken.getValue(), Variable::NO_TYPE);
			if (currentToken.getType() == T_ID && (temp != NULL)) 
			{
				Variable* dst = temp;
				eat(T_ID);

				Variables* source = new Variables();
				Variables* destination = new Variables();

				destination->push_back(dst);

				instr = new Instruction(g_posInstruction, I_B, *destination, *source);
				g_instructions.push_back(instr);
				g_posInstruction++;
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_BEQ:
			eat(T_BEQ);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* src1 = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
					if (currentToken.getType() == T_R_ID && (temp != NULL)) 
					{
						Variable* src2 = temp;
						eat(T_R_ID);

						if (currentToken.getType() == T_COMMA) 
						{
							eat(T_COMMA);

							temp = checkTable(currentToken.getValue(), Variable::NO_TYPE);
							if ((currentToken.getType() == T_ID) && (temp != NULL)) 
							{
								Variable* dst = temp;
								eat(T_ID);

								Variables* source = new Variables();
								Variables* destination = new Variables();

								source->push_back(src1);
								source->push_back(src2);
								destination->push_back(dst);

								instr = new Instruction(g_posInstruction, I_BEQ, *destination, *source);
								g_instructions.push_back(instr);
								g_posInstruction++;
							}
							else 
							{
								printSyntaxError(currentToken);
								errorFound = true;
							}
						}
						else 
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
			break;

		case T_BLTZ:
			eat(T_BLTZ);

			temp = checkTable(currentToken.getValue(), Variable::REG_VAR);
			if (currentToken.getType() == T_R_ID && (temp != NULL)) 
			{
				Variable* src1 = temp;
				eat(T_R_ID);

				if (currentToken.getType() == T_COMMA) 
				{
					eat(T_COMMA);

					temp = checkTable(currentToken.getValue(), Variable::NO_TYPE);
					if ((currentToken.getType() == T_ID) && (temp != NULL)) 
					{
						Variable* dst = temp;
						eat(T_ID);

						Variables* source = new Variables();
						Variables* destination = new Variables();

						source->push_back(src1);
						destination->push_back(dst);

						instr = new Instruction(g_posInstruction, I_BLTZ, *destination, *source);

						g_instructions.push_back(instr);
						g_posInstruction++;

					}
					else 
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else 
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else 
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}

			break;

		case T_NOP:
			eat(T_NOP);

			instr = new Instruction();
			instr->m_position = g_posInstruction;
			instr->m_type = I_NOP;
			g_instructions.push_back(instr);
			break;

		default:
			printSyntaxError(currentToken);
			errorFound = true;
			break;
		}
	}
	else 
	{
		return;
	}
}