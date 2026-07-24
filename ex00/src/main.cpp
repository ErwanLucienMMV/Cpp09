#include "BitcoinExchange.hpp"
#include "Date.hpp"
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <cerrno>
#include <climits>
#include <limits>
#include <stdexcept>

static std::string trim(const std::string &str)
{
	size_t start = 0;
	size_t end = str.size();

	while (start < end && std::isspace(static_cast<unsigned char>(str[start])))
		++start;
	while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
		--end;
	return (str.substr(start, end - start));
}

static bool parseInt(const std::string &str, int &value)
{
	char *end = 0;
	long parsed = std::strtol(str.c_str(), &end, 10);

	if (str.empty() || end == str.c_str() || *end != '\0' || parsed < INT_MIN || parsed > INT_MAX)
		return (false);
	value = static_cast<int>(parsed);
	return (true);
}

static bool parseDouble(const std::string &str, double &value)
{
	char *end = 0;

	errno = 0;
	value = std::strtod(str.c_str(), &end);
	if (str.empty() || end == str.c_str() || *end != '\0' || errno == ERANGE)
		return (false);
	return (true);
}

static bool parseDate(const std::string &str, Date &date)
{
	std::stringstream dateStream(str);
	std::string yearStr;
	std::string monthStr;
	std::string dayStr;
	std::string extra;
	int year;
	int month;
	int day;

	if (!std::getline(dateStream, yearStr, '-') || !std::getline(dateStream, monthStr, '-') || !std::getline(dateStream, dayStr, '-'))
		return (false);
	if (std::getline(dateStream, extra, '-'))
		return (false);
	if (!parseInt(yearStr, year) || !parseInt(monthStr, month) || !parseInt(dayStr, day))
		return (false);
	try
	{
		date = Date(year, month, day);
	}
	catch (std::exception &)
	{
		return (false);
	}
	return (true);
}

static bool sameDate(const Date &left, const Date &right)
{
	return (!(left < right) && !(right < left));
}

std::map<Date, float> parseDataBase(void)
{
	std::ifstream myfile;
	std::string str;
	std::map<Date, float> res;

	myfile.open("data.csv");
	if (myfile.is_open())
	{
		std::getline(myfile, str);
		while (std::getline(myfile, str))
		{
			try
			{
				if (str.empty())
					continue;

				std::stringstream ss(str);
				std::string dateStr;
				std::string valueStr;
				std::getline(ss, dateStr, ',');
				std::getline(ss, valueStr, ',');

				if (dateStr.empty() || valueStr.empty())
					throw std::runtime_error("Invalid CSV line");

				std::stringstream dateStream(dateStr);
				std::string yearStr;
				std::string monthStr;
				std::string dayStr;
				std::getline(dateStream, yearStr, '-');
				std::getline(dateStream, monthStr, '-');
				std::getline(dateStream, dayStr, '-');

				int year = std::atoi(yearStr.c_str());
				int month = std::atoi(monthStr.c_str());
				int day = std::atoi(dayStr.c_str());
				float value = std::atof(valueStr.c_str());

				Date date(year, month, day);
				res[date] = value;
			}
			catch (std::exception &e)
			{
				std::cout << "Database file contained anormal data, exiting the program" << std::endl;
				myfile.close();
				exit(1);
			}
		}
		myfile.close();
		return (res);
	}
	else
	{
		std::cout << "Database file was not found, exiting the program" << std::endl;
		exit(1);
	}
}

void processInputFile(const std::string &filename, const std::map<Date, float> &data)
{
	std::ifstream input(filename.c_str());
	std::string line;
	bool headerSkipped = false;

	if (data.size() < 1)
	{
		std::cout << "Invalid argument, database was empty" << std::endl;
		return ;
	}

	if (!input.is_open())
	{
		std::cout << "Error: could not open file." << std::endl;
		return ;
	}
	while (std::getline(input, line))
	{
		std::string trimmedLine = trim(line);

		if (trimmedLine.empty())
			continue;
		if (!headerSkipped)
		{
			headerSkipped = true;
			if (trimmedLine.find("date") != std::string::npos && trimmedLine.find("value") != std::string::npos)
				continue;
		}
		std::stringstream lineStream(trimmedLine);
		std::string dateToken;
		std::string valueToken;

		if (!std::getline(lineStream, dateToken, '|') || !std::getline(lineStream, valueToken, '|'))
		{
			std::cout << "Error: bad input => " << trimmedLine << std::endl;
			continue;
		}
		dateToken = trim(dateToken);
		valueToken = trim(valueToken);
		if (dateToken.empty() || valueToken.empty())
		{
			std::cout << "Error: bad input => " << trimmedLine << std::endl;
			continue;
		}
		Date inputDate;
		if (!parseDate(dateToken, inputDate))
		{
			std::cout << "Error: bad input => " << dateToken << std::endl;
			continue;
		}
		double value;
		if (!parseDouble(valueToken, value))
		{
			std::cout << "Error: bad input => " << trimmedLine << std::endl;
			continue;
		}
		if (value < 0.0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > static_cast<double>(std::numeric_limits<int>::max()))
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}
		std::map<Date, float>::const_iterator it = data.begin();
		std::map<Date, float>::const_iterator closest = data.end();
		for (; it != data.end(); ++it)
		{
			if (inputDate < data.begin()->first)
			{
				std::cout << "Before: "<< dateToken << " => " << "-42" << " = " << "-42 (satoshi hadn't published the paper yet)" << std::endl;
				break ;
			}
			if (sameDate(it->first, inputDate))
			{
				closest = it;
				break;
			}
			if (it->first < inputDate)
				closest = it;
		}
		if (closest == data.end() && !data.empty())
			closest = data.begin();
		if (closest == data.end())
			continue;
		double result = value * closest->second;
		std::cout << dateToken << " => " << valueToken << " = " << result << std::endl;
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Invalid argument, usage: ./btc <dates asked>" << std::endl;
		return (1);
	}

	BitcoinExchange exchange;
	exchange.setData(parseDataBase());
	processInputFile(argv[1], exchange.getData());
	
	return (0);
}