#include <iostream>
#include <stack>
#include <queue>
#include <deque>
#include <cctype>
#include <string>

bool isValidExpression(const std::string &expression)
{
	int digits = 0;
	int operations = 0;

	for (std::string::size_type i = 0; i < expression.size(); ++i)
	{
		char c = expression[i];

		if (std::isspace(static_cast<unsigned char>(c)))
			continue;
		if (std::isdigit(static_cast<unsigned char>(c)))
		{
			++digits;
			continue;
		}
		if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			++operations;
			continue;
		}
		return (false);
	}
	return (digits > 0 && operations > 0 && digits == operations + 1);
}

float doOPeration(char symbol, float first, float second)
{
	if (symbol == '+')
		return (first + second);
	if (symbol == '-')
		return (first - second);
	if (symbol == '*')
		return (first * second);
	if (symbol == '/')
		return (first / second);
	return (0);
}

int main(int argc, char **argv)
{
	std::stack< float, std::deque<float> > digits;
	float res = 0;

	if (argc != 2)
	{
		std::cout << "Invalid usage, try ./RPN <your expression to solve>" << std::endl;
		return (1);
	}
	if (!isValidExpression(argv[1]))
	{
		std::cout << "Error: Invalid expression" << std::endl;
		return (1);
	}
	try
	{
		for (std::string::size_type i = 0; i < std::string(argv[1]).size(); ++i)
		{
			char c = argv[1][i];

			if (std::isspace(static_cast<unsigned char>(c)))
				continue;
			if (std::isdigit(static_cast<unsigned char>(c)))
			{
				digits.push(static_cast<float>(c - '0'));
				continue;
			}
			if (c == '+' || c == '-' || c == '*' || c == '/')
			{
				float second = digits.top();
    			digits.pop();

    			float first = digits.top();
    			digits.pop();

    			res = doOPeration(c, first, second);

    			digits.push(res);
			}
		}
		std::cout << "Result: " << res << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}