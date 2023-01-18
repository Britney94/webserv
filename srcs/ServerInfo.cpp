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

ServerInfo::ServerInfo(ServerInfo* copy) {
	this->_allow[0] = copy->_allow[0];
	this->_allow[1] = copy->_allow[1];
	this->_allow[2] = copy->_allow[2];
	this->_allow[3] = copy->_allow[3];
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
	this->_allow[3] = copy._allow[3];
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
	this->_allow[3] = copy._allow[3];
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


int	ServerInfo::setServerNames(std::string names)
{
	names.erase(0, names.find(' ') + 1);
	while (names.find(' ') != std::string::npos) {
		_serverNames.push_back(names.substr(0, names.find(' ')));
		names.erase(0, names.find(' ') + 1);
	}
	_serverNames.push_back(names);
	return 0;
}

int	ServerInfo::setIp(std::string line)
{
	if (line.find("localhost") != std::string::npos)
		this->_ip = "127.0.0.1";
	else
		if (line.find(" ") != std::string::npos)
			this->_ip = &line[line.find(" ") + 1];
	if (this->_ip.find(":") != std::string::npos)
		this->_ip.erase(this->_ip.find(":"));
	int point = 0;
	int	i = 0;
	while(this->_ip[i])
	{
		if (this->_ip[i] == '.')
			point++;
		if (point > 3)
			this->_ip[i] = '\0';
		i++;
	}
	if (this->_ip != "127.0.0.1" && this->_ip != "0.0.0.0")
	{
		std::cerr << "Error: Parsing configuration file (ip address)" << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setRoot(std::string line) {
	if (line.find(" ") != std::string::npos)
	{
		this->_root = &line[line.find(" ") + 1];
		if (this->_root[this->_root.size() - 1] != '/')
			this->_root.push_back('/');
	}
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
		std::cerr << "Error: Parsing configuration file (autoindex)" << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setClientSize(std::string line)
{
	std::string tmp = &line[line.find(" ") + 1];
	int i = 0;
	while (tmp[i])
	{
		if (tmp[i] < '0' || tmp[i] > '9')
			return 1;
		i++;
	}
	if (line.find(" ") != std::string::npos)
		this->_clientSize = atoi(&line[line.find(" ")]);
	if (this->_clientSize == 0){
		std::cerr << "Error: Parsing configuration file (clientSize)" << std::endl;
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
			std::cerr << "Error: Parsing configuration file (allow_methods)" << std::endl;
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
		std::cerr << "Error: Parsing configuration file (allow_methods)" << std::endl;
		return 1;
	}
	return 0;
}

int	ServerInfo::setLoc(Location& loc)
{
	this->_loc.push_back(loc);
	return 0;
}

std::vector<std::string> ServerInfo::getServerNames() const
{
	return(this->_serverNames);
}

std::string ServerInfo::getIp() const
{
	return (this->_ip);
}

std::string ServerInfo::getRoot() const
{
	return (this->_root);
}

std::string ServerInfo::getIndex() const
{
	return (this->_index);
}

long int	ServerInfo::getClientSize() const
{
	return (this->_clientSize);
}

int	ServerInfo::getAutoIndex() const
{
	return(this->_autoIndex);
}

std::vector<Location>	ServerInfo::getLoc() const {
	return _loc;
}

int	ServerInfo::getAllow(std::string allow) const
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
	this->_serverNames.clear();
}

std::ostream	&operator<<(std::ostream &x, Location const & inf)
{
	x << "URI: " << inf.uri << std::endl;
	x << "Root: " << inf.root << std::endl;
	x << "Index: " << inf.index << std::endl;
	x << "ClientSize: " << inf.clientSize << std::endl;
	x << "CGI: " << inf.cgi << std::endl;
	x << "Allow_methods: " << inf.allow[0] << inf.allow[1] << inf.allow[2] << inf.allow[3] << std::endl;
	for (std::vector<Location>::const_iterator in = inf.loc.begin(); in != inf.loc.end(); in++) {
		x << *in << std::endl;
	}
	x << std::endl;
	return (x);
}

std::ostream	&operator<<(std::ostream &x, ServerInfo const & inf)
{
	x << "Server_names: ";
	std::cout << "Server_names: ";
	std::vector<std::string>::iterator name;
	for (name = inf.getServerNames().begin(); name != inf.getServerNames().end(); name++) {
		x << *name << " ";
		std::cout << *name << " ";
	}
	x << std::endl;
	x << "Ip address: " << inf.getIp() << std::endl;
	x << "Root: " << inf.getRoot() << std::endl;
	x << "Index: " << inf.getIndex() << std::endl;
	x << "Allow: " << inf.getAllow("GET") << inf.getAllow("POST") << inf.getAllow("DELETE") << inf.getAllow("PUT") << std::endl;
	x << "ClientSize: " << inf.getClientSize() << std::endl;
	x << "Autoindex: " << inf.getAutoIndex() << std::endl;
	std::vector<Location>::iterator loc;
	for (loc = inf.getLoc().begin(); loc != inf.getLoc().end(); loc++) {
		x << *loc << std::endl;
	}
	x << std::endl;
	return (x);
}
