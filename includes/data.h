#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>

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

class Data
{
public:
	Data(int i=-9999, char c='~', std::string s="", std::string label="")
		: i_(i), c_(c), s_(s), label_(label)
	{}
	Data & operator=(const Data & data)
	{
		if (i_ == -9999 && s_ == "" && data.getChar() != '~') 
		{
			c_ = data.getChar();
		}
		if (i_ == -9999 && c_ == '~' && data.getString() != "")
		{
			s_ = data.getString();
		}
		if (i_ != -9999 && c_ == '~' && data.getString() == "")
		{
			i_ = data.getInt();
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
	void setChar(const char & c)
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
	void view()
	{
    	std::cout << getLabel() << ": ";
    	if (getInt() != -9999) std::cout << getInt();
		if (getChar() != '~') std::cout << getChar();
		if (getString() != "")
		{
			for (int i = 0; i < getString().size(); i++)
			{
				std::cout << getString()[i];
			}
		}
	}
private:
	int i_;
	char c_;
	std::string s_;
	std::string label_;
};

std::ostream & operator<<(std::ostream & cout, const Data & data)
{
	if (data.getInt() != -9999 && data.getChar() == '~' && data.getString() == "") cout << data.getInt();
	else if (data.getChar() != '~') cout << data.getChar();
	else if (data.getString() != "")
	{
		for (int i = 0; i < data.getString().size(); i++)
		{
			if (i < data.getString().size() - 1 && 
				data.getString()[i] == '\\')
			{
				if (data.getString()[i + 1] == 'n')
				{
					std::cout << '\n';
					i += 1;
					continue;
				}
				else if (data.getString()[i + 1] == 't')
				{
					std::cout << '\t';
					i += 1;
					continue;
				}
			}
			std::cout << data.getString()[i];
		}
	}
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

#endif