#include "RPN.hpp"

RPN::RPN(){}
RPN::RPN(const RPN &oth){ (void)oth; }
RPN &RPN::operator=(const RPN &oth)
{
	(void)oth;
	return *this;
}
RPN::~RPN(){}


bool	RPN::digit_token(const std::string &token)
{
	if ( token.size() == 1 && isdigit(token[0]))
		return true;
	return false;
}

bool	RPN::operator_token(const std::string &token)
{

	if (token.size() != 1)
		return false;
	if (token[0] == '+' || token[0] == '-' 
		|| token[0] == '/' || token[0] == '*')
		return true;
	return false;
}


void	RPN::experssion(const std::string &token)
{
	if (stack_t.size() < 2)
		throw std::runtime_error("Error");
	int b = stack_t.top();
	stack_t.pop();
	int a = stack_t.top();
	stack_t.pop();
	int res;
	if (token[0] == '*')
		res = a * b;
	else if (token[0] == '-')
		res = a - b;
	else if (token[0] == '+')
		res = a + b;
	else if (token[0] == '/')
	{
		if (b == 0)
			throw std::runtime_error("Error");
		res = a / b;
	}
	else 
		throw std::runtime_error("Error");
	stack_t.push(res);
}

void RPN::processInput(const std::string &av)
{
	std::istringstream iss(av);
	std::string token;
	while (iss >> token)
	{
		if (RPN::digit_token(token))
			stack_t.push(atoi(token.c_str()));
		else if (RPN::operator_token(token))
			experssion(token);
		else throw std::runtime_error("Error");
	}
	if (stack_t.size() != 1)
		throw std::runtime_error("Error");
	std::cout << stack_t.top() << std::endl;
}