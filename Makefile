NAME = webserv
CC = c++ -g #TO DELETE -g
CFLAGS = -std=c++98 -Wall -Wextra -Werror
INCLUDES = -I ./includes/

SRCS = srcs/Server.cpp \
       srcs/Location.cpp \
       srcs/WebServer.cpp \
       srcs/AutoIndex.cpp \
       srcs/CGI.cpp \
       srcs/utils.cpp \
       srcs/ServerInfo.cpp \
       srcs/ClientRequest.cpp \
       srcs/HttpResponse.cpp \
       srcs/File.cpp \
       srcs/ConfigInfo.cpp \
       srcs/main.cpp

CLIE = client.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@ $(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@ mkdir -p tmp

client:
	@ $(CC) $(CFLAGS) $(INCLUDES) $(CLIE) -o client

%.o: %.cpp
	@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@ rm -f $(OBJS)
	@ rm -rf tmp

fclean: clean
	@ rm -f $(NAME)

re: fclean all

test: all
	@ ./webserv config/default.conf

vtest: all
	@ valgrind --show-leak-kinds=all --leak-check=full --track-origins=yes ./webserv config/default.conf

.PHONY: all clean fclean re bonus
