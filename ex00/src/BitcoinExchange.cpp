#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(void)
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : data(other.data)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		data = other.data;
	return (*this);
}

BitcoinExchange::~BitcoinExchange(void)
{
}

const std::map<Date, float> &BitcoinExchange::getData(void) const
{
	return (data);
}

void BitcoinExchange::setData(const std::map<Date, float> &newData)
{
	data = newData;
}
