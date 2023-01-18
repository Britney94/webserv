#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once

#include "webserv.hpp"

class ServerInfo;

class Server {
	public:
		Server(void){};
		Server(ServerInfo* infos, int port = 80);
		Server(Server& copy, int new_socket);
		Server&	operator=(Server& copy);
		void	addNewInfo(ServerInfo* new_infos);
		void	setSocket(int socket_descriptor);
		int	accept_fd();
		int	getSocket() const;
		void	close_socket();
		int	parseRequest();
		int	sendResponse(std::map<int, std::string> errors);
		int	chunkedRequest();
		void	parseChunked();
		ServerInfo	*requestInfos();
		std::string	getError();
		std::vector<ServerInfo *>	getInfos() const;
		ServerInfo	*_default;
		std::vector<ServerInfo *>	_infos;
		~Server(void);
	private:
		std::string	_error;
		struct sockaddr_in	_addr;
		std::string	_file_request;
		std::string	_request;
		std::string	_body;
		std::string	_cgi;
		std::string	_method;
		int	_socket;
		int	_status;
		int	_size;
};

std::ostream	&operator<<(std::ostream &x, Server const & serv);

#endif
