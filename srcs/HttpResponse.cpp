#include "../includes/webserv.hpp"

HttpResponse::HttpResponse() {
	_code[200] = "OK";
	_code[201] = "Created";
	_code[204] = "No Content";
	_code[301] = "Redirection";
	_code[400] = "Bad Request";
	_code[403] = "Forbidden";
	_code[404] = "Not Found";
	_code[405] = "Method Not Allowed";
	_code[413] = "Payload Too Large";
	_code[500] = "Internal Server Error";
	this->_autoindex = 0;
	this->_contentType = "";
	this->_boundary = "";
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

void    HttpResponse::setContentType(std::string contentType) {
    this->_contentType = contentType;
    return ;
}

void    HttpResponse::setContentLength(std::string contentLength) {
    this->_contentLength = contentLength;
    return ;
}

void    HttpResponse::setPathTranslated(std::string pathTranslated) {
    this->_pathTranslated = pathTranslated;
    return ;
}

void    HttpResponse::setBoundary(std::string boundary) {
    this->_boundary = boundary;
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

static std::string  postForm(std::string file, std::string body) {
    std::string newBody;
    std::ifstream infile;
    std::string fileContent;
    open(file.c_str(), O_RDWR | O_CREAT, 0777);
    infile.open(file.c_str());
    while (infile.good()) {
        std::getline(infile, fileContent);
        newBody += fileContent;
        newBody += '\n';
    }
    infile.close();
    std::string tmpNewBody;
    while (body.find("&") != std::string::npos || body.find("=") != std::string::npos) {
        std::string key = body.substr(0, body.find("="));
        body.erase(0, body.find("=") + 1);
        std::string value = body.substr(0, body.find("&"));
        body.erase(0, body.find("&") + 1);
        while (value.find("+") != std::string::npos)
            value.replace(value.find("+"), 1, " ");
        if (key.find("name") != std::string::npos)
            key = "Name";
        if (key.find("mail") != std::string::npos && value.find("%40") != std::string::npos)
            value.replace(value.find("%40"), 3, "@");
        if (key.find("mail") != std::string::npos)
            key = "Email";
        if (key.find("message") != std::string::npos)
            key = "Message";
        if (value != "")
            tmpNewBody += "<div><p>" + key + " <i>" + value + "</i></p></div>\n";
    }
    if (tmpNewBody != "") {
        newBody += "<div><h3>Post data</h3></div>\n";
        newBody += tmpNewBody;
    }
    std::ofstream outfile;
    outfile.open(file.c_str());
    outfile << newBody;
    return newBody;
}

int	HttpResponse::createResponse(char **envp) {
	std::ifstream   filestream;
	std::filebuf    filebuf;
	int isCGI = 0;
	std::string query;
	// Check the URI for a query string
	if (_file.find("?") != std::string::npos) {
	    query = _file.substr(_file.find("?") + 1);
        _file.erase(_file.find("?"));
    }
	if (_status >= 400 && _status < 500) {
		filestream.open(_errorFiles[_status].c_str());
		while(filestream.good()) {
			std::getline(filestream, this->_file_content);
			this->_body += this->_file_content;
			this->_body += '\n';
		}
	}
	else if (_method == "GET") {
	    // Check if the file is a CGI script
		isCGI = isCGIRequest(_file);
		// Execute the CGI script if it is
	    if (isCGI == 1 || _cgi == "on") {
            CGI cgi;
            if (query.size() == 0)
                cgi.setQuery("");
            else
                cgi.setQuery(query);
            this->_body = cgi.execute(_file, envp);
			if (this->_body == "Status: 500\r\n\r\n") {
				_status = 500;
				this->_body.clear();
				filestream.open(_errorFiles[_status].c_str());
				while(filestream.good()) {
					std::getline(filestream, this->_file_content);
					this->_body += this->_file_content;
					this->_body += '\n';
				}
				filestream.close();
			}
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
		// Check if the file is a CGI script
		isCGI = isCGIRequest(_file);
		// Execute the CGI script if it is
	    if (isCGI == 1 || _cgi == "on") {
			std::cout << "CGI PASS" << std::endl;
            CGI cgi;
            cgi.setBody(_clientBody);
            if (_contentType != "") {
                cgi.setContentType(_contentType);
                cgi.setContentLength(_contentLength);
                cgi.setPathTranslated(_pathTranslated);
                cgi.setBoundary(_boundary);
            }
            this->_body = cgi.execute(_file, envp);
			_status = 200;
	    }
	    else if (_clientBody.size() > 0 && _file.find("form.html") != std::string::npos) {
            this->_body = postForm(_file, _clientBody);
            if (this->_body == "")
                _status = 404;
            else
                _status = 200;
        }
        else if (_clientBody.size() > 0) {
		    _status = 204;
        	std::fstream    post_file;
            std::string path = "./tmp/" + _file.substr(_file.find_last_of("/") + 1);
        	open(path.c_str(), O_RDWR|O_CREAT, 0777);
        	post_file.open(path.c_str());
        	post_file << this->_clientBody;
		}
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
