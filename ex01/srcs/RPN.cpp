#include "RPN.hpp"

RPN::RPN(void)
{
}

RPN::RPN(const RPN &other) : data(other.data)
{
}

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
		data = other.data;
	return (*this);
}

RPN::~RPN(void)
{
}

bool RPN::isValidExpression(const std::string &expression) const
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

float RPN::evaluate(const std::string &expression)
{
	if (!isValidExpression(expression))
		throw std::runtime_error("Error: Invalid expression");

	while (!data.empty())
		data.pop();

	for (std::string::size_type i = 0; i < expression.size(); ++i)
	{
		char c = expression[i];

		if (std::isspace(static_cast<unsigned char>(c)))
			continue;
		if (std::isdigit(static_cast<unsigned char>(c)))
		{
			data.push(static_cast<float>(c - '0'));
			continue;
		}
		if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			if (data.size() < 2)
				throw std::runtime_error("Error: Invalid expression, operator coming too early, are you sure it is POLISH REVERSE NOTATION?");

			float second = data.top();
			data.pop();
			float first = data.top();
			data.pop();

			if (c == '+')
				data.push(first + second);
			else if (c == '-')
				data.push(first - second);
			else if (c == '*')
				data.push(first * second);
			else
				data.push(first / second);
		}
	}

	if (data.size() != 1)
		throw std::runtime_error("Error: Invalid expression");
	return (data.top());
}