#include <iostream>
#include <string>
#include <fstream>
#include "fileutil.h"

const std::string NAME = "{Simulator Name}";

int main()
{
	int arr[10][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			arr[j][i] = -1;
		}
	}
	std::cout << NAME << ": Please enter a filename. (Example: main.s)\n"
			  << NAME << ": ";
	//fileutil.open();
	//interpreter.interpret(fileutil.getline());
	std::string input;
	std::getline(std::cin, input);
	std::ifstream infile(input.c_str(), std::ios::in);
	std::string file(input);
	std::string line;
	if (infile.is_open())
	{
		//while (infile)
		//{
			getline(infile, line);
			std::cout << line << std::endl;
			int row = 0; //up to 10
			int column = 0; //up to 4
			for (int i = 0; i < line.size(); i++) 
			{
				if (line[i] != 9 && line[i] != 32)
				{
					if (line[i] == 36 || line[i] == 44)
					{
						column++;
						row = 0;
						continue;
					}
					arr[row++][column] = line[i];
				}
  			}
		//}
		infile.close();
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			std::cout << arr[j][i] << ' ';
		}
		std::cout << std::endl;
	}

	return 0;
}