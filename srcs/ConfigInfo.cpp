#include "../includes/webserv.hpp"

ConfigInfo::ConfigInfo(){
	this->_maxFd = 0;
	this->_err = 0;
	this->setErrorFiles();
}

ConfigInfo::ConfigInfo(ConfigInfo& copy){
	(void)copy;
	this->_err = 0;
	this->_maxFd = 0;
	this->setSize(0);
	this->setErrorFiles();
}

ConfigInfo::ConfigInfo(char *filename){
	this->_err = 0;
	this->setSize(0);
	this->_maxFd = 0;
	this->setErrorFiles();
	this->_servers = this->parse(filename);
}

static int checkBrackets(char *filename){
	File	file(filename);
	std::string	line;
	int		i = 0;
	int		j = 0;
	while (file.lineHistory < file.getMaxLine()){
		line = file.getLine();
		i = 0;
		while (line[i] != '\0'){
			if (line[i] == '{')
				j++;
			else if (line[i] == '}')
				j--;
			i++;
		}
	}
	if (j != 0)
		return 1;
	return 0;
}

std::map<int, Server *>	ConfigInfo::parse(char *filename){
	File		file(filename);
	int			ret = 0;
	size_t		count = 0;
	std::vector<ServerInfo *>	tmp;

	if (file.cantOpen == 1)
	{
		_err = 1;
		return _servers;
	}
	if (checkBrackets(filename) == 1) {
		std::cerr << RED << "Config file is incorrect: no end brackets" << BLANK << std::endl;
		_err = 1;
		return _servers;
	}
	std::string	line = file.getLine();
	while (file.lineHistory < file.getMaxLine()){
		if (line.find("server {") != std::string::npos){
			ServerInfo	*tmpInfo = new ServerInfo();
			tmp.push_back(tmpInfo);
			line = file.getLine();
			while (line.find("server {") == std::string::npos && file.lineHistory < file.getMaxLine()) {
				if (line.find("server_name ") != std::string::npos)
					ret = (*tmpInfo).setServerNames(line);
				else if (line.find("listen ") != std::string::npos){
					int	port;
					if (line.find(":") != std::string::npos) {
						port = atoi(&line[line.find(":") + 1]);
						ret = (*tmpInfo).setIp(line);
					}
					else if (line.find('.') == std::string::npos)
						port = atoi(&line[line.find(" ") + 1]);
					else {
						std::cerr << RED << "Error: Parsing configuration file : port" << BLANK << std::endl;
						_err = 1;
						if (tmp.size() != count) {
							delete tmp.at(tmp.size() - 1);
						}
						return _servers;
					}
					try {
						_servers.at(port)->addNewInfo(tmpInfo);
						count++;
					}
					catch (std::out_of_range& e) {
						Server	*new_server = new Server(tmpInfo, port);

						_servers.insert(std::make_pair(port, new_server));
						count++;
						if (new_server->getError() == 1) {
							_err = 1;
							return _servers;
						}
					}
				}
				else if (line.find("root ") != std::string::npos)
					ret = (*tmpInfo).setRoot(line);
				else if (line.find("index ") != std::string::npos)
					ret = (*tmpInfo).setIndex(line);
				else if (line.find("client_body_buffer_size ") != std::string::npos)
					ret = (*tmpInfo).setClientSize(line);
				else if (line.find("allow_methods ") != std::string::npos)
					ret = (*tmpInfo).setAllow(line);
				else if (line.find("location ") != std::string::npos)
				{
					ret = (*tmpInfo).setLoc(setupLoc(file, line));
					if (_err == 1)
					{
						std::cerr << RED << "Config file is incorrect: syntax error(s)" << BLANK << std::endl;
						if (tmp.size() != count) {
							delete tmp.at(tmp.size() - 1);
						}
						return _servers;
					}
				}
				else if (line.find("autoindex ") != std::string::npos)
					ret = (*tmpInfo).setAutoIndex(line);
				else if (line.size() != 0 && line != "}") {
					std::cerr << RED << "Config file is incorrect: unknown directive: " << line << BLANK << std::endl;
					_err = 1;
					if (tmp.size() != count) {
						delete tmp.at(tmp.size() - 1);
					}
					return _servers;
				}
				if (ret) {
					std::cerr << RED << "Config file is incorrect: syntax error(s)" << BLANK << std::endl;
					_err = 1;
					if (tmp.size() != count) {
						delete tmp.at(tmp.size() - 1);
					}
					return _servers;
				}
				line = file.getLine(); 
			}
			if (tmp.size() != count) {
				delete tmp.at(tmp.size() - 1);
			}
		}
		else if (line.find("error_page ") != std::string::npos) {
			if (setErrorFile(line)) {
				std::cerr << RED << "Config file is incorrect: error_page" << BLANK << std::endl;
				_err = 1;
				if (tmp.size() != count) {
					delete tmp.at(tmp.size() - 1);
				}
				return _servers;
			}
			line = file.getLine();
		}
		else if (line.size() == 0) {
			line = file.getLine();
		}
		else {
			std::cerr << RED << "Error: Parsing configuration file : unknown directive : " << line << BLANK << std::endl;
			_err = 1;
			if (tmp.size() != count) {
				delete tmp.at(tmp.size() - 1);
			}
			return _servers;
		}
	}
	this->_size = _servers.size();
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		if (it->second->getSocket() > _maxFd)
			_maxFd = it->second->getSocket();
		
		for (size_t count = 0; count < it->second->getInfos().size(); count++) {
			if (it->second->getInfos().at(count)->getAllow("GET") == 0 &&
				it->second->getInfos().at(count)->getAllow("POST") == 0 &&
				it->second->getInfos().at(count)->getAllow("DELETE") == 0)
				it->second->getInfos().at(count)->setAllow("allow_methods GET POST DELETE");
		}
	}
	return (_servers);
}

Location&	ConfigInfo::setupLoc(File& file, std::string curr_line) {
	Location	tmp;
	std::string	line = file.getLine();

	tmp.uri = curr_line.substr(curr_line.find("location ") + 9, curr_line.find("{") - (curr_line.find("location ") + 9));
	while (tmp.uri.at(tmp.uri.length() - 1) == ' ')
		tmp.uri.erase(tmp.uri.length() - 1);
	if (tmp.uri.find(".") == std::string::npos && tmp.uri.at(tmp.uri.length() - 1) != '/')
		tmp.uri += "/";
	if (tmp.uri.find("*") != std::string::npos)
		tmp.uri.erase(0, 1);
	tmp.allow[0] = 0;
	tmp.allow[1] = 0;
	tmp.allow[2] = 0;
	tmp.cgi = "";
	tmp.clientSize = -1;
	tmp.index = "";
	tmp.root = "";

	while (line.find("}") == std::string::npos) {
		if (line.find(";") == std::string::npos && line.find("{") == std::string::npos && trim(line) != "") {
			this->_err = 1;
			_tmp_loc = tmp;
			return _tmp_loc;
		}
		if (line.find("location ") != std::string::npos)
			tmp.loc.push_back(setupLoc(file, line));
		else if (line.find("root ") != std::string::npos) {
			tmp.root = line.substr(line.find(" ") + 1);
			if (tmp.root.at(tmp.root.length() - 1) != '/')
				tmp.root += "/";
		}
		else if (line.find("index ") != std::string::npos)
			tmp.index = line.substr(line.find(" ") + 1);
		else if (line.find("cgi_pass ") != std::string::npos)
			tmp.cgi = line.substr(line.find(" ") + 1);
		else if (line.find("client_body_buffer_size ") != std::string::npos)
		{
			tmp.clientSize = atoi(&(line.substr(line.find(" ") + 1))[0]);
			if (tmp.clientSize <= 0){
				this->_err = 1;
				_tmp_loc = tmp;
				return _tmp_loc;
			}
		}
		else if (line.find("allow_methods ") != std::string::npos) {
			if (line.find("GET") != std::string::npos)
				tmp.allow[0] = 1;
			if (line.find("POST") != std::string::npos)
				tmp.allow[1] = 1;
			if (line.find("DELETE") != std::string::npos)
				tmp.allow[2] = 1;
		}
		line = file.getLine();
	}
	_tmp_loc = tmp;
	return _tmp_loc;
}

int	ConfigInfo::setErrorFile(std::string line) {
	int error = atoi(&(line.substr(line.find(" ") + 1))[0]);

	if (error < 400 || error >= 600)
		return 1;
	else {
		line.erase(0, line.find_last_of(" ") + 1);
		_errorFiles[error] = line;
	}
	return 0;
}

void	ConfigInfo::setErrorFiles(){
	_errorFiles[400] = "./data/error_files/400.html";
	_errorFiles[403] = "./data/error_files/403.html";
	_errorFiles[404] = "./data/error_files/404.html";
	_errorFiles[405] = "./data/error_files/405.html";
	_errorFiles[413] = "./data/error_files/413.html";
	_errorFiles[500] = "./data/error_files/500.html";
}

void ConfigInfo::setSize(int size){
		this->_size = size;
}

int ConfigInfo::getError() const {
		return (this->_err);
}

std::map<int, std::string>	ConfigInfo::getErrors() const {
		return (this->_errorFiles);
}

std::map<int, Server *>	ConfigInfo::getServers() const {
	return _servers;
}


int ConfigInfo::getSize() const {
		return (this->_size);
}

int ConfigInfo::getMaxFd() const {
    return (this->_maxFd);
}

ConfigInfo::~ConfigInfo(){
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		it->second->close_socket();
		if (it->second != NULL)
			delete (it->second);
	}
	_errorFiles.clear();
	return ;
}

std::ostream	&operator<<(std::ostream &x, ConfigInfo const & inf)
{
	int	count = 1;

	x << "**** ConfigInfo ****" << BLANK << std::endl << std::endl;
	std::map<int, Server *>::const_iterator it;
	for (it = inf.getServers().begin(); count <= inf.getSize(); it++) {
		x << "*** Server n°" << count++ << std::endl << *(it->second) << std::endl;
	}
	return (x);
}
