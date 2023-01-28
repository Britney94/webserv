#ifndef CLIENT_REQUEST_HPP
#define CLIENT_REQUEST_HPP

#include "webserv.hpp"
#include "ServerInfo.hpp"

class	ClientRequest {

	public:
		ClientRequest(ServerInfo info, std::string request = "");
		~ClientRequest(void);

		int			checkSyntax();
		int			checkMethod();
		int			checkSize();
		int			isMethod(std::string word);
		int			determinateLoc();
		std::string	determinateFile();

		int			getStatus() const;
		std::string	getFile() const;
		std::string	getBody() const;
		std::string	getMethod() const;
		std::string	getCGI() const;

	private:
		ServerInfo	_info;
		Location	_loc;
		std::string	_request;
		std::string	_method;
		std::string	_uri;
		std::string	_file;
		std::string	_body;
		int			_status;

};

/* Function that manages client requests between the server and the browser */

#endif
