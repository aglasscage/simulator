#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>

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
	bool operator!=(const std::string label)
	{
		if (label_ == label) return false;
		else return true;
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

class Reg
{
public:
	Reg & operator +=(const Reg & reg) { i_ += reg.get_value(); }
	Reg & operator +=(const int & i) { i_ += i; }
	Reg & operator -=(const Reg & reg) { i_ -= reg.get_value(); }
	Reg & operator *=(const Reg & reg) { i_ *= reg.get_value(); }
	Reg & operator /=(const Reg & reg)
	{
		if (reg.get_value() != 0)
		{
			i_ /= reg.get_value();
		}
		else std::cout << "ERROR: Register division by 0" << std::endl;
	}
	Reg & operator=(const int & i) 
	{ 
		i_ = i;
		d_ = false;
	}
	Reg & operator=(const Reg & reg) 
	{ 
		i_ = reg.get_value();
		d_ = reg.get_segment();
	}
	Reg & operator=(const bool & d) { d_ = d; }
	int operator=(const Reg & reg) const { return i_ + reg.get_value(); } const
	int operator+(const Reg & reg) { return i_ + reg.get_value(); }	
	int operator+(const int & i) { return i_ + i; }	
	int operator-(const Reg & reg) { return i_ - reg.get_value(); }
	int operator*(const Reg & reg) { return i_ * reg.get_value(); }
	int operator/(const Reg & reg)
	{
		if (reg.get_value() != 0)
		{
			return i_ / reg.get_value();
		}
		else std::cout << "ERROR: Register division by 0" << std::endl;
	}
	int operator%(const Reg & reg)
	{
		if (reg.get_value() != 0)
		{
			return i_ % reg.get_value();
		}
		else std::cout << "ERROR: Register division by 0" << std::endl;
	}
	bool operator==(const int & i)
	{
		if (i_ == i) return true;
		else return false;
	}
	bool operator==(const bool & d)
	{
		if (d_ == d) return true;
		else return false;
	}
	int get_value() const
	{
		return i_;
	}
	bool get_segment() const
	{
		return d_;
	}
private:
	int i_;
	bool d_;
};

std::ostream & operator<<(std::ostream & cout, const Reg & reg)
{
    cout << reg.get_value();
    return cout;
}

class Data
{
public:
	Data(int i=-9999, char c=' ', std::string s="", std::string label="")
		: i_(i), c_(c), s_(s), label_(label)
	{}
	Data & operator=(const Data & data)
	{
		if (i_ == -9999 && s_ == "" && data.getChar() != ' ') 
		{
			c_ = data.getChar();
		}
		if (i_ == -9999 && c_ == ' ' && data.getString() != "")
		{
			s_ = data.getString();
		}
		else
		{
			std::cout << "ERROR: You are trying to store an invalid type.\n"
					  << "ERROR: You must store values in the data segment according to their type"
					  << std::endl;
		}
	}
	void setData(const std::string & type, const std::string & data)
	{
		if (type == "asciiz")
		{
			setString(data);
		}
	}
	void setInt(const int & i)
	{
		i_ = i;
	}
	void setChar(const int & c)
	{
		c_ = c;
	}
	void setString(const std::string & s)
	{
		s_ = s;
	}
	void setLabel(const std::string & label)
	{
		label_ = label;
	}
	int getInt() const
	{
		return i_;
	}
	char getChar() const
	{
		return c_;
	}
	std::string getString() const
	{
		return s_;
	}
	std::string getLabel() const
	{
		return label_;
	}
private:
	int i_;
	char c_;
	std::string s_;
	std::string label_;
};

std::ostream & operator<<(std::ostream & cout, const Data & data)
{
	if (data.getInt() != -9999) cout << data.getInt();
	else if (data.getChar() != ' ') cout << data.getChar();
	else if (data.getString() != "") cout << data.getString();
	/*
    cout << "Label:" << data.getLabel() << ", "
    	 << "i:" << data.getInt() << ", "
    	 << "c:" << data.getChar() << ", "
    	 << "s:" << data.getString() << '\n';
    */
    
    return cout;
}

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
				 std::vector < Data > & data,
				 std::vector < int > line_number,
				 std::vector < Label > label, int & index)
	{
		//std::cout << "processing: " << instruction << std::endl;
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
			if (instruction[0] == 'j')
			{
				std::string temp = instruction.substr(0 , 3); 
				if (temp == "jal")
				{
					type = temp;
					i = 3;
					break;
				}
				i++;
				break;
			}
			if (instruction[i] == ':')
			{
				type = "";
			}
		}
		if (type == "li") load_immediate(instruction, i);
		else if (type == "la") load_address(instruction, data, i);
		else if (type == "lw") load_word(instruction, i);
		else if (type == "sw") store_word(instruction, data, i);
		else if (type == "syscall") syscall(instruction, data);
		else if (type == "add") add(instruction, i);
		else if (type == "addi") addi(instruction, i);
		else if (type == "sub") sub(instruction, i);
		else if (type == "mul") mul(instruction, i);
		else if (type == "div") div(instruction, i);
		else if (type == "rem") rem(instruction, i);
		else if (type == "jal") jal(instruction, label, line_number, i, index);
		else if (type == "j") jump(instruction, label, line_number, i, index);
		else if (type == "") return;
	}
	void jal(const std::string & instruction, std::vector < Label > label,
			  std::vector < int > line_number, int & i,
			  int & index)
	{
		reg[31] = index;
		jump(instruction, label, line_number, i, index);
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

		if (instruction.find("$ra") != std::string::npos) 
		{
			index = reg[31].get_value();
			return;
		}

		int j = 0;

		for (j; j < label.size(); j++)
		{
			if (label[j] == jump_to) break;
		}

		if (j == label.size())
		{
			std::cout << "\nError: Cannot find label " << jump_to << '.' << std::endl;
			return;
		}

		for (int k = 0; k < line_number.size(); k++)
		{
			if (label[j] <= line_number[k])
			{
				// NOTE : THIS COULD CAUSE PROBLEMS DUE TO OUT OF RANGE INDEX
				index = k - 1;
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
	void load_address(const std::string & instruction, const std::vector < Data > & data, int & i)
	{
		std::string reg_key = "";
		std::string label = "";
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
			label.push_back(instruction[i]);
		}
		set_reg(reg_key);
		if (arg_valid(1))
		{
			for (int j = 0; j < data.size(); j++)
			{
				if (data[j].getLabel() == label)
				{
					*arg[0] = j;
					*arg[0] = true;
					break;
				}
			}
		}
		release_reg();
	}
	void load_word(const std::string & instruction, int & i)
	{
		std::string reg_key0 = "";
		std::string value = "";
		std::string reg_key1 = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				break;
			}
			reg_key0.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == '(')
			{
				i++;
				break;
			}
			value.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == '$') continue;
			if (instruction[i] == ')')
			{
				break;
			}
			reg_key1.push_back(instruction[i]);
		}
		set_reg(reg_key0);
		set_reg(reg_key1);
		if (arg_valid(2))
		{
			*arg[0] = *arg[1];
			*arg[0] += string_to_integer(value) / 4;
		}
		release_reg();
		//lw 	$t3, 0($t5)
		return;
	}
	void store_word(const std::string & instruction, std::vector < Data > & data, int & i)
	{
		std::string reg_key0 = "";
		std::string value = "";
		std::string reg_key1 = "";
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == ',')
			{
				i++;
				break;
			}
			reg_key0.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == '(')
			{
				i++;
				break;
			}
			value.push_back(instruction[i]);
		}
		for (i; i < instruction.size(); i++)
		{
			if (instruction[i] == '$') continue;
			if (instruction[i] == ')')
			{
				break;
			}
			reg_key1.push_back(instruction[i]);
		}
		set_reg(reg_key0);
		set_reg(reg_key1);
		if (arg_valid(2))
		{
			int index = arg[1]->get_value();
			index += string_to_integer(value) / 4;
			if (*arg[0] == true)
			{
				data[index] = data[arg[0]->get_value()];
			}
			else
			{
				data[index].setInt(arg[0]->get_value());
			}
		}
		release_reg();
		//lw 	$t3, 0($t5)
		return;
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
	void syscall(const std::string & instruction, const std::vector < Data > & data)
	{
		if (reg[2] == 1)
		{
			// reg 4
			if (reg[4] == false)
			{
				std::cout << reg[4].get_value();
			}
			else
			{
				std::cout << data[reg[4].get_value()];
			}
		}
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
	Reg * arg[ARG_PTRS];
	Reg reg[MAX_REGISTERS];
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
	Interpreter()
	{
		data_flag = false;
	}
	void add_instruction(const std::string & line, const int & line_num)
	{
		code.push_back(line);
		if (!data_flag)
		{	
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
						temp = "";
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
			if (temp != "") 
			{
				if (temp == ".data") data_flag = true;
				instruction.push_back(temp);
				line_number.push_back(line_num);
			}
		}
		else
		{
			std::string temp_type;
			std::string temp_label;
			std::string temp_data;
			int i = 0;
			for (i; i < line.size(); i++)
			{
				if (line[i] == ':')
				{
					i++;
					break;
				}
				temp_label.push_back(line[i]);
			}
			for (i; i < line.size(); i++)
			{
				if (line[i] == '.')
				{
					i++;
					break;
				}
			}
			for (i; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					i++;
					break;
				}
				temp_type.push_back(line[i]);
			}
			for (i; i < line.size(); i++)
			{
				if (line[i] == '"') continue;
				temp_data.push_back(line[i]);
			}
			if (temp_type == "space")
			{
				int d = string_to_integer(temp_data);
				d /= 4;
				Data temp(0);
				for (int i = 0; i < d; i++)
				{
					if (i == 0)
					{
						Data temp1(-9999);
						temp1.setLabel(temp_label);
						data.push_back(temp1);
					}
					else
					{
						data.push_back(temp);
					}
				}
			}
			else
			{
				Data temp;
				temp.setData(temp_type, temp_data);
				temp.setLabel(temp_label);
				data.push_back(temp);
			}
		}
	}
	void run()
	{
		int i = 0;
		int j = 0;
		int start = 0;
		for (j = 0; j < instruction.size(); j++)
		{
			if (instruction[j] == ".text")
			{
				break;
			}
		}
		if (j == instruction.size())
		{
			std::cout << "Error: no .text segment found." << std::endl;
			return;
		}
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
			registers.compute(instruction[i], data, line_number, label, i);
		}
		std::cout << registers << std::endl;
		//print_labels();
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
	void print_data()
	{
		std::cout << "---Data Segment---\n";
		for (int i = 0; i < data.size(); i++)
		{
			std::cout << i << ": " << data[i];
		}
		std::cout << "---End Data Segment---" << std::endl;
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
	bool data_flag;
	std::vector < std::string > code;
	std::vector < std::string > instruction;
	std::vector < int > line_number;
	std::vector < Label > label;
	std::vector < Data > data;
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