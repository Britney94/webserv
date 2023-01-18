#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#pragma once

#include "webserv.hpp"

class	HttpResponse {

	public:
		HttpResponse();
		~HttpResponse();
    int         createResponse();
		std::string	getResponse() const;

        void setFile(std::string file);
        void setStatus(int status);
        void setCGI(std::string cgi);
        void setMethod(std::string method);
        void setClientBody(std::string clientBody);
        void setErrorFiles(std::map<int, std::string> errorFiles);

	private:
		int			_status;
		std::string	_cgi;
		std::string	_file;
        std::string _method;
        std::string _clientBody;

        std::map<int, std::string>  _errorFiles;

        std::string _header;
        std::string _body;
        std::string _response;
        std::string _file_content;

};

#endif
