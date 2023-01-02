#include "../includes/webserv.hpp"

ConfigInfo::ConfigInfo(){
	this->setSize(0);
	this->_maxFd = 0;
}

ConfigInfo::ConfigInfo(ConfigInfo& copy){
	this->_maxFd = 0;
	this->setSize(0);
}

//ConfigInfo& ConfigInfo::operator=(ConfigInfo& copy){
//}

//std::map<int, Server>   parse(char *filename){
//}

void ConfigInfo::setSize(int size){
		this->_size = size;
}

std::string ConfigInfo::getError(){
		return (this->_error);
}

int ConfigInfo::getSize(){
		return (this->_size);
}

int ConfigInfo::getMaxFd(){
    return (this->_maxFd);
}

ConfigInfo::~ConfigInfo(){
}

std::ostream	&operator<<(std::ostream &x, ConfigInfo inf)
{
	x << "**** ConfigInfo ****" << std::endl;
	x << "Size : " << inf.getSize() << std::endl;
	x << "MaxFd : " << inf.getMaxFd() << std::endl;
	return (x);
}
