#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#pragma once

#include "webserv.hpp"

class AutoIndex {
	public:
		AutoIndex(){};
		std::string	renderPage(std::string directory, std::string path, std::string host);
		~AutoIndex(){};	
};

/* Fonction d'automatisation de l'indexing */

#endif
