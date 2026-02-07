#pragma once

#include <sstream>
#include <iostream>
#include <exception>
#include <stack>

class RPN
{
	public:
		RPN();
		RPN(const RPN &oth);
		RPN &operator=(const RPN &oth);
		~RPN();

		std::stack<int> stack_t;
		void	processInput(const std::string &av);
		bool	operator_token(const std::string &token);
		bool	digit_token(const std::string &token);
		void	experssion(const std::string &token);
};

