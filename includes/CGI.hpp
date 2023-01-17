#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"

class CGI{
	public:
		CGI();
		CGI(const CGI &src);
		CGI &operator=(const CGI &src);
		std::string	execute(const std::string& scriptName);
		~CGI(){};
	private:
		std::string	_body;
		std::map<std::string, std::string>	_env;
		void	_setEnv();
		char	**_convertEnv() const;
};

#endif
