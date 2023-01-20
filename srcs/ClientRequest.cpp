#include "../includes/ClientRequest.hpp"

ClientRequest::ClientRequest(ServerInfo info, std::string request) : _info(info), _request(request), _file(""), _status(200) {
	checkSyntax();
std::cout << "Syntax checked! Status: " << _status << std::endl;
	determinateLoc();
std::cout << "Location found! Root is: " << _loc.root << std::endl;
	checkMethod();
std::cout << "Method checked! Status: " << _status << std::endl;
	checkSize();
std::cout << "Size of body checked! Status: " << _status << std::endl;
	determinateFile();
std::cout << "File requested: " << _file << " Status: " << _status << std::endl << std::endl;
}

ClientRequest::~ClientRequest() {
	
}

int	ClientRequest::checkMethod() {
std::cout << "Method: " << _method << " Allowed[GPDP]: " << _loc.allow[0] << _loc.allow[1] << _loc.allow[2] << std::endl;
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

//std::cout << "Host found" << std::endl;

	std::string	firstLine = _request.substr(0, _request.find("\r\n") + 2);
	std::string	word = firstLine.substr(0, firstLine.find(" "));

// std::cout << "FirstLine: " << firstLine << std::endl;


	for (int count = 1; count < 4; count++) {
// std::cout << "Word: " << word << std::endl;
		if (count == 1) {
			if (!isMethod(word)) {
				_status = 400;
				// return _status;
			}
			_method = word;
		}
		if (count == 2) {
			_uri = word;
		}
		if (count == 3) {
			if (word != "HTTP/1.1\r\n") {
				_status = 400;
				// return _status;
			}
		}
		firstLine.erase(0, word.length() + 1);
		word = firstLine.substr(0, firstLine.find(" "));
	}

//std::cout << "FirstLine OK!" << std::endl;

	std::string copy_request = _request;
	std::string	bodyLine;
	std::string	header_field;
	std::string	value;
	int			body_size = 0;

	copy_request.erase(0, copy_request.find("\r\n") + 2);
	bodyLine = copy_request.substr(0, copy_request.find("\r\n") + 2);


	while (bodyLine.length() && bodyLine != "\r\n") {
// std::cout << "Bodyline: " << bodyLine << std::endl;
		if (bodyLine.find(":") == std::string::npos) {
			_status = 400;
			return _status;
		}
		
		header_field = bodyLine.substr(0, bodyLine.find(":"));
		value = bodyLine.substr(bodyLine.find(":") + 1);
// std::cout << "Header_field: " << header_field << std::endl;
// std::cout << "Value " << value << std::endl;

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

//std::cout << "Headers OK!" << std::endl;

	if (!bodyLine.length())
		return 200;
	
	copy_request.erase(0, 2);
	_body = copy_request;
	if ((long)copy_request.size() != (long)body_size) {
		_status = 400;
		return _status;
	}

	return 200;
}

int	ClientRequest::checkSize() {
	if ((long)_body.size() > (long)_loc.clientSize)
		_status = 413;
	return _status;
}

std::string	ClientRequest::determinateFile() {
	std::string	file_uri = _uri.substr(_loc.uri.length());
	_file.insert(0, _loc.root);
	_file.insert(_file.size(), file_uri);
	int ext_true = 0;
	if (file_uri.find(".") != std::string::npos && file_uri.find("/") != 0)
		ext_true = 1;
	if (_file.at(_file.length() - 1) != '/' && !ext_true)
		_file.insert(_file.size(), "/");
	if (!_info.getAutoIndex() && _file.at(_file.length() - 1) == '/') {
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
	_loc.uri = "/";
	_loc.allow[0] = _info.getAllow("GET");
	_loc.allow[1] = _info.getAllow("POST");
	_loc.allow[2] = _info.getAllow("DELETE");
	_loc.clientSize = _info.getClientSize();

	tmp_uri = _uri;
	if (tmp_uri.find(".") != std::string::npos)
		ext = tmp_uri.substr(tmp_uri.find("."));
	tmp_uri.erase(tmp_uri.find_last_of('/') + 1);
std::cout << "URI: " << tmp_uri << " EXT: " << ext << std::endl;

	while (tmp_vec.size() != 0) {
		int	loop = 1;
		for (int count = 0; count < (int)tmp_vec.size() && loop; count++) {
			Location tmp = tmp_vec.at(count);
//std::cout << "URI demande:" << tmp_uri << " URI de la location: " << tmp.uri << std::endl;
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
		if (loop) {
			tmp_uri.erase(tmp_uri.length() - 1);
			tmp_uri.erase(tmp_uri.find_last_of('/') + 1);
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
