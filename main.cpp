#include <iostream>
#include <string>
#include <fstream>
#include "fileutil.h"
#include "interpreter.h"

const std::string NAME = "{Mips Sim}";

int main()
{
	Interpreter interpreter;

	std::string input;
	std::cout << NAME << ": Please enter a command. (\"help\" for a list of commands)\n";
	while (input != "q")
	{
		std::cout << NAME << ": ";
		std::getline(std::cin, input);
		if (input == "help")
		{
			std::cout << "\n================Mips Sim================\n"
					  << "\"reinitialize\" - Reinitialize Mips Sim.\n"
					  << "\"open\" - Open a file in your Mips Sim folder.\n"
					  << "\"run\" - Run the file opened by Mips Sim.\n"
					  << "\"data\" - View the data segment.\n"
					  << "\"edit\" - Enter edit mode.\n"
					  << "\"quit\" - Exit Mips Sim.\n"
					  << "========================================\n\n";
		}
		else if (input == "open")
		{
			std::cout << "\n\tPlease enter a file name. (Example: test.s)\n\n"
				  	  << NAME << ": ";
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
				std::cout << "\n\tFile: \"" << input << "\" has been opened.\n\n";
			}
			else
			{
				std::cout << "\n\tCould not find file: \"" << input << "\".\n\n";
			}
		}
		else if (input == "data")
		{
			std::cout << '\n';
			interpreter.data_view();
			std::cout << '\n';
		}
		else if (input == "reinitialize")
		{
			interpreter.reinitialize();
			std::cout << "\n\tThe Interpreter has been reinitialized\n" << std::endl;
		}
		else if (input == "run")
		{
			interpreter.run();
		}
		else if (input == "") continue;
		else
		{
			std::cout << "\n\tInvalid command. Please try again.\n" << std::endl;
		}
	}

	return 0;
}