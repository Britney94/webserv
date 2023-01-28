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
		
		std::map<int, Server *>	parse(char *filename);
		
		void	setErrorFiles();
		int		setErrorFile(std::string line);
		void	setSize(int size);
		
		Location&	setupLoc(File& file, std::string curr_line);
	
		std::map<int, std::string>	getErrors() const;
		std::map<int, Server *>		getServers() const;
		
		int getError() const;
		int	getSize() const;
		int	getMaxFd() const;
	
	private:
		std::map<int, Server *>		_servers;
		std::map<int, std::string>	_errorFiles;
		
		int	_err;
		int	_maxFd;
		int	_size;

		Location	_tmp_loc;
};

/* Function that allows you to index the error logs on both the client and server side */

#endif
