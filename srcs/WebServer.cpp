#include "../includes/webserv.hpp"

WebServer::WebServer(void) {
	this->_isRunning = 1;
	return ;
}

void	WebServer::setRunning(int running) {
	this->_isRunning = running;
}

int WebServer::getRunning() {
	return this->_isRunning;
}

WebServer::~WebServer(void) {
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		it->second->close_socket();
	}
	return ;
}

int	WebServer::parsefile(char *filename) {
	_servers = _config.parse(filename);
	if (_config.getError() == 1) {
		return 0;
	}
	_max_fd = _config.getMaxFd();
	FD_ZERO(&_sockets);
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		FD_SET(it->second->getSocket(), &_sockets);
	}
	return 1;
}

int	WebServer::launch(void) {
	
	fd_set			readfds;
	fd_set			writefds;
	struct timeval	timeout;
	int				pending;
	int				ret;
	
	_isRunning = 1;

	while (_isRunning) {
		
		pending = 0;
		ret = 0;
		
		while (pending == 0) {
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			memcpy(&readfds, &_sockets, sizeof(_sockets));
			FD_ZERO(&writefds);

			for (std::map<int, Server *>::iterator it = _writablefds.begin(); it != _writablefds.end(); it++) {
				FD_SET(it->second->getSocket(), &writefds);
			}
			std::cout << "\rWaiting" << std::flush;
			pending = select(_max_fd + 1, &readfds, &writefds, NULL, &timeout);
			if (pending < 0) {
				if (errno == EINTR) {
					this->reset();
					return 0;
				}
				std::cerr << "select() failed" << std::endl;
				this->reset();
			}
			// if (_isRunning == 0) {
			// 	this->reset();
			// 	return 0;
			// }
		}

		for (std::map<int, Server *>::iterator it = _writablefds.begin(); pending && it != _writablefds.end(); it++) {
			int	fd = it->second->getSocket();

			if (FD_ISSET(fd, &writefds)) {
				std::map<int, Server *>::iterator tmp;
				it->second->sendResponse(_config.getErrors());

				tmp = it++;
				_writablefds.erase(tmp);
				pending--;
				break;
			}
		}


		for (std::map<int, Server *>::iterator it = _acceptfds.begin(); pending && it != _acceptfds.end(); it++) {
			int	fd = it->second->getSocket();
			if (FD_ISSET(fd, &readfds)) {
				std::map<int, Server *>::iterator tmp;
				ret = it->second->parseRequest();

				if (ret <= 0) {
					if (!ret)
						_writablefds.insert(std::make_pair(it->first, it->second));
					FD_CLR(fd, &_sockets);
					FD_CLR(fd, &readfds);
					tmp = it++;
					_acceptfds.erase(tmp);
				}
				pending--;
				break;
			}
		}

		for (std::map<int, Server *>::iterator it = _servers.begin(); pending && it != _servers.end(); it++) {
			int	fd = it->second->getSocket();

			if (FD_ISSET(fd, &readfds)) {
				int	new_socket = it->second->accept_fd();
				if (new_socket == -1)
					return 1;
				if (new_socket > 0) {
				 	Server	*new_fd = new Server(*(it->second), new_socket);
					
					FD_SET(new_socket, &_sockets);
					if (new_socket > _max_fd)
						_max_fd = new_socket;
					
					_acceptfds.insert(std::make_pair(it->first, new_fd));
				}
				pending--;
				break;
			}
		}
	}
	return 0;
}

void	WebServer::reset(void) {
	for (std::map<int, Server *>::iterator it = _acceptfds.begin(); it != _acceptfds.end(); it++) {
		it->second->close_socket();
	}
	_acceptfds.clear();
	_writablefds.clear();
	FD_ZERO(&_sockets);
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		FD_SET(it->second->getSocket(), &_sockets);
	}
}

void	WebServer::clean() {
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		it->second->close_socket();
	}
	_servers.clear();
}
