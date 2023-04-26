#include "../includes/webserv.hpp"

ServerInfo::ServerInfo(void) {
	this->_ip = "0.0.0.0";
	this->_clientSize = 10000;
	this->_root = "./www/";
	this->_index = "index.html";
	this->_autoIndex = 0; 
	this->_port = 0; 
	this->_allow[0] = 0;
	this->_allow[1] = 0;
	this->_allow[2] = 0;
}

ServerInfo::ServerInfo(ServerInfo* copy) {
	this->_allow[0] = copy->_allow[0];
	this->_allow[1] = copy->_allow[1];
	this->_allow[2] = copy->_allow[2];
	this->_index = copy->_index;
	this->_root = copy->_root;
	this->_clientSize = copy->_clientSize;
	this->_autoIndex = copy->_autoIndex;
	this->_serverNames = copy->_serverNames;
	for (std::vector<Location>::iterator it = copy->_loc.begin(); it != copy->_loc.end(); it++) {
		this->_loc.push_back(*it);
	}
	this->_ip = copy->_ip;
}

ServerInfo::ServerInfo(ServerInfo& copy) {
	this->_allow[0] = copy._allow[0];
	this->_allow[1] = copy._allow[1];
	this->_allow[2] = copy._allow[2];
	this->_index = copy._index;
	this->_root = copy._root;
	this->_clientSize = copy._clientSize;
	this->_autoIndex = copy._autoIndex;
	this->_serverNames = copy._serverNames;
	for (std::vector<Location>::iterator it = copy._loc.begin(); it != copy._loc.end(); it++) {
		this->_loc.push_back(*it);
	}
	this->_ip = copy._ip;
}

ServerInfo&	ServerInfo::operator=(ServerInfo& copy) {
	this->_allow[0] = copy._allow[0];
	this->_allow[1] = copy._allow[1];
	this->_allow[2] = copy._allow[2];
	this->_index = copy._index;
	this->_root = copy._root;
	this->_clientSize = copy._clientSize;
	this->_autoIndex = copy._autoIndex;
	this->_serverNames = copy._serverNames;
	for (std::vector<Location>::iterator it = copy._loc.begin(); it != copy._loc.end(); it++) {
		this->_loc.push_back(*it);
	}
	this->_ip = copy._ip;
	return *this;
}

int	ServerInfo::setServerNames(std::string names) {
	if (names.find(";") == std::string::npos)
		return 1;
	names.resize(names.find(";"));
	names.erase(0, names.find(' ') + 1);
	while (names.find(' ') != std::string::npos) {
		_serverNames.push_back(names.substr(0, names.find(' ')));
		names.erase(0, names.find(' ') + 1);
	}
	_serverNames.push_back(names);
	return 0;
}

int	ServerInfo::setIp(std::string line) {
	if (this->_port != 0) {
	std::cerr << RED << "Config file is incorrect: more than one listen directive." << BLANK << std::endl;
		return 2;
	}
    std::string port;
	if (line.find(";") == std::string::npos)
		return 1;
	line.resize(line.find(";"));
	line.erase(0, line.find(' ') + 1);
	line = trim(line);
	if (line.find("localhost") != std::string::npos)
		this->_ip = "127.0.0.1";
	else
		this->_ip = &line[line.find(" ") + 1];
	if (has(this->_ip, ":") >= 0) {
	    port = &this->_ip[this->_ip.find(":")];
		this->_ip.erase(this->_ip.find(":"));
	}
	int point = 0;
	int	i = 0;
	while(this->_ip[i]) {
		if (this->_ip[i] == '.')
			point++;
		if (point > 3)
			this->_ip[i] = '\0';
		i++;
	}
	if (this->_ip != "127.0.0.1" && this->_ip != "0.0.0.0")
		return 1;
	this->_ip += port;
	this->_port = atoi(&port[1]);
	return 0;
}

int	ServerInfo::setRoot(std::string line) {
	if (line.find(";") == std::string::npos)
		return 1;
	line.resize(line.find(";"));
	line.erase(0, line.find(' ') + 1);
	line = trim(line);
	if (line.size()) {
		this->_root = line;
		if (this->_root[this->_root.size() - 1] != '/')
			this->_root.push_back('/');
	}
	return 0;
}

int	ServerInfo::setIndex(std::string line) {
	if (line.find(";") == std::string::npos)
		return 1;
	line.resize(line.find(";"));
	line.erase(0, line.find(' ') + 1);
	if (line.at(line.length() - 1) == '\n')
		line.erase(line.at(line.length() - 1));
	_index = line;
	return 0;
}

int	ServerInfo::setAutoIndex(std::string line) {
	if (line.find(";") == std::string::npos)
		return 1;
	line.resize(line.find(";"));
	line.erase(0, line.find(' ') + 1);
	if (line.at(line.length() - 1) == '\n')
		line.erase(line.at(line.length() - 1));
	if (line == "on")
		_autoIndex = 1;
	if (line != "on" && line != "off") {
		std::cerr << RED << "Error: Parsing configuration file (autoindex)" << BLANK << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setClientSize(std::string line) {
	if (line.find(";") == std::string::npos)
		return 1;
	line.resize(line.find(";"));
	line = trim(line);
	std::string tmp = &line[line.find(" ") + 1];
	int i = 0;
	while (tmp[i]) {
		if (tmp[i] < '0' || tmp[i] > '9')
			return 1;
		i++;
	}
	if (line.find(" ") != std::string::npos)
		this->_clientSize = atoi(&line[line.find(" ")]);
	if (this->_clientSize <= 0){
		std::cerr << RED << "Error: Parsing configuration file (clientSize)" << BLANK << std::endl;
		return 1;
	}
	return 0;
}


int	ServerInfo::setAllow(std::string line) {
	if (line.find(";") == std::string::npos)
		return 1;
	line.resize(line.find(";"));
	line.erase(0, line.find(' ') + 1);
	line = trim(line);
	if (line.at(line.length() - 1) == '\n')
		line.erase(line.at(line.length() - 1));
	while (line.find(' ') != std::string::npos) {
		if (line.substr(0, line.find(' ')) == "GET")
			_allow[0] = 1;
		else if (line.substr(0, line.find(' ')) == "POST")
			_allow[1] = 1;
		else if (line.substr(0, line.find(' ')) == "DELETE")
			_allow[2] = 1;
		else {
			std::cerr << RED << "Error: Parsing configuration file (allow_methods)" << BLANK << std::endl;
			return 1;
		}
		line.erase(0, line.find(' ') + 1);
	}
	if (line.substr(0, line.find(' ')) == "GET")
		_allow[0] = 1;
	else if (line.substr(0, line.find(' ')) == "POST")
		_allow[1] = 1;
	else if (line.substr(0, line.find(' ')) == "DELETE")
		_allow[2] = 1;
	else {
		std::cerr << RED << "Error: Parsing configuration file (allow_methods)" << BLANK << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setLoc(Location& loc) {
	if (loc.allow[0] == -999)
		return (-999);
	this->_loc.push_back(loc);
	return 0;
}

std::vector<std::string> ServerInfo::getServerNames() const {
	return(this->_serverNames);
}

std::string ServerInfo::getIp() const {
	return (this->_ip);
}

int ServerInfo::getPort() const {
    return (this->_port);
}

std::string ServerInfo::getRoot() const {
	return (this->_root);
}

std::string ServerInfo::getIndex() const {
	return (this->_index);
}

long int	ServerInfo::getClientSize() const {
	return (this->_clientSize);
}

bool	ServerInfo::getAutoIndex() const {
	return(this->_autoIndex);
}

std::vector<Location>	ServerInfo::getLoc() const {
	return _loc;
}

int	ServerInfo::getAllow(std::string allow) const {
    // GET POST DELETE : 0 si interdit, 1 si autorise, return -1 if not found
    if (allow == "GET")
		return (this->_allow[0]);
	if (allow == "POST")
		return (this->_allow[1]);
	if (allow == "DELETE")
		return (this->_allow[2]);
	return (-1);
}

ServerInfo::~ServerInfo()
{
	this->_serverNames.clear();
	this->_loc.clear();
}