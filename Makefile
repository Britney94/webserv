NAME = webserv 
CC = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror
INCLUDES = -I ./includes/

SRCS = srcs/Server.cpp \
       srcs/Location.cpp \
       srcs/WebServer.cpp \
       srcs/AutoIndex.cpp \
       srcs/CGI.cpp \
       srcs/ServerInfo.cpp \
       srcs/ClientRequest.cpp \
       srcs/File.cpp \
       srcs/ConfigInfo.cpp \
       srcs/main.cpp

CLIE = client.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@ $(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

client:
	@ $(CC) $(CFLAGS) $(INCLUDES) $(CLIE) -o client

%.o: %.cpp
	@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@ rm -f $(OBJS)

fclean: clean
	@ rm -f $(NAME)

re: fclean all

pouette: $(OBJS)
	@ $(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)


test: all
	@ valgrind ./webserv config/default.conf

.PHONY: all clean fclean re bonus
