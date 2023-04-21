#include "../includes/ClientRequest.hpp"

ClientRequest::ClientRequest(ServerInfo info, std::string request, int status) : _info(info), _request(request), _file(""), _status(status) {

    	std::cout << "Debug1\n";	// DEBUG
	checkSyntax();
    	std::cout << "Debug2\n";	// DEBUG
	determinateLoc();
    	std::cout << "Debug3\n";	// DEBUG
	checkMethod();
    	std::cout << "Debug4\n";	// DEBUG
	checkSize();
    	std::cout << "Debug5\n";	// DEBUG
	determinateFile();
    	std::cout << "Debug6\n";	// DEBUG
}

ClientRequest::~ClientRequest() {
	if (_info.getLoc().size() == 0)
		_info.getLoc().push_back(Location());
}

int	ClientRequest::checkMethod() {
	if (_method == "GET" && !_loc.allow[0])
		_status = 405;
	if (_method == "POST" && !_loc.allow[1])
		_status = 405;
	if (_method == "DELETE" && !_loc.allow[2])
		_status = 405;
	return _status;
}

int	ClientRequest::isMethod(std::string word) {
	if (word == "GET" || word == "POST" || word == "DELETE")
	    return 1;
	return 0;
}

int	ClientRequest::checkSyntax() {
	if (!_request.length() || (_request.find("Host:") == std::string::npos && _request.find("HOST:") == std::string::npos)) {
		_status = 400;
		return _status;
	}
	std::string	firstLine = _request.substr(0, _request.find("\r\n") + 2);
	std::string	word = firstLine.substr(0, firstLine.find(" "));
	for (int count = 1; count < 4; count++) {
		if (count == 1) {
			if (!isMethod(word)) {
				_status = 400;
			}
			_method = word;
		}
		if (count == 2) {
			_uri = word;
		}
		if (count == 3) {
			if (word != "HTTP/1.1\r\n") {
				_status = 400;
			}
		}
		firstLine.erase(0, word.length() + 1);
		word = firstLine.substr(0, firstLine.find(" "));
	}
	std::string copy_request = _request;
	std::string	bodyLine;
	std::string	header_field;
	std::string	value;
	int body_size = 0;
	copy_request.erase(0, copy_request.find("\r\n") + 2);
	bodyLine = copy_request.substr(0, copy_request.find("\r\n") + 2);
	while (bodyLine.length() && bodyLine != "\r\n") {
		if (bodyLine.find(":") == std::string::npos) {
			_status = 400;
			return _status;
		}
		header_field = bodyLine.substr(0, bodyLine.find(":"));
		value = bodyLine.substr(bodyLine.find(":") + 1);
		if (value.find("\r\n") == std::string::npos || !header_field.length() || !value.length()) {
			_status = 400;
			return _status;
		}
		if (header_field.find("Content-Length") != std::string::npos || header_field.find("CONTENT-LENGTH") != std::string::npos) {
			if (value.at(0) == ' ')
				value.erase(0, 1);
			body_size = atoi(&(value)[0]);
		}
		copy_request.erase(0, copy_request.find("\r\n") + 2);
		bodyLine = copy_request.substr(0, copy_request.find("\r\n") + 2);
	}
	if (!bodyLine.length())
		return _status;
	copy_request.erase(0, 2);
	_body = copy_request;
	if ((long)copy_request.size() != (long)body_size && _request.find("Content-Type: multipart") == std::string::npos) {
		_status = 400;
		return _status;
	}
	return _status;
}

int	ClientRequest::checkSize() {
	if ((long)_body.size() > (long)_loc.clientSize)
		_status = 413;
	return _status;
}

std::string	ClientRequest::determinateFile() {
	std::string	file_uri;
	if (this->_uri.empty() == true)
	{
    		std::cout << "Debug FILE  = [" << _loc.uri << "]\n";	// DEBUG
		file_uri = _uri;
	}
	else
		file_uri = _uri.substr(_loc.uri.length());
//	std::string	file_uri = _uri.substr(_loc.uri.length());	//ABORT 
	_file.insert(0, _loc.root);
	_file.insert(_file.size(), file_uri);
	if (_info.getAutoIndex() == 0 && _file.at(_file.length() - 1) == '/') {
		if (_loc.index.size() && _loc.index != "")
			_file += _loc.index;
		else
			_file.insert(_file.size(), "index.html");
	}
	return _file;
}

int	ClientRequest::determinateLoc() {
	std::string	ext;
	std::string	tmp_uri;
	std::vector<Location>	tmp_vec = _info.getLoc();
	_loc.root = _info.getRoot();
	_loc.index = _info.getIndex();
    	std::cout << "Debug7\n";	// DEBUG
	_loc.uri = "/";
	_loc.cgi = "off";
	_loc.allow[0] = _info.getAllow("GET");
	_loc.allow[1] = _info.getAllow("POST");
	_loc.allow[2] = _info.getAllow("DELETE");
    	std::cout << "Debug8\n";	// DEBUG
	_loc.clientSize = _info.getClientSize();
    	std::cout << "Debug9\n";	// DEBUG
	tmp_uri = _uri;
    	std::cout << "Debug9.1 URI = [" << tmp_uri << "]\n";	// DEBUG
	if (tmp_uri.empty() == true)
		return (this->_status);
	if (tmp_uri.find(".") != std::string::npos)
		ext = tmp_uri.substr(tmp_uri.find("."));
    	std::cout << "Debug10\n";	// DEBUG
	tmp_uri.erase(tmp_uri.find_last_of('/') + 1);
    	std::cout << "Debug11\n";	// DEBUG
	while (tmp_vec.size() != 0) {
		int	loop = 1;
		for (int count = 0; count < (int)tmp_vec.size() && loop; count++) {
    			std::cout << "Debug12\n";	// DEBUG
			Location tmp = tmp_vec.at(count);
			if (tmp_uri == tmp.uri || ext == tmp.uri) {
				_loc.uri = tmp_uri;
				if (tmp.root.size())
					_loc.root = tmp.root;
				if (tmp.index.size())
					_loc.index = tmp.index;
				if (tmp.clientSize != -1)
					_loc.clientSize = tmp.clientSize;
				if (tmp.cgi.size()) {
					_loc.cgi = tmp.cgi;
				}
				if (tmp.allow[0] || tmp.allow[1] || tmp.allow[2]) {
					_loc.allow[0] = tmp.allow[0];
					_loc.allow[1] = tmp.allow[1];
					_loc.allow[2] = tmp.allow[2];
				}
				if (ext == tmp.uri)
					return _status;
				tmp_vec = tmp.loc;
				loop = 0;
			}
		}
		if (tmp_uri == "/")
			return _status;
    		std::cout << "Debug13\n";	// DEBUG
		if (loop) {
			if (tmp_uri.size() != 0)
			{
    				std::cout << "Debug14 URI = [" << tmp_uri << "]\n";	// DEBUG
				tmp_uri.erase(tmp_uri.length() - 1);
    				std::cout << "Debug15 URI = [" << tmp_uri << "]\n";	// DEBUG
				tmp_uri.erase(tmp_uri.find_last_of('/') + 1);
			}
    			std::cout << "Debug16\n";	// DEBUG
		}
	}
	return _status;
}

int	ClientRequest::getStatus() const {
	return _status;
}

std::string	ClientRequest::getFile() const {
	return _file;
}

std::string	ClientRequest::getBody() const {
	return _body;
}

std::string	ClientRequest::getMethod() const {
	return _method;
}

std::string	ClientRequest::getCGI() const {
	return _loc.cgi;
}
