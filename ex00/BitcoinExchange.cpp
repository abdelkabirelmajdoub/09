#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(){}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &oth)
{
	(void)oth;
}
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &oth)
{
	(void)oth;
	return *this;
}
BitcoinExchange::~BitcoinExchange(){}


static std::string trim(const std::string &str)
{
	size_t start = 0;
	while(start < str.size() && isspace(str[start]))
		start++;
	size_t end = str.size();
	while(end > start && isspace(str[end - 1]))
		end--;
	return str.substr(start, end - start);
}


void	BitcoinExchange::parseLine(const std::string &line)
{
	size_t coma = line.find(',');
	if (coma == std::string::npos)
		throw std::runtime_error("Invalid Format DataBase");
	
	std::string full_date = trim(line.substr(0, coma));
	std::string rate_str = trim(line.substr(coma + 1));
	if (full_date.empty() || rate_str.empty())
		throw std::runtime_error("Invalid format Data base.");
	float rate = atof(rate_str.c_str());
	db[full_date] = rate;
}

void		BitcoinExchange::loadDataBase(const std::string &s)
{
	std::ifstream fd(s);
	if (!fd.is_open())
		throw std::runtime_error("Error: unable open file (data.csv) for reading.");
	
	std::string line;
	if (!std::getline(fd, line))
		throw std::runtime_error("Error: empty data base.");
	while (std::getline(fd, line))
	{
		if (line.empty())
			continue;
		parseLine(line);
	}

	fd.close();
}
// parse input file ------->>>
static bool onlyDigit(const std::string &s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.length(); i++)
	{
		if (!isdigit(s[i]))
			return false;
	}
	return true;
}

static bool isLeap(int y)
{
	if (y % 4 == 0)
	{
		if (y % 100 == 0)
			return (y % 400 == 0);
		return true;
	}
	return false;
}
int days_inmonth(int m, int y)
{
    const int days[] = { 
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (m == 2)
        return isLeap(y) ? 29 : 28;
	return days[m - 1];
} 

static bool checkDate(int y, int m, int d)
{
	if (y < 1)
		return false;
	if (m < 1 || m > 12)
		return false;
	
	int dim = days_inmonth(m, y);

	return (d >= 1 && d <= dim);
}

static bool parse_date(std::string date)
{
	std::stringstream ss(date);
	std::string year, month, day;
	if (!std::getline(ss, year, '-') ||
		!std::getline(ss, month, '-') ||
		!std::getline(ss, day, '-'))
		return false;
	if (!onlyDigit(year) || !onlyDigit(month) || !onlyDigit(day))
		return false;
	int years = atoi(year.c_str()), months = atoi(month.c_str()),
					days = atoi(day.c_str());

	if (!checkDate(years, months, days))
		return false;
	return true;
}

void BitcoinExchange::procces_line(const std::string &line)
{
	size_t coma = line.find('|');
	if (coma == std::string::npos)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return;
    }
	std::string full_date = trim(line.substr(0, coma));
	if (full_date.empty() )
	{
		std::cerr << "bad input: no date.\n";
		return ;
	}
	if (!parse_date(full_date))
	{
		std::cerr << "Error: bad input => " << full_date << std::endl;
		return ;
	}
	if (coma == std::string::npos)
	{
		std::cerr << "Invalid Format input.\n";
		return ;
	}
	
	std::string value_str = trim(line.substr(coma + 1));
	if (value_str.empty())
	{
		std::cerr << "Bad input: no value.\n";
		return ;
	}
	float value = atof(value_str.c_str());
	if (value < 0)
	{
		std::cerr << "Error: not a positive number.\n";
		return ;
	}
	if (value > 1000)
	{
		std::cerr << "Error: too large number.\n";
		return ;
	}
	if (db.empty())
	{
		std::cerr << "Bad input => " << line << std::endl;
		return ;
	}
	std::map<std::string, float>::iterator it = db.find(full_date);
	if (it != db.end())
		std::cout << full_date << " => " << value << " = " << it->second * value << std::endl;
	else
	{
		it = db.lower_bound(full_date);
		if (it == db.begin())
		{
			std::cerr << "Error: too early date.\n";
			return ;
		}
		it--;
		std::cout << full_date << " => " << value << " = " << it->second * value << std::endl;
	}
}

void		BitcoinExchange::processInput(char *av)
{
	std::ifstream file(av);
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");
	std::string line;
	if (!std::getline(file , line))
		throw std::runtime_error("Error: empty file input.");
	while(std::getline(file, line))
	{
		if (line.empty())
			continue;
		procces_line(line);
	}
}