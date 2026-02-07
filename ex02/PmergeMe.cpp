#include "PmergeMe.hpp"

PmergeMe::PmergeMe(){}
PmergeMe::~PmergeMe(){}
PmergeMe::PmergeMe(const PmergeMe& other){(void)other;}
PmergeMe &PmergeMe::operator=(const PmergeMe &other){
	(void)other;
	return *this;
}

bool	validNumber(const std::string token)
{
	for (size_t i = 0; i < token.length(); i++)
	{
		if (!isdigit(token[i]))
			throw std::runtime_error("Error: Should be only positive integer.");
	}
	float tmp = atof(token.c_str());
	if (tmp > std::numeric_limits<int>::max())
			throw std::runtime_error("Error: Number too large.");
	return true;
}
void	PmergeMe::checkArg(const std::string &str)
{
	std::istringstream iss(str);
	std::string token;

	while(iss >> token)
	{
		if (validNumber(token))
		{
			v.push_back(atoi(token.c_str()));
			d.push_back(atoi(token.c_str()));
		}
	}
}

void PmergeMe::print()
{
	std::vector<int>::iterator it = v.begin();
	std::cout << "Before: ";
	for (; it != v.end(); it++)
	{
		std::cout << *it << " " ;
	}
	std::cout << std::endl;
}
void	PmergeMe::proceesArg(char **av, int ac)
{
	for (int i = 1; i < ac; i++)
		checkArg(av[i]);
	print();
}


