#include "../includes/webserv.hpp"

ConfigInfo::ConfigInfo() {
	this->_maxFd = 0;
	this->_err = 0;
	this->setErrorFiles();
}

ConfigInfo::ConfigInfo(ConfigInfo& copy) {
	(void)copy;
	this->_err = 0;
	this->_maxFd = 0;
	this->setSize(0);
	this->setErrorFiles();
}

ConfigInfo::ConfigInfo(char *filename) {
	this->_err = 0;
	this->setSize(0);
	this->_maxFd = 0;
	this->setErrorFiles();
	this->_servers = this->parse(filename);
}

static int checkBrackets(char *filename) {
	File	file(filename);
	std::string	line;
	int		i = 0;
	int		j = 0;
	while (file.lineHistory < file.getMaxLine()) {
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

std::map<int, Server *>	ConfigInfo::parse(char *filename) {
	File    file(filename);
	int ret = 0;
	size_t  count = 0;
	std::vector<ServerInfo *>   tmp;
	if (file.cantOpen == 1) {
		_err = 1;
		return _servers;
	}
	if (checkBrackets(filename) == 1) {
		std::cerr << RED << "Config file is incorrect: no end brackets" << BLANK << std::endl, _err = 1;
		return _servers;
	}
	std::string	line = file.getLine();
	while (file.lineHistory < file.getMaxLine()) {
		if (has(line, "server {") >= 0) {
			ServerInfo	*tmpInfo = new ServerInfo();
			tmp.push_back(tmpInfo);
			line = file.getLine();
			while (line.find("server {") == std::string::npos && file.lineHistory < file.getMaxLine()) {
				if (has(line, "autoindex ") >= 0)
                	ret = (*tmpInfo).setAutoIndex(line);
				else if (has(line, "server_name ") >= 0)
					ret = (*tmpInfo).setServerNames(line);
				else if (has(line, "listen ") >= 0) {
					int	port;
					if (has(line, ":") >= 0) {
						port = atoi(&line[line.find(":") + 1]);
						ret = (*tmpInfo).setIp(line);
						if (ret == 2) {
							_err = 1;
							return _servers;
						}
					}
					else if (has(line, ".") >= 0)
						port = atoi(&line[line.find(" ") + 1]);
					else {
						std::cerr << RED << "Error: Parsing configuration file : port" << BLANK << std::endl, _err = 1;
						if (tmp.size() != count) {
							delete tmp.at(tmp.size() - 1);
							tmp.clear();
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
				else if (has(line, "root ") >= 0)
					ret = (*tmpInfo).setRoot(line);
				else if (has(line, "index ") >= 0)
					ret = (*tmpInfo).setIndex(line);
				else if (has(line, "client_body_buffer_size ") >= 0)
					ret = (*tmpInfo).setClientSize(line);
				else if (has(line, "allow_methods ") >= 0)
					ret = (*tmpInfo).setAllow(line);
				else if (has(line, "location ") >= 0) {
					ret = (*tmpInfo).setLoc(setupLoc(file, line));
					if (_err == 1) {
						std::cerr << RED << "Config file is incorrect: syntax error(s)" << BLANK << std::endl;
						if (tmp.size() != count) {
							delete tmp.at(tmp.size() - 1);
							tmp.clear();
						}
						return _servers;
					}
				}
				if (ret) {
					std::cerr << RED << "Config file is incorrect: syntax error(s)" << BLANK << std::endl, _err = 1;
					if (tmp.size() != count) {
						delete tmp.at(tmp.size() - 1);
						tmp.clear();
					}
					return _servers;
				}
				line = file.getLine();
			}
			if (tmp.size() != count) {
				delete tmp.at(tmp.size() - 1);
				tmp.clear();
			}
		}
		else if (has(line, "upload_directory ") >= 0) {
			if (line.find(";") != std::string::npos)
				line.resize(line.find(";"));
			line.erase(0, line.find(' ') + 1);
			line = trim(line);
			if (line.size()) {
				_upload = line;
				if (_upload[_upload.size() - 1] != '/')
					_upload.push_back('/');
			}
			line = file.getLine();
		}
		else if (has(line, "error_page ") >= 0) {
			if (setErrorFile(line)) {
				std::cerr << RED << "Config file is incorrect: error_page" << BLANK << std::endl, _err = 1;
				if (tmp.size() != count) {
					delete tmp.at(tmp.size() - 1);
					tmp.clear();
				}
				return _servers;
			}
			line = file.getLine();
		}
		else if (line.size() == 0)
			line = file.getLine();
		else {
			std::cerr << RED << "Error: Parsing configuration file : unknown directive : " << line << BLANK << std::endl, _err = 1;
			if (tmp.size() != count) {
				delete tmp.at(tmp.size() - 1);
				tmp.clear();
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
				it->second->getInfos().at(count)->setAllow("allow_methods GET POST DELETE;");
		}
	}
	return (_servers);
}

Location&	ConfigInfo::setupLoc(File& file, std::string curr_line) {
	Location	tmp;
	std::string	line = file.getLine();
	tmp.uri = curr_line.substr(curr_line.find("location ") + 9, curr_line.find("{") - (curr_line.find("location ") + 9));
	while (tmp.uri.at(tmp.uri.length() - 1) == ' ')
	{
		tmp.uri.erase(tmp.uri.length() - 1);
	}
	if (has(tmp.uri, ".") >= 0 && tmp.uri.at(tmp.uri.length() - 1) != '/')
		tmp.uri += "/";
	if (has(tmp.uri, "*") >= 0)
		tmp.uri.erase(0, 1);
	tmp.allow[0] = 0;
	tmp.allow[1] = 0;
	tmp.allow[2] = 0;
	tmp.cgi = "off";
	tmp.clientSize = -1;
	tmp.index = "";
	tmp.root = "";
	while (has(line, "}") == -1) {
		if (has(line, ";") == -1 && has(line, "{") == -1 && trim(line) != "") {
			this->_err = 1;
			_tmp_loc = tmp;
			return _tmp_loc;
		}
		std::cout << "P3 setupLoc Line = [" << line << "]\n";	//DEBUG
		if (line.find(';') != std::string::npos)	// Protects erase if location was inside location
			line.erase(line.find(';'));		// ABORT CORE DUMP //#2 RESOLVED
		std::cout << "P4 setupLoc Line = [" << line << "]\n";	//DEBUG
		if (has(line, "location ") >= 0)
			tmp.loc.push_back(setupLoc(file, line));
		else if (has(line, "root ") >= 0) {
			tmp.root = line.substr(line.find(" ") + 1);
			if (tmp.root.at(tmp.root.length() - 1) != '/')
				tmp.root += "/";
		}
		else if (has(line, "index ") >= 0)
			tmp.index = line.substr(line.find(" ") + 1);
		else if (has(line, "cgi_pass ") >= 0) {
			tmp.cgi = line.substr(line.find(" ") + 1);
			if (tmp.cgi != "on") {
				std::cout << RED << "Please limit CGI configuration to \"on\" or \"off\"." << BLANK << std::endl;
				tmp.cgi = "off";
			}
		}
		else if (has(line, "client_body_buffer_size ") >= 0) {
			tmp.clientSize = atoi(&(line.substr(has(line, " ") + 1))[0]);
			if (tmp.clientSize <= 0) {
				this->_err = 1;
				_tmp_loc = tmp;
				return _tmp_loc;
			}
		}
		else if (has(line, "allow_methods ") >= 0) {
			if (has(line, "GET") >= 0)
				tmp.allow[0] = 1;
			if (has(line, "POST") >= 0)
				tmp.allow[1] = 1;
			if (has(line, "DELETE") >= 0)
				tmp.allow[2] = 1;
		}
		line = file.getLine();
	}
	_tmp_loc = tmp;
	return _tmp_loc;
}

int	ConfigInfo::setErrorFile(std::string line) {
	int error = atoi(&(line.substr(has(line, " ") + 1))[0]);
	if (error < 400 || error >= 600)
		return 1;
	else {
		line.erase(0, line.find_last_of(" ") + 1);
		if (line.find(';') != std::string::npos)
			line.erase(line.find(';'));
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

void ConfigInfo::setSize(int size) {
	this->_size = size;
}

int ConfigInfo::getError() const {
	return (this->_err);
}

std::string	ConfigInfo::getUpload() const {
	return (this->_upload);
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

ConfigInfo::~ConfigInfo() {
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		it->second->close_socket();
		if (it->second != NULL)
			delete (it->second);
	}
	_servers.clear();
	_errorFiles.clear();
	return ;
}
