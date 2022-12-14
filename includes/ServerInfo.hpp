#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <vector>

#include "WebServer.hpp"
#include "webserv.hpp"
#include "Server.hpp"
#include "File.hpp"

class	Server;

struct Location
{
	std::string	uri; 
	std::string	root;
	std::string	index;
	int	allow[3]; // GET POST DELETE : 0 si interdit, 1 si autorise
};

class ServerInfo 
{
	public:
		int	_allow[3]; // GET POST DELETE : 0 si interdit, 1 si autorise
		std::vector<Location>	_loc;
		ServerInfo();
		void	setServerName(std::string name);
		void	setIp(std::string line);
		void	setClientSize(std::string line);
		void	setAutoIndex(int autoIndex);
		void	setAllow(std::string line);
		void	setLoc(std::string uri, std::string root, std::string index, std::string allow);
		std::string	getServerName();
		std::string	getIp();
		long	getClientSize();
		int	getAutoIndex();
		int	getAllow(std::string allow);
		~ServerInfo();
	private:
		std::string	_ip;
		std::string	_serverName;
		long	_clientSize;
		int	_autoIndex; //0 si off, 1 si on
};

std::ostream	&operator<<(std::ostream &x, std::vector<Location> _loc);
std::ostream	&operator<<(std::ostream &x, ServerInfo inf);

#endif
