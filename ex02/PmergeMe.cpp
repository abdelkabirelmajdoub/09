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

void	PmergeMe::proceesArg(char **av, int ac)
{
	for (int i = 1; i < ac; i++)
		checkArg(av[i]);
	std::cout << "Before: ";
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;
	clock_t start = clock();
	fordJhonson(v);
	clock_t end = clock();
	double v_time = (double)(end - start) / CLOCKS_PER_SEC * 1e6;
	
	clock_t Dstart = clock();
	fordJhonson(d);
	clock_t Dend = clock();
	double d_time = (double)Dend - Dstart / CLOCKS_PER_SEC * 1e6;
	
	std::cout << "after: ";
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;

	std::cout << "Time to process a range of " << v.size() 
		<< " elements with std::vector : " << v_time << " us" << std::endl;
	std::cout << "Time to process a range of " << d.size() 
		<< " elements with std::deque : " << d_time <<  " us" << std::endl;
}


