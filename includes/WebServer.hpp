#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#pragma once

#include "webserv.hpp"
#include "Server.hpp"

class Server;

class WebServer {
	
	public:
		WebServer(void);
		~WebServer(void);
		void		setRunning(int running);
		int			getRunning();
		int		parsefile(char *filename);
		int		launch();
		void		clean();
		void	reset();

		void	setRunning(int num);
	
	private:
		int isRunning;
		ConfigInfo				_config;
		std::map<int, Server *>	_servers;
		std::map<int, Server *>	_acceptfds;
		std::map<int, Server *>	_writablefds;	
		fd_set					_sockets;
		int						_max_fd;
		int						_isRunning;
};

#endif
