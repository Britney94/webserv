#include "../includes/webserv.hpp"

std::vector<char>	permissions(const char *file)
{
	struct stat	st;
	//char		*modeval = (char *)malloc(sizeof(char) * 9 + 1);
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
		//modeval[9] = '\0';
		return modeval;     
	}
	else
	{
		perror("Error stat call: ");
		return modeval;     
		//return strerror(errno);
	}
};

HttpResponse::HttpResponse() {
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

std::string toString(int num)
{
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

int	HttpResponse::createResponse()
{
	// Constructor
	// PARAM : 	method = GET/POST/DELETE etc, from the request received
	// 		file = path to file to open ("/test" for example)
	// 		status = set to 400+ if an error occured prior to this
	// 		conf = param file containing the map to [error_files] + [clientSize]
	// 		autoindex = 1 or 0, indicator of auto-generation of an index
	// 			if a folder is set as param-1 [file-to-open]
	// 0th : Verify if status indicates an error detected somewhere else
	// 1st : Try to open file, if impossible, search proof of 403, if none = 404
	// 2nd : Open it and retrieve its content into a string
	// 3rd : DEBUG, print the content in outstream,
	// 4th : Generate a header + body according to situation (ongoing)
	
	std::ifstream			filestream;
	std::filebuf			filebuf;

std::cout << "File to open: " << _file << std::endl;
std::cout << "Status: " << _status << std::endl;
std::cout << "Method: " << _method << std::endl;
std::cout << "Body: " << _body << std::endl;


	if (_status >= 400 && _status < 500)
	{
		// Status was already set by another feature
		// body becomes the corresponding error file

		filestream.open(_errorFiles[_status].c_str());
		filestream >> this->_file_content; 
		std::cout << this->_file_content << std::endl; 	// DEBUG
	}
	else
	{
		// Status must be defined here

		std::string	path = _file;

		int	ret = open(path.c_str(), O_RDONLY);
		if (ret > 0 && _method == "PUT")
		{
			this->_status = 201;
			// And ?
		}
		else if (ret > 0 && _method != "PUT")
		{
			filestream.open(path.c_str());
			filestream >> this->_file_content;
			std::cout << "File content: " << this->_file_content << std::endl; 	// DEBUG
			this->_status = 200;				// SUCCESS
		}
		else if (ret < 0)
		{
			// Couldn't open the file

			//char	*file_permissions = permissions(path.c_str());
			std::vector<char>	file_permissions = permissions(path.c_str());
			if (file_permissions[0] == 'e')
				this->_status = 404;
			else if (_method == "POST")
			{
				if (file_permissions[1] == '-')
					this->_status = 403;	// using method object as is
			}
			else if (_method == "GET")
			{
				if (file_permissions[0] == '-')
					this->_status = 403;	// using method object as is
			}
			else
				this->_status = 404;
			filestream.open(_errorFiles[_status].c_str());
			filestream >> this->_file_content; 
		}
		close(ret);
	}
	// Status is now set, creating response object parts
	// HTTP/1.1 2XX OK
	// HTTP/1.1 4xx ERROR
	if (this->_status >= 400)
	{
		this->_header = "HTTP/1.1 ";
		this->_header = this->_header + toString(this->_status) + " ";
		this->_header = this->_header + "ERROR\r\n";
		this->_body = this->_file_content;
	}
	if (this->_status >= 200 && this->_status < 300)
	{
		this->_header = "HTTP/1.1 ";
		this->_header = this->_header + toString(this->_status) + " ";
		this->_header = this->_header + "OK\r\n";
		this->_body = this->_file_content;
	}
	// Content-length: xxx
	this->_header = this->_header + "Content-Lenght: ";	// Content-length = clientSize
	this->_header = this->_header + toString(this->_body.length());
	this->_header = this->_header + "\r\n";	
	// When HEADER + BODY are generated, join both to
	// create the response
	this->_response = this->_header;
	this->_response = this->_response + "\r\r\n\n";
	this->_response = this->_response + this->_body;
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