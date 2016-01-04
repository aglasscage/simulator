#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>
#include "registers.h"
#include "data.h"
#include "registers.h"

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
					if (line[i] == '#') break; // ignore comments
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
		//print_data();
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
			//data_view();
		}
		std::cout << registers << std::endl;
		//print_labels();
	}
	int string_to_integer(const std::string & string)
	{
		bool negative = false;
		int result = 0;
		int i = 0;
		if (int(string[i] == 45))
		{
			i++;
			negative = true;
		}
		for (i; i < string.size(); i++)
		{
			result += int(string[i] - 48) * pow(10, string.size() - i - 1);
		}
		if (negative) result = result * -1;
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
	void data_view()
	{
		std::cout << "=====Data Segment=====";
		for (int i = 0; i < data.size(); i++)
		{
			std::cout << '\n';
			data[i].view();
		}
		std::cout << "\n===End Data Segment===" << std::endl;
	}
	void reinitialize()
	{
		data_flag = false;
		code.clear();
		instruction.clear();
		line_number.clear();
		label.clear();
		data.clear();
		registers.reinitialize();
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