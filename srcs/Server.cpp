#include "../includes/webserv.hpp"

Server::Server(ServerInfo* infos, int port) {
    // Generating socket file descriptor
    // DOMAIN	= Ipv4 Internet protocol
    // TYPE		= Non-blocking socket descriptor
    // 		  (prevent usage of fcntl()
    // PROTOCOL	= Default (unspecified)
    // port = 80;
	this->_error = 0;
	this->_default = infos;
	this->_infos.push_back(infos);
	this->_size = this->_infos.size();
	this->_status = 200;
	if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Error socket");
        _error = 1;
		return ;
    }
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->_addr.sin_port = htons(port);
	// ioctl()
	memset(this->_addr.sin_zero, '\0', sizeof(this->_addr.sin_zero));
	// Binding the socket to the
	// socket-address struct parameters
	// +
	// Start listening on the socket
	if (bind(this->_socket, (struct sockaddr *)&(this->_addr), sizeof(this->_addr)) < 0) {
		std::cerr << RED;
		perror("Error bind");
		std::cerr << BLANK;
        _error = 1;
		return ;
	}
	if (listen(this->_socket, MAX_FD) < 0) {
		perror("Error listen");
        _error = 1;
		return ;
	}
}

Server::Server(Server& copy, int new_socket) {
	this->_socket = new_socket;
	for (size_t count = 0; count < copy._infos.size(); count++) {
		ServerInfo * new_info = new ServerInfo(*(copy._infos.at(count)));
		this->_infos.push_back(new_info);
	}
	this->_default = this->_infos[0];
	this->_size = this->_infos.size();
}

Server& Server::operator=(Server& copy) {
	this->_socket = dup(copy._socket);
	for (size_t count = 0; count < copy._infos.size(); count++) {
		ServerInfo * new_info = new ServerInfo(*(copy._infos.at(count)));
		this->_infos.push_back(new_info);
	}
	this->_default = this->_infos[0];
	this->_size = this->_infos.size();
	return (*this);
}

Server::~Server(void) {
	while(this->_infos.size() > 0)
		this->_infos.clear();
	if (this->_default)
		delete(this->_default);
	if (this->_socket)
		close(this->_socket);
}


int	Server::getSocket(void) const {
	return (this->_socket);
}

void	Server::setSocket(int socket_descriptor) {
	this->_socket = socket_descriptor;
}

void	Server::addNewInfo(ServerInfo* new_infos) {
	this->_infos.push_back(new_infos);
	this->_size++;
}

int	Server::accept_fd() {
	int	new_socket;
	int	size = sizeof(_addr);
	new_socket = accept(_socket, (struct sockaddr *)&_addr, (socklen_t *)&(size));
	if (new_socket == -1)
		std::cerr << RED << "Error: accept()" << BLANK << std::endl;
	return new_socket;
}

void	Server::close_socket() {
	if (this->_socket > 0)
		close(this->_socket);
}

/*
 * Create the file with the body of the multipart request
 */
//static void extractFileData(const string& body, const string& boundary, const string& filename) {
//    string data = body.substr(body.find("Content-Type:"));
//    data = data.substr(data.find("\n") + 1);
//    data = data.substr(data.find("\n") + 1, data.find("\r\n--"));
//    string delimiter = "\r\n--" + boundary + "\r\n";
//    ofstream file(filename.c_str(), ios::binary);
//    if (file) {
//        file.write(data.data(), data.size() - 1);
//        file.close();
//    }
//}

std::string getFilename(std::vector<char> body, std::string boundary) {
    std::string filename = "";
    size_t i = 0;
    int boundaryFound = 0;
    std::string filenameStr = "filename=\"";
    while (i < body.size()) {
        size_t j = 0;
        if (body[i] == filenameStr[j]) {
            while (j < filenameStr.size() && body[i + j] == filenameStr[j]) {
                j++;
            }
            if (j == filenameStr.size()) {
                i += j;
                while (body[i] != '"') {
                    filename += body[i];
                    i++;
                }
                if (boundaryFound <= 1)
                    return filename;
                else
                    return "";
            }
        }
        size_t k = 0;
        if (body[i] == boundary[k]) {
            while (k < boundary.size() && body[i + k] == boundary[k]) {
                k++;
            }
            if (k == boundary.size()) {
                boundaryFound++;
                i += k;
            }
        }
        i++;
    }
    return filename;
}

/*
 * Save the files in the directory (when multipart request)
 * Return pathTranslated which is the environment variable with the path(s) of the file(s)
 */
static std::string saveFiles(std::vector<char> body, std::string boundary, std::string bodyStr) {
    std::string pathTranslated = "";
    boundary = "--" + boundary;
    // Remove header of the body
    for (size_t i = 0; i < body.size(); i++) {
        if (body[i] == '\r' && body[i + 1] == '\n' && body[i + 2] == '\r' && body[i + 3] == '\n') {
            body.erase(body.begin(), body.begin() + i + 4);
            break;
        }
    }
    // Remove the first boundary
    for (size_t i = 0; i < body.size(); i++) {
        size_t j = 0;
        while (j < boundary.size() && body[i + j] == boundary[j]) {
            j++;
        }
        if (j == boundary.size()) {
            body.erase(body.begin(), body.begin() + i + j + 2);
            break;
        }
    }
    std::string filename = "";
    while (body.size() > 0) {
        std::string bufferString = body.data();
        // Check if there is a filename before the boundary
        filename = getFilename(body, boundary);
        if (filename != "")
            filename = "./tmp/" + filename;
        std::cout << "filename: " << filename << std::endl;
        std::ofstream file(filename.c_str(), std::ios::binary);
        if (filename == "")
            file.close();
        else
            pathTranslated += filename + "\n";
        std::cout << "pathTranslated: " << pathTranslated << std::endl;
        int sizeBoundary = bufferString.find_last_of("Content-Type:");
        bufferString = bufferString.substr(sizeBoundary);
        sizeBoundary += bufferString.find("\n") + 1;
        bufferString = bufferString.substr(bufferString.find("\n") + 1);
        sizeBoundary += bufferString.find("\n") + 1;
        bufferString = bufferString.substr(bufferString.find("\n") + 1);
        while (sizeBoundary < (int)body.size()) {
            int i = 0;
            if (body[sizeBoundary] == boundary[i]) {
                i++;
                while (body[sizeBoundary + i] == boundary[i] && i < (int)boundary.size()) {
                    i++;
                }
                if (i == (int)boundary.size()) {
                    break;
                }
            }
            if (filename != "" && (filename.find(".jpg") != std::string::npos || filename.find(".jpeg") != std::string::npos || filename.find(".png") != std::string::npos))
                file.write(&body[sizeBoundary], 1);
            sizeBoundary++;
        }
        if (filename != "" && (filename.find(".jpg") == std::string::npos && filename.find(".jpeg") == std::string::npos && filename.find(".png") == std::string::npos)) {
            std::string searchFile = filename.substr(filename.find_last_of("/") + 1);
            if (bodyStr.find(searchFile) != std::string::npos) {
                std::string tmp(bodyStr.substr(bodyStr.find(searchFile)));
                tmp = tmp.substr(tmp.find("\n") + 1);
                tmp = tmp.substr(tmp.find("\n") + 1);
                tmp = tmp.substr(tmp.find("\n") + 1, tmp.find("\r\n--"));
                while (!isascii(tmp[0]))
                    tmp.erase(tmp.begin());
                file.write(tmp.c_str(), tmp.size() - 1);
            }
        }
        body.erase(body.begin(), body.begin() + sizeBoundary);
    }
    return pathTranslated;
}

/*
 * Compare the length of the request with the content-length
 * Return 1 if the request is good
 */
int Server::checkContentRequest() {
    // Get the body of the request and write it in a file
    std::string body = _request.substr(_request.find("\r\n\r\n") + 4);
    std::string tmpFile = "tmp/" + toString(_socket);
    _tmpBody.open(tmpFile.c_str(), std::ios::out | std::ios::trunc);
    // Check if the request is a multipart/form-data
    if (_request.find("Content-Type: multipart") != std::string::npos) {
        std::cout << "MULTIPART" << std::endl;
        return 1;
    }
    // Check the content length of the request (if not multipart)
    else if (_request.find("Content-Length") != std::string::npos) {
        size_t	n = std::atoi(&(_request.substr(_request.find("Content-Length: ") + 16))[0]);
        _tmpBody << body;
        _tmpBody.seekg(0, _tmpBody.end);
        int	size = _tmpBody.tellg();
        if (size != (int)n) {
	        _tmpBody.close();
        	return 0;
        }
    }
	_tmpBody.close();
    return 1;
}

int	Server::parseRequest() {
    int        ret;
    char    buffer[REQUEST_SIZE] = {0};
    std::vector<char> tmpBuffer(REQUEST_SIZE);
    ret = read(_socket, tmpBuffer.data(), REQUEST_SIZE - 1);
    memcpy(buffer, tmpBuffer.data(), tmpBuffer.size());
    _vectorBody = std::vector<char>(tmpBuffer.begin(), tmpBuffer.end());
//    std::string bufferString = tmpBuffer.data();
//    std::ofstream file("test.png", std::ios::binary | std::ios::app);
//    if (!file.is_open()) {
//        std::cerr << "Impossible d'ouvrir le fichier\n";
//        return 1;
//    }
//    std::ofstream file2("test.txt", std::ios::binary | std::ios::app);
//    if (!file.is_open()) {
//        std::cerr << "Impossible d'ouvrir le fichier\n";
//        return 1;
//    }
//    int sizeBoundary = bufferString.find_last_of("Content-Type:");
//    bufferString = bufferString.substr(sizeBoundary);
//    sizeBoundary += bufferString.find("\n") + 1;
//    bufferString = bufferString.substr(bufferString.find("\n") + 1);
//    sizeBoundary += bufferString.find("\n") + 1;
//    bufferString = bufferString.substr(bufferString.find("\n") + 1);
//    while (sizeBoundary < (int)tmpBuffer.size()) {
//        // checker ici si c'est delimiter, si oui, break
//        file.write(&tmpBuffer[sizeBoundary], 1);
//        file2.write(&tmpBuffer[sizeBoundary], 1);
//        sizeBoundary++;
//    }
	if (ret <= 0) {
		this->close_socket();
		if (!ret)
			std::cerr << "\nConnection closed by client.\n" << std::endl;
		else
			std::cerr << "\nError: Could not read request.\n" << std::endl;
		return -1;
	}
	_request.insert(_request.size(), buffer);
	if (_request.find("Transfer-Encoding: chunked") != std::string::npos) {
		if (this->chunkedRequest())
			return 1;
		else
			parseChunked(); 
	}
	if (!this->checkContentRequest())
        return 1;
	std::cout << std::endl << std::endl << BHBLU << "*** Request ***\n" << BLUE << _request << BLANK;
	ServerInfo	clientInfo(requestInfos());
	ClientRequest	client(clientInfo, _request);
	_file_request = client.getFile();
	_body = client.getBody();
	_status = client.getStatus();
	_method = client.getMethod();
	_cgi = client.getCGI();
	return 0;
}

int	Server::chunkedRequest() {
	if (_request.find("0\r\n\r\n") != std::string::npos)
		return 0;
	return 1;
}

int	Hex_to_Int(std::string hex) {
	std::ostringstream oss;
    oss << std::hex << hex;
    return std::atoi((char *)&(oss.str())[0]);
}

void	Server::parseChunked() {
	std::string header;
	std::string received;
	std::string body = "";
	int size = 0;
	int i;
	header = _request.substr(0, _request.find("\r\n\r\n") + 4);
	received = _request.substr(_request.find("\r\n\r\n") + 4);
	while (received.size()) {
		i = received.find("\r\n") + 2;
		size = Hex_to_Int(received.substr(0, i - 2));
		body += received.substr(i, i + size);
		received = received.substr(i + size + 2);
	}
	_request = header + body + "\r\n";
}

int	Server::sendResponse(std::map<int, std::string> errors, char **envp) {
	HttpResponse    response;
	response.setHost(_default->getIp());
	response.setMethod(_method);
    if (_request.find("Content-Type: multipart") != std::string::npos) {
	    std::string tmpType = _request.substr(_request.find("Content-Type: ") + 14);
	    tmpType = tmpType.substr(0, tmpType.find(";"));
        response.setContentType(tmpType);
        std::string tmpLength = _request.substr(_request.find("Content-Length: "));
        tmpLength = tmpLength.substr(0, tmpLength.find("\n"));
        response.setContentLength(tmpLength);
        std::string boundary = _request.substr(_request.find("boundary=") + 9);
        boundary = boundary.substr(0, boundary.find("\r\n"));
        response.setBoundary(boundary);
        std::string tmpBody = _request;
        response.setPathTranslated(saveFiles(_vectorBody, boundary, tmpBody));

    }
	response.setClientBody(_body);
	response.setCGI(_cgi);
	response.setFile(_file_request);
	response.setStatus(_status);
	response.setErrorFiles(errors);
	response.createResponse(envp);
	std::string message = response.getResponse();
	int	ret;
	int max_size = 0;
	ret = write(_socket, &message[0], message.size());
	if (ret <= 0) {
		std::cerr << "Error: Could not write response to client." << std::endl;
		return ret;
	}
	if (ret != (int)message.size()) {
	    std::cerr << "Error: Could not write all response to client." << std::endl;
	    return ret;
	}
	if (message.size() > 125) {
        message.resize(125);
        max_size = 1;
    }
	std::cout << std::endl << std::endl << BHGRN << "*** Response ***" << BLANK << std::endl;
	std::cout << GREEN << message << BLANK << std::endl;
	if (max_size)
        std::cout << LGREEN << "[...]" << BLANK << std::endl;
	this->close_socket();
	_request.erase();
	_file_request.erase();
	_body.erase();
	_method.erase();
	_status = 200;
	return (0);
}

ServerInfo	*Server::requestInfos() {
	std::string	serv_name;
	size_t  found;
	found = _request.find("HOST:");
	if (found == std::string::npos)
		found = _request.find("Host:");
	if (found == std::string::npos)
		return _default;
	serv_name = _request.substr(found + 5);
	if (serv_name.at(0) == ' ')
		serv_name.erase(0, 1);
	serv_name = serv_name.substr(0, serv_name.find("\r\n"));
	if (serv_name.find(":") != std::string::npos)
		serv_name.erase(serv_name.find(":"));
	for (std::vector<ServerInfo *>::iterator it = _infos.begin(); it != _infos.end(); it++) {
		for (int count = 0; count < (int)(*it)->getServerNames().size(); count++) {
			std::string	name = (*it)->getServerNames().at(count);
			if (serv_name == name)
				return (*it);
		}
	}
	return _default;
}

int	Server::getError() const {
	return _error;
}

std::vector<ServerInfo *>	Server::getInfos() const {
	return _infos;
}
