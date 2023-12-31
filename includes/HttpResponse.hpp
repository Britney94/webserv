#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#pragma once

#include "webserv.hpp"

class	HttpResponse {
	public:
		HttpResponse();
		~HttpResponse();
		int	createResponse(char **envp);
		void	setFile(std::string file);
		void	setStatus(int status);
		void	setAutoIndex(int autoindex);
		void	setDir(std::string dir);
		void	setHost(std::string host);
		void	setCGI(std::string cgi);
		void	setQuery(std::string query);
		void	setMethod(std::string method);
		void	setClientBody(std::string clientBody);
		void	setErrorFiles(std::map<int, std::string> errorFiles);
		void    setContentType(std::string contentType);
		void    setContentLength(std::string contentLength);
		void    setPathTranslated(std::string pathTranslated);
		void    setBoundary(std::string boundary);
		std::string	getResponse() const;
	private:
		int _status;
		int _autoindex;
		std::string	_cgi;
		std::string	_query;
		std::string	_file;
		std::string	_host;
		std::string	_dir;
        std::string _method;
        std::string _clientBody;
        std::map<int, std::string>  _errorFiles;
        std::map<int, std::string>  _code;
        std::string _header;
        std::string _body;
        std::string _response;
        std::string _file_content;
        std::string _contentType;
        std::string _contentLength;
        std::string _pathTranslated;
        std::string _boundary;
};

/* Function that allows to manage response query */

#endif
