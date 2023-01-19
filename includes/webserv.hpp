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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sstream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "AutoIndex.hpp"
#include "CGI.hpp"
#include "Location.hpp"
#include "ClientRequest.hpp"
#include "ConfigInfo.hpp"
#include "File.hpp"
#include "Server.hpp"
#include "ServerInfo.hpp"
#include "WebServer.hpp"
#include "HttpResponse.hpp"

std::string trim(const std::string& str);

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
