#pragma once 

#include <vector>
#include <deque>
#include <iostream>
#include <exception>
#include <sstream>
#include <limits>


class PmergeMe
{
	public:	
		std::vector<int>	v;
		std::deque<int>		d;

		PmergeMe();
		~PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe &operator=(const PmergeMe &other);
		void 	print();
		void	proceesArg(char **av, int ac);
		void	checkArg(const std::string &str);
};