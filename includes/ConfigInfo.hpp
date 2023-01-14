#ifndef CONFIGINFO_HPP
#define CONFIGINFO_HPP

#pragma once

#include "webserv.hpp"
#include "File.hpp"

class Server;

class ConfigInfo {
	
	public:
		
		ConfigInfo();
		ConfigInfo(ConfigInfo& copy);
		ConfigInfo(char *filename);
		~ConfigInfo();
		
		std::map<int, Server>	parse(char *filename);
		
		void	setErrorFiles();
		int		setErrorFile(std::string line);
		void	setSize(int size);
		
		std::map<int, std::string>	getErrors() const;
		int getError() const;
		int	getSize() const;
		int	getMaxFd() const;
		
		Location&	setupLoc(File& file, std::string curr_line);
	
	private:
		std::map<int, Server>		_servers;
		std::map<int, std::string>	_errorFiles;
		
		int	_err;
		int	_maxFd;
		int	_size;

		Location	_tmp_loc;
};

std::ostream	&operator<<(std::ostream &x, ConfigInfo const & inf);

#endif
