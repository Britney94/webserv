#include "../includes/webserv.hpp"

std::vector<char>	permissions(const char *file)
{
	struct stat	st;
	std::vector<char>	modeval(9, 'e');

	if (stat(file, &st) == 0)
	{
		mode_t perm = st.st_mode;
		modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
		modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
		modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
		modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
		modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
		modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
		modeval[6] = (perm & S_IROTH) ? 'r' : '-';
		modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
		modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
		return modeval;     
	}
	else
	{
		perror("Error stat call: ");
		return modeval;     
	}
}

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

void	HttpResponse::setPort(int port) {
	_port = port;
	return ;
}

void	HttpResponse::setAutoIndex(int autoindex) {
	_autoindex = autoindex;
	return ;
}

std::string toString(int num)
{
    std::ostringstream oss;
    oss << num;
    return oss.str();
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

int	HttpResponse::createResponse()

{	
		std::ifstream			filestream;
		std::filebuf			filebuf;

std::cout << "File to open: " << _file << std::endl;
std::cout << "Status: " << _status << std::endl;
std::cout << "Method: " << _method << std::endl;
std::cout << "Body: " << _body << std::endl;



	if (_status >= 400 && _status < 500)
	{

	std::cout << "Error file: " << _errorFiles[_status].c_str() << std::endl;
			filestream.open(_errorFiles[_status].c_str());
			while(filestream.good()) {
				std::getline(filestream, this->_file_content);
				this->_body += this->_file_content;
				this->_body += '\n';
			}
	std::cout << "Error file content: " << this->_file_content << std::endl;
	}
	// else if (_cgi.size() != 0) {
	// 	CGI	handler;


	// 	handler.setScript(_cgi);
	// 	handler.setMethod(_method);
	// 	handler.setBody(_body);
	// 	handler.setPath(_file);
	// 	handler.setQuery(_query);
	// 	handler.setAddr(_host);
	// 	handler.setPort(_port);

	// 	_response = handler.execute(_cgi);
	// 	std::cout << "Reponse CGI: " << _response << std::endl;

	// 	return 0;
	// }
	else if (_method == "GET") {

		if (isFile(_file)) {
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
		else if (isDir(_file) && _autoindex) {
			autoindex	content;

			_body = content.renderPage(_dir, _file, _port, _host);
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

	else if (_method == "POST") {
		_status = 204;
	}
	else if (_method == "PUT") {
		std::ofstream	newFile;
		
		if (isFile(_file)) {
			newFile.open(_file.c_str());
			newFile << _body;
			newFile.close();
			_status = 204;
		}
		else {
			newFile.open(_file.c_str(), std::ofstream::trunc);
			
			if (newFile.is_open() == false) {
				_status = 403;
				filestream.open(_errorFiles[_status].c_str());
				while(filestream.good()) {
					std::getline(filestream, this->_file_content);
					this->_body += this->_file_content;
					this->_body += '\n';
				}
				filestream.close();
			}
			else {
				newFile << _body;
				newFile.close();
				_status = 201;
			}
		}
	}
	else if (_method == "DELETE")
	{
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
		_header += "\r\n" + _body + "\r\n";

	}
	_response = _header;
	return 0;
}

HttpResponse::~HttpResponse(void)
{
	return ;
}

std::string	HttpResponse::getResponse(void) const
{
	return (this->_response);
}
