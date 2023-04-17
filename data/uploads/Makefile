NAME=	PmergeMe
SRC=	main.cpp \
	PmergeMe.cpp

OBJ=	$(SRC:.cpp=.o)

FLAG=	-Wall -Wextra -Werror -std=c++98

$(NAME):	$(OBJ)
	c++ $(FLAG) $(SRC) -o $(NAME)

all:	$(NAME)

clean:
	rm -rf $(OBJ)

fclean:	clean
	rm -rf $(NAME)

re: fclean all
