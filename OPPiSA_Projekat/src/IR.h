/* Autor: Ksenija Barakovic Datum: 06.06.2022. */

#ifndef __IR__
#define __IR__

#include "Types.h"

/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(VariableType type, std::string name, int pos) : m_type(type), m_name(name), m_position(pos), m_assignment(no_assign) {}

	int getPosition() {
		return m_position;
	}

	std::string getName() {
		return m_name;
	}

	VariableType getType() {
		return m_type;
	}

	void setAssignment(Regs color) {
		m_assignment = color;
	}

	int getAssignment() {
		return (m_assignment-1);
	}

	void setValue(std::string s) {
		m_value = s;
	}

	std::string getValue() {
		return m_value;
	}

private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
	std::string m_value;
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;

/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}

	int m_position;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};

/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

#endif
