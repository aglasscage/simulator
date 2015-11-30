#include <iostream>
#include <string>
#include <fstream>
#include "fileutil.h"
#include "interpreter.h"

const std::string NAME = "{Simulator Name}";

int main()
{
	Interpreter interpreter;

	std::cout << NAME << ": Please enter a filename. (Example: main.s)\n"
			  << NAME << ": ";
	//fileutil.open();
	//interpreter.interpret(fileutil.getline());
	std::string input;
	std::getline(std::cin, input);
	std::ifstream infile(input.c_str(), std::ios::in);
	std::string file(input);
	std::string line;
	int line_num = 1;
	if (infile.is_open())
	{
		while (getline(infile, line))
		{
			interpreter.add_instruction(line, line_num);
			line_num++;
		}
		infile.close();
	}

	std::cout << interpreter << std::endl;

	interpreter.run();

	return 0;
}