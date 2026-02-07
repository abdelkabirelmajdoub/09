CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC =  main.cpp BitcoinExchange.cpp

OBJ = $(SRC:.cpp=.o)

NAME = btc

all: $(NAME)
 
$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o:%.cpp	BitcoinExchange.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all