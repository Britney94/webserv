#ifndef FILE_HPP
#define FILE_HPP

#pragma once

#include "webserv.hpp"

using namespace std;

class File {
	public:
		File(char *filename);
		std::string	getName();
		int	getMaxLine();
		int	getEnd();
		std::string	getLine();
		~File(){};
		std::string name;
		vector<std::string>	content;
		int	lineHistory;
		int	maxLine;
		int	end;
		int	cantOpen;
};

#endif
