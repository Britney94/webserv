#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

class Location
{
	public:
		std::string	uri; 
		std::string	root;
		std::string	index;
		std::string	cgi;
		int	        allow[4]; // GET POST DELETE PUT: 0 si interdit, 1 si autorise
		long		clientSize;
		std::vector<Location>	loc;

		Location(void){
			uri = "";
			root = "";
			index = "";
			cgi = "";
			allow[0] = 0;
			allow[1] = 0;
			allow[2] = 0;
			allow[3] = 0;
			clientSize = -1;
		};

		Location& operator=(Location copy) {
		 	uri = copy.uri;
		 	root = copy.root;
		 	index = copy.index;
		 	cgi = copy.cgi;
		 	allow[0] = copy.allow[0];
		 	allow[1] = copy.allow[1];
		 	allow[2] = copy.allow[2];
		 	allow[3] = copy.allow[3];
		 	clientSize = copy.clientSize;
		 	loc = copy.loc;
			return *this;
		};
};

//#include "AutoIndex.hpp"
//#include "CGI.hpp"
//#include "ClientRequest.hpp"
#include "ConfigInfo.hpp"
#include "File.hpp"
#include "Server.hpp"
#include "ServerInfo.hpp"
#include "WebServer.hpp"
// #include "HttpResponse.hpp"

#define BLANK "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define CYAN "\033[36m"

#define MAX_FD 2048
#define REQUEST_SIZE 30000



#endif
