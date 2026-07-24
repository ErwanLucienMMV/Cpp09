#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <list>
# include <string>
#include <stdexcept>

class RPN
{
	public:
		RPN(void);
		RPN(const RPN &other);
		RPN &operator=(const RPN &other);
		~RPN(void);

		bool isValidExpression(const std::string &expression) const;
		float evaluate(const std::string &expression);

	private:
		std::stack<float, std::list<float> > data;
};

#endif
