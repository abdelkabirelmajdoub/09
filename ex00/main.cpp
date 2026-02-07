#include <iostream>
#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: couldn't open file.\n"; 
		return 1;
	}
	try
	{
		BitcoinExchange ex;
		ex.loadDataBase("data.csv");
		ex.processInput(av[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}