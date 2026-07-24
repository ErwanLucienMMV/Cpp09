#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include "Date.hpp"
# include <map>

class BitcoinExchange
{
	public:
		BitcoinExchange(void);
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange(void);

		const std::map<Date, float> &getData(void) const;
		void setData(const std::map<Date, float> &newData);

	private:
		std::map<Date, float> data;
};

#endif
