#include "../includes/Date.hpp"


bool Date::isLeapYear(int year)
{
	if (year % 400 == 0)
		return (true);
	if (year % 100 == 0)
		return (false);
	if (year % 4 == 0)
		return (true);
	return (false);
}

int Date::getDaysInMonth(int month, int year)
{
	int daysPerMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month < 1 || month > 12)
		throw MonthOutOfBoundsException();

	if (month == 2 && isLeapYear(year))
		return (29);
	return (daysPerMonth[month]);
}


Date::Date(void) : Year(1970), Month(1), Day(1)
{
}


Date::Date(int year, int month, int day) : Year(year), Month(1), Day(1)
{
	setMonth(month);
	setDay(day);
	this->Year = year;
}


Date::Date(const Date &other) : Year(other.Year), Month(other.Month), Day(other.Day)
{
}


Date &Date::operator=(const Date &other)
{
	if (this != &other)
	{
		this->Year = other.Year;
		this->Month = other.Month;
		this->Day = other.Day;
	}
	return (*this);
}


Date::~Date(void)
{
}


bool Date::operator<(const Date &other) const
{
	if (this->Year != other.Year)
		return (this->Year < other.Year);
	if (this->Month != other.Month)
		return (this->Month < other.Month);
	return (this->Day < other.Day);
}

bool Date::operator>(const Date &other) const
{
	if (this->Year != other.Year)
		return (this->Year > other.Year);
	if (this->Month != other.Month)
		return (this->Month > other.Month);
	return (this->Day > other.Day);
}


int Date::getYear(void) const
{
	return (this->Year);
}

int Date::getMonth(void) const
{
	return (this->Month);
}

int Date::getDay(void) const
{
	return (this->Day);
}


void Date::setYear(int year)
{
	this->Year = year;
}

void Date::setMonth(int month)
{
	if (month < 1 || month > 12)
		throw MonthOutOfBoundsException();
	this->Month = month;
}

void Date::setDay(int day)
{
	int maxDays = Date::getDaysInMonth(this->Month, this->Year);
	if (day < 1 || day > maxDays)
		throw DayOutOfBoundsException();
	this->Day = day;
}


std::ostream &operator<<(std::ostream &os, const Date &date)
{
	os << date.getYear() << "-" << date.getMonth() << "-" << date.getDay();
	return (os);
}
