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
		ServerInfo(ServerInfo* copy);
		ServerInfo(ServerInfo& copy);
		~ServerInfo();
		
		ServerInfo&	operator=(ServerInfo& copy);

		int	setServerNames(std::string names);
		int	setIp(std::string line);
		int	setIndex(std::string line);
		int	setRoot(std::string line);
		int	setClientSize(std::string line);
		int	setAutoIndex(std::string line);
		int	setAllow(std::string line);
		int	setLoc(Location& loc);
		
		std::string	getIp() const;
		std::string	getRoot() const;
		std::string	getIndex() const;
		long		getClientSize() const;
		int			getAutoIndex() const;
		int			getAllow(std::string allow) const;

		std::vector<std::string>	getServerNames() const;
		std::vector<Location>		getLoc() const;
	
	private:
	
		std::vector<std::string>	_serverNames;
		std::string					_ip;
		std::string					_root;
		std::string					_index;
		int							_allow[3]; // GET POST DELETE: 0 si interdit, 1 si autorise
		long						_clientSize;
		int							_autoIndex; //0 si off, 1 si on
		std::vector<Location>		_loc;
};

#endif
