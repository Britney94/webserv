#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once

#include "webserv.hpp"

class ServerInfo;

class Server {
	public:
		Server(void){};
		~Server(void);
		Server(ServerInfo* infos, int port = 80);
		Server(Server& copy, int new_socket);
		Server&	operator=(Server& copy);
		void	addNewInfo(ServerInfo* new_infos);
		void	setSocket(int socket_descriptor);
		void	close_socket();
		int	getSocket() const;
		int	getError() const;
		std::vector<ServerInfo *>	getInfos() const;
		int	accept_fd();
		int	parseRequest(std::map<int, Server *> servs);
		int	sendResponse(std::map<int, std::string> errors, char **envp, std::string uploadDir);
		int	chunkedRequest();
		void    parseChunked();
		int checkContentRequest();
		ServerInfo	*requestInfos(std::map<int, Server *> servs);
	private:
		struct sockaddr_in	_addr;
		std::vector<ServerInfo *>	_infos;
		ServerInfo	*_default;
		std::string	_file_request;
		std::string	_request;
		std::string	_body;
		std::string	_cgi;
		std::string	_method;
		std::fstream _tmpBody;
		std::vector<char> _vectorBody;
		int	_socket;
		int	_status;
		int	_error;
		int	_size;
};

/* Management function of the server settings: web socket, imap; error and status */

#endif
