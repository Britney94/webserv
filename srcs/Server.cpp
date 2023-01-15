#include "../includes/webserv.hpp"

Server::Server(ServerInfo* infos, int port)
{

	// Generating socket file descriptor
	// DOMAIN	= Ipv4 Internet protocol
	// TYPE		= Non-blocking socket descriptor
	// 		  (prevent usage of fcntl()
	// PROTOCOL	= Default (unspecified)
	// port = 80;
	
	if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Error socket");
        return ;
    }
    
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->_addr.sin_port = htons(port); 
	
	// ioctl() a voir

	memset(this->_addr.sin_zero, '\0', sizeof(this->_addr.sin_zero));
	
	////////////////////////////////////
        
	// Binding the socket to the
	// socket-address struct parameters
	// +
	// Start listening on the socket
	
	if (bind(this->_socket, (struct sockaddr *)&(this->_addr), sizeof(this->_addr)) < 0)
	{
		perror("Error bind");
        return ;
	}

	if (listen(this->_socket, MAX_FD) < 0)
	{
		perror("Error listen");
        return ;
	}
	this->_error = "";
	this->_default = infos;
	this->_infos.push_back(infos);
	this->_size = this->_infos.size();
	this->_status = 200;
}


Server::Server(Server& copy, int new_socket)
{
	this->_socket = new_socket;
	this->_default = copy._default;
	this->_infos = copy._infos;
	this->_size = this->_infos.size();
}

Server& Server::operator=(Server& copy)
{
	this->_socket = copy._socket;
	this->_default = copy._default;
	this->_infos = copy._infos;
	this->_size = this->_infos.size();
	return (*this);
}

Server::~Server(void)
{
	// Default destructor, closes the
	// socket descriptor generated at
	// construction.
	close(this->_socket);
}


int	Server::getSocket(void) const
{
	// Simple getter : socket descriptor
	return (this->_socket);
}

void	Server::setSocket(int socket_descriptor)
{
	// Simple setter : socket descriptor
	this->_socket = socket_descriptor;
}

void	Server::addNewInfo(ServerInfo* new_infos)
{
	// Adds a new ServerInfo into the current
	// Server's vector (of infos),
	// modifying vector size accordingly.
	this->_infos.push_back(new_infos);
	this->_size++;
}


int	Server::accept_fd() {
	
	int	new_socket;
	int	size = sizeof(_addr);

	new_socket = accept(_socket, (struct sockaddr *)&_addr, (socklen_t *)&(size));

	if (new_socket == -1)
		std::cerr << "Error: accept()" << std::endl;
	return new_socket;
}

void	Server::close_socket() {
	if (_socket > 0)
		close(_socket);
}

int	Server::parseRequest() {
	int		ret;
	char	buffer[REQUEST_SIZE] = {0};

	ret = read(_socket, buffer, REQUEST_SIZE - 1);

	if (ret <= 0) {
		this->close_socket();
		std::cerr << "Error : Could not read from the socket.\n" << std::endl;
		return -1;
	}

	_request += std::string(buffer);
	if (_request.find("Transfer-Encoding: chunked") != std::string::npos) {
		if (this->chunkedRequest())
			return 1;
		else
			parseChunked(); 
	}

	std::cout << _request << std::endl;

	// ServerInfo	clientInfo = requestInfos();
	// ClientRequest	client(clientInfo, _request);

	// _file_request = client.getFile();
	// _status = client.getStatus();

	return 0;
}

int	Server::chunkedRequest() {
	if (_request.find("0\r\n\r\n") != std::string::npos)
		return 0;
	return 1;
}

void	Server::parseChunked() {
	std::string		header;
	std::string		received;
	std::string		body = "";
	int				size = 0;
	int				i;

	header = _request.substr(0, _request.find("\r\n\r\n") + 4);
	received = _request.substr(_request.find("\r\n\r\n") + 4);

	while (received.size()) {
		i = received.find("\r\n") + 2;
		//size = std::stoi(received.substr(0, i - 2), 0, 16);
		body += received.substr(i, i + size);
		received = received.substr(i + size + 2);
	}

	_request = header + body + "\r\n";
}

int	Server::sendResponse(std::map<int, std::string> errors) {
	
	// HttpResponse	response(_file_request, _status, requestInfos().getAutoIndex(), errors);
	// std::string		message = response.getResponse();
	// int	ret;

	// ret = write(_socket, message, message.size());
	// if (ret < 0) {
	// 	// Verifier si 0 succes ou erreur
	// 	// Gestion d'erreur
	// }

	// this->close_socket();
	// _request.erase();
	// _file_request.erase();

	return (0);
}

ServerInfo	Server::requestInfos() {
	std::string	serv_name;
	int			found;

	found = _request.find("HOST:");
	if (found == std::string::npos)
		found = _request.find("Host:");

	serv_name = _request.substr(found + 5);
	if (serv_name.at(0) == ' ')
		serv_name.erase(0, 1);
	serv_name = serv_name.substr(0, serv_name.find("\r\n"));

	for (std::vector<ServerInfo *>::iterator it = _infos.begin(); it != _infos.end(); it++) {
		for (std::vector<std::string>::iterator name = (*it)->getServerNames().begin(); name != (*it)->getServerNames().end(); name++) {
			if (serv_name == *name)
			return *(*it);
		}
	}
	return *_default;
}

std::string	Server::getError(){
	return _error;
}

std::vector<ServerInfo *>	Server::getInfos() const {
	return _infos;
}

std::ostream	&operator<<(std::ostream &x, Server const & serv)
{
	int	count = 1;

	x << "Socket: " << serv.getSocket() << std::endl;
	for (std::vector<ServerInfo *>::iterator it = serv.getInfos().begin(); count <= serv.getInfos().size(); it++) {
		x << "ServerInfo n°" << count++ << std::endl << (*it)->getIp() << std::endl; 
	}
	x << std::endl;
	return (x);
}
