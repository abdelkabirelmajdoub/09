#pragma once
#include <map>
#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include <algorithm>

class BitcoinExchange
{
	public:
		std::map<std::string, float> db;
		
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &oth);
		BitcoinExchange &operator=(const BitcoinExchange &oth);
		~BitcoinExchange();
		void		loadDataBase(const std::string &s);
		void		processInput(char *av);
		void		parseLine(const std::string &date);
		void		procces_line(const std::string &line);
};

