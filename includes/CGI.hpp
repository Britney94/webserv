#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"

/*
 * CGI class for executing CGI scripts
 */
class CGI {
	public:
		CGI();
		CGI(const CGI &src);
		CGI &operator=(const CGI &src);
		std::string	execute(const std::string& scriptName);
		void	setBody(std::string body);
		void	setScript(std::string cgi);
		void	setMethod(std::string method);
		void	setPath(std::string file);
		void	setType(std::string type);
		void	setQuery(std::string query);
		void	setAddr(std::string host);
		void	setPort(int	port);
		~CGI(){};
	private:
		std::string	_body;
		std::string	_cgi;
		std::string	_type;
		std::string	_method;
		std::string	_file;
		std::string	_query;
		std::string	_host;
		int _port;
		std::map<std::string, std::string>	_env;
		char    **_convertEnv() const;
};

#endif
