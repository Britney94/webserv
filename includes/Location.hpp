#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "webserv.hpp"

class Location
{
	public:
		std::string	uri; 
		std::string	root;
		std::string	index;
		std::string	cgi;
		int	        allow[3]; // GET POST DELETE: 0 si interdit, 1 si autorise
		long		clientSize;
		std::vector<Location>	loc;

		Location();
		Location(const Location &copy);
		~Location();

		Location& operator=(const Location &copy);
};

#endif