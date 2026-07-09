#include "Date.hpp"
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

std::map<Date, int> parseDataBase(void)
{
	std::ifstream myfile;
	std::string str;
	std::map<Date, int> res;

	myfile.open("data.csv");
	if (myfile.is_open())
	{
		std::getline(myfile, str, '\n');
		while (std::getline(myfile, str, '\n'))
		{
			try 
			{

			}
			catch (std::exception &e)
			{

			}
		}
	}
	else
	{
		std::cout << "Database file was not found, exiting the program" << std::endl;
		exit(1);
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Invalid argument, usage: ./btc <dates asked>" << std::endl;
	}
	
	return (0);
}