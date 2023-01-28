#ifndef FILE_HPP
#define FILE_HPP

#pragma once

#include "webserv.hpp"

using namespace std;

class File {
	
	public:
		File(char *filename);
		~File(){};
	
		std::string	getName();
		std::string	getLine();
	
		int	getMaxLine();
		int	getEnd();
	
		vector<std::string>	content;
		
		std::string	name;
		int			lineHistory;
		int			maxLine;
		int			end;
		int			cantOpen;
};

/* Function that allows to manage files (get what is inside the file) */

#endif
