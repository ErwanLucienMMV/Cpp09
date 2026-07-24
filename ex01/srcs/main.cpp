#include "RPN.hpp"
#include <iostream>
#include <cctype>
#include <stdexcept>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Invalid usage, try ./RPN <your expression to solve>" << std::endl;
		return (1);
	}

	try
	{
		RPN rpn;
		float result = rpn.evaluate(argv[1]);
		std::cout << "Result: " << result << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}