#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

const int ARG_PTRS = 3;
const int MAX_REGISTERS = 32;

struct Registers
{
	Registers()
	{
		for (int i = 0; i < MAX_REGISTERS; i++)
		{
			reg[i] = 0;
		}
		for (int i = 0; i < ARG_PTRS; i++)
		{
			arg[i] = NULL;
		}
	}
	void set_reg(const std::string & argument)
	{
		int i;
		for (i = 0; i < ARG_PTRS; i++)
		{
			if (arg[i] == NULL) break;
		}
		if (argument == "v0") arg[i] = &reg[2];
	}
	void compute(const std::string & instruction)
	{
		std::string type = "";
		int i = 0;
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == '$')
			{
				i++;
				break;
			}
			type.push_back(instruction[i]);
		}
		if (type == "li")
		{
			load_immediate(instruction, i);
		}
	}
	void load_immediate(const std::string & instruction, int & i)
	{
		std::string reg_key = "";
		std::string value = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				break;
			}
			reg_key.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (int(instruction[i]) < 48 || int(instruction[i]) > 57)
			{
				std::cerr << "Error" << std::endl;
			}
			value.push_back(instruction[i]);
		}
		set_reg(reg_key);
		// computation
		*arg[0] = string_to_integer(value);
	}
	int string_to_integer(std::string & string)
	{
		int result = 0;
		for (int i = 0; i < string.size(); i++)
		{
			result += int(string[i] - 48) * pow(10, string.size() - i - 1);
		}
		return result;
	}
	int * arg[ARG_PTRS];
	int reg[MAX_REGISTERS];
	// 0 = $0 / $zero
	// 1 = at (assembler temporary)
	// 2-3 = $v0, $v1
	// 4-7 = $a0 - $a3
	// 8-15 = $t0 - $t7
	// 16-23 = $s0 - $s7
	// 24-25 = $t8 - $t9
	// 26-27 = $k0 - $k1
	// 28 = gp (global pointer)
	// 29 = sp (stack pointer)
	// 30 = $s8
	// 31 = $ra (return address)
};

std::ostream & operator<<(std::ostream & cout, const Registers & registers)
{
    cout << "$0: " << registers.reg[0] << '\n'
    	 << "$1: " << registers.reg[1] << '\n'
    	 << "$v0: " << registers.reg[2] << '\n'
    	 << "$v1: " << registers.reg[3] << '\n'
    	 << "$a0: " << registers.reg[4] << '\n'
    	 << "$a1: " << registers.reg[5] << '\n'
    	 << "$a2: " << registers.reg[6] << '\n'
    	 << "$a3: " << registers.reg[7] << '\n'
    	 << "$t0: " << registers.reg[8] << '\n'
    	 << "$t1: " << registers.reg[9] << '\n'
    	 << "$t2: " << registers.reg[10] << '\n'
    	 << "$t3: " << registers.reg[11] << '\n'
    	 << "$t4: " << registers.reg[12] << '\n'
    	 << "$t5: " << registers.reg[13] << '\n'
    	 << "$t6: " << registers.reg[14] << '\n'
    	 << "$t7: " << registers.reg[15] << '\n'
    	 << "$s0: " << registers.reg[16] << '\n'
    	 << "$s1: " << registers.reg[17] << '\n'
    	 << "$s2: " << registers.reg[18] << '\n'
    	 << "$s3: " << registers.reg[19] << '\n'
    	 << "$s4: " << registers.reg[20] << '\n'
    	 << "$s5: " << registers.reg[21] << '\n'
    	 << "$s6: " << registers.reg[22] << '\n'
    	 << "$s7: " << registers.reg[23] << '\n'
    	 << "$t8: " << registers.reg[24] << '\n'
    	 << "$t9: " << registers.reg[25] << '\n'
    	 << "$k0: " << registers.reg[26] << '\n'
    	 << "$k1: " << registers.reg[27] << '\n'
    	 << "$gp: " << registers.reg[28] << '\n'
    	 << "$sp: " << registers.reg[29] << '\n'
    	 << "$s8: " << registers.reg[30] << '\n'
    	 << "$ra: " << registers.reg[31] << std::endl;
    return cout;
}

class Interpreter
{
public:
	void add_instruction(const std::string & line, const int & line_num)
	{
		code.push_back(line);
		std::string temp;
		for (int i = 0; i < line.size(); i++)
		{
			// ignore spaces, newlines, and tabs
			if (line[i] != ' ' && line[i] != '\n' && line[i] && line[i] != '\t') 
			{
				if (line[i] == '#') return; // ignore comments
				temp.push_back(line[i]);
			}	
		}
		instruction.push_back(temp);
		line_number.push_back(line_num);
	}
	void run()
	{
		for (int i = 0; i < instruction.size(); i++)
		{
			registers.compute(instruction[i]);
		}
		std::cout << registers << std::endl;
	}
	int get_size() const
	{
		return instruction.size();
	}
	std::string get_code(const int & i) const
	{
		return code[i];
	}
	std::string get_instruction(const int & i) const
	{
		return instruction[i];
	}
	int get_line_number(const int & i) const
	{
		return line_number[i];
	}
private:
	std::vector < std::string > code;
	std::vector < std::string > instruction;
	std::vector < int > line_number;
	Registers registers;
};

std::ostream & operator<<(std::ostream & cout, const Interpreter & interpreter)
{
    for (int i = 0; i < interpreter.get_size(); i++)
    {
    	cout << interpreter.get_line_number(i);
    	cout << ": ";
    	cout << interpreter.get_code(i) << std::endl;
    	cout << "Instruction: " << interpreter.get_instruction(i) << std::endl;
    }
    return cout;
}

#endif