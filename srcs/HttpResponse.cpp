#include "../includes/webserv.hpp"

HttpResponse::HttpResponse() {
	_code[200] = "OK";
	_code[201] = "Created";
	_code[204] = "No Content";
	_code[400] = "Bad Request";
	_code[403] = "Forbidden";
	_code[404] = "Not Found";
	_code[405] = "Method Not Allowed";
	_code[413] = "Payload Too Large";
	_code[500] = "Internal Server Error";
	this->_autoindex = 0;
	return ;
}

void	HttpResponse::setStatus(int status) {
	_status = status;
	return ;
}

void	HttpResponse::setMethod(std::string method) {
	_method = method;
	return ;
}

void	HttpResponse::setClientBody(std::string clientBody) {
	_clientBody = clientBody;
	return ;
}

void	HttpResponse::setErrorFiles(std::map<int, std::string> errorFiles) {
	_errorFiles = errorFiles;
	return ;
}

void	HttpResponse::setFile(std::string file) {
	_file = file;
	return ;
}

void	HttpResponse::setCGI(std::string cgi) {
	_cgi = cgi;
	return ;
}

void	HttpResponse::setQuery(std::string query) {
	_query = query;
	return ;
}

void	HttpResponse::setDir(std::string dir) {
	_dir = dir;
	return ;
}

void	HttpResponse::setHost(std::string host) {
	_host = host;
	return ;
}

void	HttpResponse::setAutoIndex(int autoindex) {
	this->_autoindex = autoindex;
	return ;
}

int	isFile(std::string file) {
	struct stat	st;
	if (stat(file.c_str(), &st) == 0) {
		if ((st.st_mode & S_IFMT) == S_IFREG)
			return 1;
	}
	return 0;
}

int	isDir(std::string file) {
	struct stat	st;
	if (stat(file.c_str(), &st) == 0) {
		if ((st.st_mode & S_IFMT) == S_IFDIR)
			return 1;
	}
	return 0;
}

/*
 * Check if the file is a CGI script
 */
static int  isCGIRequest(std::string file) {
    if (file.find(".cgi") == std::string::npos)
            return 0;
    std::ifstream infile(file.c_str());
    if (infile.good() == false)
        return 0;
    return 1;
}

int	HttpResponse::createResponse(char **envp) {
	std::ifstream   filestream;
	std::filebuf    filebuf;
	std::cout << std::endl << std::endl << "File to open: " << _file << std::endl;
	std::cout << "Status: " << _status << std::endl;
	std::cout << "Method: " << _method << std::endl;
	std::cout << "Body: " << _body << BLANK << std::endl;
	int isCGI = 0;
	std::string query;
	// Check the URI for a query string
	if (_file.find("?") != std::string::npos) {
	    query = _file.substr(_file.find("?") + 1);
        _file.erase(_file.find("?"));
    }
	if (_status >= 400 && _status < 500) {
		std::cout << RED << "Error file: " << _errorFiles[_status].c_str() << BLANK << std::endl;
		filestream.open(_errorFiles[_status].c_str());
		while(filestream.good()) {
			std::getline(filestream, this->_file_content);
			this->_body += this->_file_content;
			this->_body += '\n';
		}
		std::cout << RED << "Error file content: " << this->_file_content << BLANK << std::endl;
	}
	else if (_method == "GET") {
	    // Check if the file is a CGI script
		isCGI = isCGIRequest(_file);
		std::cout << isCGI << std::endl;
		// Execute the CGI script if it is
	    if (isCGI == 1) {
            CGI cgi;
            if (query.size() == 0)
                cgi.setBody("?");
            else
                cgi.setBody(query);
            this->_body = cgi.execute(_file, envp);
			_status = 200;
	    }
	    // If this is a valid file, open it and read its content
		else if (isFile(_file)) {
			filestream.open(_file.c_str());
			if (filestream.is_open() == false) {
				_status = 403;
				filestream.open(_errorFiles[_status].c_str());
			}
			while(filestream.good()) {
				std::getline(filestream, this->_file_content);
				this->_body += this->_file_content;
				this->_body += '\n';
			}
			filestream.close();
		}
		// If this is a valid directory, create an index of its content
		else if (isDir(_file)) {
			AutoIndex   content;
			_body = content.renderPage(_dir, _file, _host);
		}
		// In other case, return a 404 status code
		else {
			_status = 404;
			filestream.open(_errorFiles[_status].c_str());
			while(filestream.good()) {
				std::getline(filestream, this->_file_content);
				this->_body += this->_file_content;
				this->_body += '\n';
			}
			filestream.close();
		}
	}
	else if (_method == "POST") {
		_status = 204;
		// Check if the file is a CGI script
		isCGI = isCGIRequest(_file);
		// Execute the CGI script if it is
	    if (isCGI == 1) {
            CGI cgi;
            cgi.setBody(_clientBody);
            this->_body = cgi.execute(_file, envp);
			_status = 200;
	    }
	    // In other case, return a 204 status code
        else {
			_status = 404;
			filestream.open(_errorFiles[_status].c_str());
			while(filestream.good()) {
				std::getline(filestream, this->_file_content);
				this->_body += this->_file_content;
				this->_body += '\n';
			}
			filestream.close();
		}
	}
	else if (_method == "DELETE") {
		if (isFile(_file)) {
			if (remove(_file.c_str()) == 0)
				_status = 204;
			else 
				_status = 403;
		}
		else
			_status = 404;
		if (_status != 204) {
			filestream.open(_errorFiles[_status].c_str());
			while(filestream.good()) {
				std::getline(filestream, this->_file_content);
				this->_body += this->_file_content;
			}
			filestream.close();
		}
	}
	_header = "HTTP/1.1 " + toString(_status) + " ";
	_header += _code[_status] + "\r\n";
	if (_body.size() != 0) {
		_header += "Content-Length: " + toString(_body.size()) + "\r\n";
		if (isCGI == 1)
		    _header += "Content-Type: text/html\r\n";
		_header += "\r\n" + _body + "\r\n";
	}
	_response = _header;
	return 0;
}

HttpResponse::~HttpResponse(void) {
	return ;
}

std::string	HttpResponse::getResponse(void) const {
	return (this->_response);
}
