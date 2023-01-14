#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

#pragma once

#include "webserv.hpp"

class ServerInfo {
	public:

	/* fonction getLoc() qui renvoit un vector<Location>
	ajouter PUT
	si allow_methods n'est pas precise, mettre tout a 1, pour server_info (pas pour location)
	si root n'est pas precise, faute de configuration ou route par defaut ? (pas pour location)
	pour location mettre clientSize par defaut a celui de serverInfo
	mettre autoindex a off part defaut
	enlever l'etoile dans location *.bla
	*/

		ServerInfo();
		~ServerInfo();
		
		int	setServerNames(std::string names);
		int	setIp(std::string line);
		int	setIndex(std::string line);
		int	setRoot(std::string line);
		int	setClientSize(std::string line);
		int	setAutoIndex(std::string line);
		int	setAllow(std::string line);
		int	setLoc(Location& loc);
		
		std::string	getIp();
		std::string	getRoot();
		std::string	getIndex();
		long		getClientSize();
		int			getAutoIndex();
		int			getAllow(std::string allow);

		std::vector<std::string>	getServerNames();
		std::vector<Location>		getLoc();
	
	private:
	
		std::vector<std::string>	_serverNames;
		std::string					_ip;
		std::string					_root;
		std::string					_index;
		int							_allow[4]; // GET POST DELETE PUT: 0 si interdit, 1 si autorise
		long						_clientSize;
		int							_autoIndex; //0 si off, 1 si on
		std::vector<Location>		_loc;
};

std::ostream	&operator<<(std::ostream &x, std::vector<Location> _loc);
std::ostream	&operator<<(std::ostream &x, ServerInfo inf);

#endif
