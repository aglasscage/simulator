#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

const int ARG_PTRS = 3;
const int MAX_REGISTERS = 32;

class Label
{
public:
	Label()
		: line_number_(0), label_("")
	{}
	Label(const std::string label, const int line_number)
		: label_(label), line_number_(line_number)
	{}
	std::string get_label() const
	{
		return label_;
	}
	int get_line_number() const
	{
		return line_number_;
	}
	bool operator==(const std::string label)
	{
		if (label_ == label) return true;
		else return false;
	}
	bool operator<=(const int & line_number)
	{
		if (line_number_ <= line_number) return true;
		else return false;
	}
private:
	std::string label_;
	int line_number_;
};

std::ostream & operator<<(std::ostream & cout, const Label & label)
{
    cout << label.get_line_number() << ": " << label.get_label() << std::endl;
    return cout;
}

struct Registers
{
	Registers()
		: line_number_(0)
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
	// function to set the register pointers.
	void set_reg(const std::string & argument)
	{
		int i;
		for (i = 0; i < ARG_PTRS; i++)
		{
			if (arg[i] == NULL) break;
		}
		if (argument == "v0") arg[i] = &reg[2];
		else if (argument == "v1") arg[i] = &reg[3];
		else if (argument == "a0") arg[i] = &reg[4];
		else if (argument == "a1") arg[i] = &reg[5];
		else if (argument == "a2") arg[i] = &reg[6];
		else if (argument == "a3") arg[i] = &reg[7];
		else if (argument == "t0") arg[i] = &reg[8];
		else if (argument == "t1") arg[i] = &reg[9];
		else if (argument == "t2") arg[i] = &reg[10];
		else if (argument == "t3") arg[i] = &reg[11];
		else if (argument == "t4") arg[i] = &reg[12];
		else if (argument == "t5") arg[i] = &reg[13];
		else if (argument == "t6") arg[i] = &reg[14];
		else if (argument == "t7") arg[i] = &reg[15];
		else if (argument == "s0") arg[i] = &reg[16];
		else if (argument == "s1") arg[i] = &reg[17];
		else if (argument == "s2") arg[i] = &reg[18];
		else if (argument == "s3") arg[i] = &reg[19];
		else if (argument == "s4") arg[i] = &reg[20];
		else if (argument == "s5") arg[i] = &reg[21];
		else if (argument == "s6") arg[i] = &reg[22];
		else if (argument == "s7") arg[i] = &reg[23];
		else if (argument == "t8") arg[i] = &reg[24];
		else if (argument == "t9") arg[i] = &reg[25];
		else if (argument == "k0") arg[i] = &reg[26];
		else if (argument == "k1") arg[i] = &reg[27];
		else
		{
			std::cout << "Error on line " << get_line_number()
					  << ". Invalid register argument."
					  << '\n';
		}
	}
	// function to reset the register pointers to NULL
	void release_reg()
	{
		for (int i = 0; i < ARG_PTRS; i++)
		{
			if (arg[i] != NULL)
			{
				arg[i] = NULL;
			}
		}
	}
	void compute(const std::string & instruction, 
				 std::vector < int > line_number,
				 std::vector < Label > label, int & index)
	{
		set_line_number(line_number[index]);
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
			if (instruction[i] == 'j')
			{
				i++;
				break;
			}
			if (instruction[i] == ':')
			{
				type = "";
			}
		}
		if (type == "li") load_immediate(instruction, i);
		else if (type == "syscall") syscall(instruction);
		else if (type == "add") add(instruction, i);
		else if (type == "addi") addi(instruction, i);
		else if (type == "sub") sub(instruction, i);
		else if (type == "mul") mul(instruction, i);
		else if (type == "div") div(instruction, i);
		else if (type == "rem") rem(instruction, i);
		else if (type == "j") jump(instruction, label, line_number, i, index);
		else if (type == "") return;
	}
	void jump(const std::string & instruction, std::vector < Label > label,
			  std::vector < int > line_number, int & i,
			  int & index)
	{
		std::string jump_to = "";
		for (i; i < instruction.size(); i++)
		{
			jump_to.push_back(instruction[i]);
		}

		int j = 0;

		for (j; j < label.size(); j++)
		{
			if (label[j] == jump_to) break;
		}

		for (int k = 0; k < line_number.size(); k++)
		{
			if (label[j] <= line_number[k])
			{
				index = k;
				break;
			}
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
		if (arg_valid(1)) *arg[0] = string_to_integer(value);
		release_reg();
	}
	void add(const std::string & instruction, int & i)
	{
		std::string reg_key0 = "";
		std::string reg_key1 = "";
		std::string reg_key2 = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key0.push_back('e');
				//i++;
				break;
			}
			reg_key0.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key1.push_back('e');
				//i++;
				break;
			}
			reg_key1.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			reg_key2.push_back(instruction[i]);
		}
		set_reg(reg_key0);
		set_reg(reg_key1);
		set_reg(reg_key2);
		if (arg_valid(3))
		{
			*arg[0] = *arg[1] + *arg[2];
		}
		release_reg();
	}
	void addi(const std::string & instruction, int & i)
	{
		std::string reg_key0 = "";
		std::string reg_key1 = "";
		std::string value = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key0.push_back('e');
				break;
			}
			reg_key0.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				break;
			}
			reg_key1.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (int(instruction[i]) < 48 || int(instruction[i]) > 57)
			{
				std::cerr << "Error" << std::endl;
			}
			value.push_back(instruction[i]);
		}
		set_reg(reg_key0);
		set_reg(reg_key1);
		// computation
		if (arg_valid(2)) *arg[0] = *arg[1] + string_to_integer(value);
		release_reg();
	}
	void sub(const std::string & instruction, int & i)
	{
		std::string reg_key0 = "";
		std::string reg_key1 = "";
		std::string reg_key2 = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key0.push_back('e');
				//i++;
				break;
			}
			reg_key0.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key1.push_back('e');
				//i++;
				break;
			}
			reg_key1.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			reg_key2.push_back(instruction[i]);
		}
		set_reg(reg_key0);
		set_reg(reg_key1);
		set_reg(reg_key2);
		if (arg_valid(3))
		{
			*arg[0] = *arg[1] - *arg[2];
		}
		release_reg();
	}
	void mul(const std::string & instruction, int & i)
	{
		std::string reg_key0 = "";
		std::string reg_key1 = "";
		std::string reg_key2 = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key0.push_back('e');
				//i++;
				break;
			}
			reg_key0.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key1.push_back('e');
				//i++;
				break;
			}
			reg_key1.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			reg_key2.push_back(instruction[i]);
		}
		set_reg(reg_key0);
		set_reg(reg_key1);
		set_reg(reg_key2);
		if (arg_valid(3))
		{
			*arg[0] = *arg[1] * *arg[2];
		}
		release_reg();
	}
	void div(const std::string & instruction, int & i)
	{
		std::string reg_key0 = "";
		std::string reg_key1 = "";
		std::string reg_key2 = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key0.push_back('e');
				//i++;
				break;
			}
			reg_key0.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key1.push_back('e');
				//i++;
				break;
			}
			reg_key1.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			reg_key2.push_back(instruction[i]);
		}
		set_reg(reg_key0);
		set_reg(reg_key1);
		set_reg(reg_key2);
		if (arg_valid(3))
		{
			*arg[0] = *arg[1] / *arg[2];
		}
		release_reg();
	}
	void rem(const std::string & instruction, int & i)
	{
		std::string reg_key0 = "";
		std::string reg_key1 = "";
		std::string reg_key2 = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key0.push_back('e');
				//i++;
				break;
			}
			reg_key0.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				if (instruction[i++] != '$') reg_key1.push_back('e');
				//i++;
				break;
			}
			reg_key1.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			reg_key2.push_back(instruction[i]);
		}
		set_reg(reg_key0);
		set_reg(reg_key1);
		set_reg(reg_key2);
		if (arg_valid(3))
		{
			*arg[0] = *arg[1] % *arg[2];
		}
		release_reg();
	}
	void syscall(const std::string & instruction)
	{
		if (reg[2] == 1) std::cout << reg[4];
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
	bool arg_valid(const int & num) const
	{
		int i = 0;
		for (i; i < ARG_PTRS; i++)
		{
			if (arg[i] == NULL) break;
		}
		if (i == num) return true;
		else return false;
	}
	void set_line_number(const int & line_number)
	{
		line_number_ = line_number;
	}
	int get_line_number() const { return line_number_; }

	int line_number_;
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
	cout << "\n\n"
    	 << "$0: " << registers.reg[0] << '\n'
    	 << "$1: " << registers.reg[1] << '\n'
    	 << "$v0: " << registers.reg[2] << '\t'
    	 << "$v1: " << registers.reg[3] << '\n'
    	 << "$a0: " << registers.reg[4] << '\t'
    	 << "$a1: " << registers.reg[5] << '\t'
    	 << "$a2: " << registers.reg[6] << '\t'
    	 << "$a3: " << registers.reg[7] << '\n'
    	 << "$t0: " << registers.reg[8] << '\t'
    	 << "$t1: " << registers.reg[9] << '\t'
    	 << "$t2: " << registers.reg[10] << '\t'
    	 << "$t3: " << registers.reg[11] << '\t'
    	 << "$t4: " << registers.reg[12] << '\t'
    	 << "$t5: " << registers.reg[13] << '\t'
    	 << "$t6: " << registers.reg[14] << '\t'
    	 << "$t7: " << registers.reg[15] << '\n'
    	 << "$s0: " << registers.reg[16] << '\t'
    	 << "$s1: " << registers.reg[17] << '\t'
    	 << "$s2: " << registers.reg[18] << '\t'
    	 << "$s3: " << registers.reg[19] << '\t'
    	 << "$s4: " << registers.reg[20] << '\t'
    	 << "$s5: " << registers.reg[21] << '\t'
    	 << "$s6: " << registers.reg[22] << '\t'
    	 << "$s7: " << registers.reg[23] << '\n'
    	 << "$t8: " << registers.reg[24] << '\t'
    	 << "$t9: " << registers.reg[25] << '\n'
    	 << "$k0: " << registers.reg[26] << '\t'
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
		std::string temp_label;
		for (int i = 0; i < line.size(); i++)
		{
			// ignore spaces, newlines, and tabs
			if (line[i] != ' ' && line[i] != '\n' && line[i] && line[i] != '\t') 
			{
				if (line[i] == '#') return; // ignore comments
				temp.push_back(line[i]);
				if (line[i] == ':')
				{
					for (int j = 0; j < i; j++)
					{
						if (line[j] != ' ' && line[j] != '\n' 
							&& line[j] && line[j] != '\t')
						{
							temp_label.push_back(line[j]);
						}
					}
				}
			}	
		}
		if (temp_label != "") 
		{
			Label t(temp_label, line_num);
			label.push_back(t);
		}
		if (temp != "") instruction.push_back(temp);
		line_number.push_back(line_num);
	}
	void run()
	{
		int i = 0;
		int start = 0;
		for (i; i < label.size(); i++)
		{
			if (label[i] == "main")
			{
				break;
			}
		}
		if (i == label.size())
		{
			std::cout << "Error: no 'main' label." << std::endl;
			return;
		}

		for (int j = 0; j < line_number.size(); j++)
		{
			if (label[i] <= line_number[j])
			{
				i = j;
				break;
			}
		}

		for (i; i < instruction.size(); i++)
		{
			registers.compute(instruction[i], line_number, label, i);
		}
		std::cout << registers << std::endl;
		print_labels();
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
	void print_labels()
	{
		std::cout << '\n';
		for (int i = 0; i < label.size(); i++)
		{
			std::cout << label[i];
		}
	}
private:
	std::vector < std::string > code;
	std::vector < std::string > instruction;
	std::vector < int > line_number;
	std::vector < Label > label;
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