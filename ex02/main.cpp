#include "PmergeMe.hpp"

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Usage: ./PmergeMe n1 n2 ... nN\n";
		return 1;
	}
	try
	{
		PmergeMe pm;
		pm.proceesArg(av, ac);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}