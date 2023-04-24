#include "../includes/webserv.hpp"

WebServer::WebServer(void) {
	return ;
}

WebServer::~WebServer(void) {
	while (_acceptfds.size() > 0)
		_acceptfds.erase(_acceptfds.begin());
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		it->second->close_socket();
	return ;
}

int	WebServer::parsefile(char *filename) {
	_servers = _config.parse(filename);
	if (_config.getError() == 1)
		return 0;
	_max_fd = _config.getMaxFd();
	FD_ZERO(&_sockets);
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		FD_SET(it->second->getSocket(), &_sockets);
	return 1;
}

int	WebServer::launch(char **envp) {
	fd_set  readfds;
	fd_set  writefds;
	struct timeval	timeout;
	int pending;
	int ret;
	while (1) {
		pending = 0;
		ret = 0;
		while (pending == 0) {
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
			memcpy(&readfds, &_sockets, sizeof(_sockets));
			FD_ZERO(&writefds);
			for (std::map<int, Server *>::iterator it = _writablefds.begin(); it != _writablefds.end(); it++)
				FD_SET(it->second->getSocket(), &writefds);
			std::cout << BHGRN << "\rWaiting..." << BLANK << std::flush;
			pending = select(_max_fd + 1, &readfds, &writefds, NULL, &timeout);
			if (pending < 0) {
				if (errno == EINTR) {
					this->reset();
					return 0;
				}
				std::cerr << RED << "select() failed" << BLANK << std::endl;
				this->reset();
			}
		}
		for (std::map<int, Server *>::iterator it = _writablefds.begin(); pending && it != _writablefds.end(); it++) {
			int	fd = it->second->getSocket();
			if (FD_ISSET(fd, &writefds)) {
				std::cout << "Pouette 1\n";   // DEBUG
				Server	*tmp = it->second;
				ret = it->second->sendResponse(_config.getErrors(), envp, _config.getUpload());
				if (ret == -1) {
					FD_CLR(fd, &_sockets);
					FD_CLR(fd, &readfds);
					_acceptfds.erase(it);
				}
				_writablefds.erase(it);
				if (tmp)
					delete tmp;
				pending--;
				it = _writablefds.begin();
				break;
			}
		}
		for (std::map<int, Server *>::iterator it = _acceptfds.begin(); pending && it != _acceptfds.end(); it++) {
			int	fd = it->second->getSocket();
			if (FD_ISSET(fd, &readfds)) {
				std::cout << "Pouette 2\n";   // DEBUG
				Server	*tmp = it->second;
				ret = it->second->parseRequest(_servers);
				if (ret <= 0) {
					if (!ret)
						_writablefds.insert(std::make_pair(it->first, it->second));
					FD_CLR(fd, &_sockets);
					FD_CLR(fd, &readfds);
					_acceptfds.erase(it);
					it = _acceptfds.begin();
					if (ret && tmp)
						delete tmp;
				}
				pending--;
				break;
			}
		}
		for (std::map<int, Server *>::iterator it = _servers.begin(); pending && it != _servers.end(); it++)
		{
			int	fd = it->second->getSocket();
			if (FD_ISSET(fd, &readfds))
			{
				std::cout << "Pouette 3\n";   // DEBUG
				int	new_socket = it->second->accept_fd();
				if (new_socket == -1)
					return 1;
/*				///////////////////////////////////////////////////////
				std::string		tmpRequest;
				char			buffer[10];
				std::vector<char>	tmpBuffer(10);
				int			res = recv(fd, tmpBuffer.data(), 10, 0);
				if (res < 0)
					std::cout << "Problem occured during RECV\n";
				memcpy(buffer, tmpBuffer.data(), tmpBuffer.size());
				if (buffer[0] == 0)
				{
					// Flush tmpServ & all.
					std::cout << "Pouette STOP buff = [" << tmpBuffer.data() << "]\n"; // DEBUG
					return (1);
				}
*/				///////////////////////////////////////////////////////
				std::cout << "Pouette 4\n";   // DEBUG
				if (new_socket > 0)
				{
					std::cout << "Pouette 5\n";   // DEBUG
				 	Server	*new_fd = new Server(*(it->second), new_socket);
					FD_SET(new_socket, &_sockets);
					if (new_socket > _max_fd)
						_max_fd = new_socket;
					_acceptfds.insert(std::make_pair(it->first, new_fd));
				}
				std::cout << "Pouette 6\n";   // DEBUG
				pending--;
				break;
			}
		}
	}
	return 0;
}

void	WebServer::reset(void) {
	for (std::map<int, Server *>::iterator it = _acceptfds.begin(); it != _acceptfds.end(); it++)
		it->second->close_socket();
	_acceptfds.clear();
	_writablefds.clear();
	_max_fd = _config.getMaxFd();
	FD_ZERO(&_sockets);
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		FD_SET(it->second->getSocket(), &_sockets);
}

void	WebServer::clean() {
	for (std::map<int, Server *>::iterator it = _acceptfds.begin(); it != _acceptfds.end(); it++)
		it->second->close_socket();
	_acceptfds.clear();
	_writablefds.clear();
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		it->second->close_socket();
	_servers.clear();
}
