#pragma once 

#include <vector>
#include <deque>
#include <iostream>
#include <exception>
#include <sstream>
#include <limits>
#include <algorithm>
#include <utility>


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

template <typename T>
void fordJhonson(T &c)
{
	if (c.size() <= 1)
		return ;
	std::vector<std::pair<int, int> > pairs;
	int pendding = 0;
	bool is_pending = (c.size() % 2 != 0);
	if (is_pending)
		pendding = c.back();

	for (size_t i = 0; i + 1 < c.size(); i += 2)
	{
		int a = c[i];
		int b = c[i + 1];
		if (a > b) 
			std::swap(a, b);
		pairs.push_back(std::make_pair(a, b));
	}
	T big;
	T small;

	for (size_t i = 0; i < pairs.size(); i++)
	{
		small.push_back(pairs[i].first);
		big.push_back(pairs[i].second);
	}
	fordJhonson(big);

	for (size_t i = 0; i < small.size(); i++)
	{
		typename T::iterator pos = std::lower_bound(big.begin(), big.end(), small[i]);
		big.insert(pos, small[i]);
	}
	if(is_pending)
	{
		typename T::iterator pos = std::lower_bound(big.begin(), big.end(), pendding);
		big.insert(pos, pendding);
	}
	c = big;
}