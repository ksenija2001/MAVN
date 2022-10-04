/* Autor: Ksenija Barakovic Datum: 07.06.2022. */

#ifndef __SYNTAX_ANALYSIS__
#define __SYNTAX_ANALYSIS__

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"

#include <iomanip>

class SyntaxAnalysis 
{
private:

	/**
	 * Reference to lexical analysis
	 */
	LexicalAnalysis& lexicalAnalysis;

	/**
	 * Iterator to the token list
	 */
	TokenList::iterator tokenIterator;
	/**
	 * Current token that is being analyzed
	 */
	Token currentToken;

	/**
	 * Nonterminal Q
	 */
	void Q();

	/**
	 * Nonterminal S
	 */
	void S();

	/**
	 * Nonterminal L
	 */
	void L();

	/**
	 * Nonterminal E
	 */
	void E();

	/**
	 * Syntax error indicator
	 */
	bool errorFound;


public:

	/**
	 * Constructor
	 */
	SyntaxAnalysis(LexicalAnalysis& lex);

	/**
	 * Returns a complete instructions list ready for liveness analysis
	*/
	Instructions getInstructions();

	/**
	 * Returns table of used variables with references to their positions, types and values
	*/
	std::vector<Variable*> getSymbolTable();

	/**
	 * Performs syntax analysis
	 */
	bool Do();

	/**
	 * Prints the token that caused the syntax error
	 */
	void printSyntaxError(Token token);

	/**
	 * Eats the current token if its type is "t"
	 * else reports syntax error
	 */
	void eat(TokenType t);

	/**
	 * Returns the next token from the token list
	 */
	Token getNextToken();

	/**
	 * Prints token type and value
	 */
	void printTokenInfo(Token token);

	/**
	* Checks if the register, memory operand or id has been declared beforehand and returns it's position
	*/
	Variable* checkTable(std::string name, Variable::VariableType type);
};

#endif
