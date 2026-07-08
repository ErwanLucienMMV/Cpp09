#ifndef DATE_HPP
# define DATE_HPP

#include <iostream>
#include <exception>

class Date 
{
	public:
		Date(void);
		Date(int year, int month, int day);
		Date(const Date &other);
		Date &operator=(const Date &other);
		~Date(void);

		bool operator<(const Date &other) const;
		bool operator>(const Date &other) const;

		int getYear(void) const;
		int getMonth(void) const;
		int getDay(void) const;

		void setYear(int year);
		void setMonth(int month);
		void setDay(int day);

		static bool isLeapYear(int year);
		static int getDaysInMonth(int month, int year);

		class MonthOutOfBoundsException : public std::exception
		{
			public:
				virtual const char *what(void) const throw()
				{
					return ("Error: Month out of bounds (must be between 1 and 12)");
				}
		};

		class DayOutOfBoundsException : public std::exception
		{
			public:
			virtual const char *what(void) const throw()
			{
				return ("Error: Day out of bounds for the given month");
			}
		};

	private:
		int Year;
		int Month;
		int Day;
};


std::ostream &operator<<(std::ostream &os, const Date &date);

#endif