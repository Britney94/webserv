#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#pragma once

#include "webserv.hpp"

class autoindex{
	public:
		autoindex(){};
		std::string	renderPage(std::string directory, std::string path, int port, std::string host);
		~autoindex(){};	
};

#endif
