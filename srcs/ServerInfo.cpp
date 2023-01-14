#include "../includes/webserv.hpp"

ServerInfo::ServerInfo(void)
{
	// Default constructor arbitrary values set
	this->_ip = "0.0.0.0";
	this->_clientSize = 1000;
	this->_root = "./data/my_website/";
	this->_index = "index.html";
	this->_autoIndex = 0; 
	this->_allow[0] = 0;
	this->_allow[1] = 0;
	this->_allow[2] = 0;
	this->_allow[3] = 0;
	// this->_loc = NULL;
}

int	ServerInfo::setServerNames(std::string names)
{
	names.erase(0, names.find(' ') + 1);
	if (names.at(names.length() - 1) == '\n')
		names.erase(names.at(names.length() - 1));
	while (names.find(' ') != std::string::npos) {
		_serverNames.push_back(names.substr(0, names.find(' ')));
		names.erase(0, names.find(' ') + 1);
	}
	_serverNames.push_back(names);
	return 0;
}

int	ServerInfo::setIp(std::string line)
{
	line.erase(0, line.find(' ') + 1);
	if (line.at(line.length() - 1) == '\n')
		line.erase(line.at(line.length() - 1));
	line.erase(line.find(':'));
	if (line != "127.0.0.1" && line != "0.0.0.0") {
		std::cerr << "Error: Parsing configuration file : ip address" << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setRoot(std::string line) {
	line.erase(0, line.find(' ') + 1);
	if (line.at(line.length() - 1) == '\n')
		line.erase(line.at(line.length() - 1));
	_root = line;
	return 0;
}

int	ServerInfo::setIndex(std::string line) {
	line.erase(0, line.find(' ') + 1);
	if (line.at(line.length() - 1) == '\n')
		line.erase(line.at(line.length() - 1));
	_index = line;
	return 0;
}

int	ServerInfo::setAutoIndex(std::string line) {
	line.erase(0, line.find(' ') + 1);
	if (line.at(line.length() - 1) == '\n')
		line.erase(line.at(line.length() - 1));
	_autoIndex = (line == "on");
	if (line != "on" && line != "off") {
		std::cerr << "Error: Parsing configuration file : autoindex" << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setClientSize(std::string line)
{
	if (line.find(" ") != std::string::npos)
		this->_clientSize = atoi(&line[line.find(" ") + 1]);
	if (_clientSize == 0) {
		std::cerr << "Error: Parsing configuration file : clientSize" << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setAllow(std::string line)
{
	line.erase(0, line.find(' ') + 1);
	if (line.at(line.length() - 1) == '\n')
		line.erase(line.at(line.length() - 1));
	while (line.find(' ') != std::string::npos) {
		if (line.substr(0, line.find(' ')) == "GET")
			_allow[0] = 1;
		else if (line.substr(0, line.find(' ')) == "POST")
			_allow[1] = 1;
		else if (line.substr(0, line.find(' ')) == "DELETE")
			_allow[2] = 1;
		else if (line.substr(0, line.find(' ')) == "PUT")
			_allow[3] = 1;
		else {
			std::cerr << "Error: Parsing configuration file : allow_methods" << std::endl;
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
	else if (line.substr(0, line.find(' ')) == "PUT")
		_allow[3] = 1;
	else {
		std::cerr << "Error: Parsing configuration file : allow_methods" << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setLoc(Location& loc)
{
	this->_loc.push_back(loc);
	return 0;
}

std::vector<std::string> ServerInfo::getServerNames()
{
	return(this->_serverNames);
}

std::string ServerInfo::getIp()
{
	return (this->_ip);
}

std::string ServerInfo::getRoot()
{
	return (this->_root);
}

std::string ServerInfo::getIndex()
{
	return (this->_index);
}

long int	ServerInfo::getClientSize()
{
	return (this->_clientSize);
}

int	ServerInfo::getAutoIndex()
{
	return(this->_autoIndex);
}

std::vector<Location>	ServerInfo::getLoc() {
	return _loc;
}

int	ServerInfo::getAllow(std::string allow)
// GET POST DELETE : 0 si interdit, 1 si autorise, return -1 if not found
{
	if (allow == "GET")
		return (this->_allow[0]);
	if (allow == "POST")
		return (this->_allow[1]);
	if (allow == "DELETE")
		return (this->_allow[2]);
	if (allow == "PUT")
		return (this->_allow[3]);
	return (-1);
}

ServerInfo::~ServerInfo()
{
	this->_loc.clear();
}

std::ostream	&operator<<(std::ostream &x, std::vector<Location> loc)
{
	int	i = 0;
	while (i < loc.size())
	{
		Location	tmp = loc.at(i);
		x << "Location at[" << i << "] : ";
		x << tmp.uri << ", ";
		x << tmp.root << ", ";
		x << tmp.index;
		if (tmp.allow[0] != 0)
		{
			x << ", ";
			x << "GET ";
		}
		if (tmp.allow[1] != 0)
		{
			x << ", ";
			x << "POST ";
		}
		if (tmp.allow[2] != 0)
		{
			x << ", ";
			x << "DELETE ";
		}
		if (i < loc.size() - 1)
			x << std::endl;
		i++;
	}
	return (x);
}

std::ostream	&operator<<(std::ostream &x, ServerInfo inf)
{
	x << "**** ServerInfo ****" << std::endl;
	for (std::vector<std::string>::iterator it = inf.getServerNames().begin(); it != inf.getServerNames().end(); it++) {
		x << *it << ", ";
	}
	x << inf.getIp() << ", ";
	x << inf.getClientSize() << ", ";
	x << inf.getAutoIndex();
	if (inf.getAllow("GET") != 0)
	{
		x << ", ";
		x << "GET ";
	}
	if (inf.getAllow("POST") != 0)
	{
		x << ", ";
		x << "POST ";
	}
	if (inf.getAllow("DELETE") != 0)
	{
		x << ", ";
		x << "DELETE ";
	}
	x << std::endl;
	x << inf.getLoc();
	return (x);
}
