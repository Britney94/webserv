#include "../includes/webserv.hpp"

CGI::CGI() {
	this->_query = "";
	this->_body = "";
}

CGI::CGI(const CGI &src) {
	if (this != &src) {
		this->_body = src._body;
		this->_env = src._env;
	}
	return ;
}

CGI	&CGI::operator=(const CGI &src) {
	if (this != &src) {
		this->_body = src._body;
		this->_env = src._env;
	}
	return *this;
}

/*
 * Create the env array for the CGI program from envp
 * Take the envp array as argument and the pathInfo and query string if needed
 * Return the new env array with PATH_INFO variable
 */
char	**CGI::_createEnv(char **envp, std::string pathInfo) const {
    // Calculate the size of the envp array
    int sizeEnvp = 0;
    while (envp[sizeEnvp])
        sizeEnvp++;
    (void)pathInfo;
    // Check if the query string is not empty
    if (this->_query.size() > 1)
        sizeEnvp++;
    // Set the new array
	char	**env = new char*[sizeEnvp + 2];
    int i = 0;
    while (i < sizeEnvp - 1) {
        env[i] = new char[strlen(envp[i]) + 1];
        env[i] = strcpy(env[i], envp[i]);
        i++;
    }
    // Add PATH_INFO variable in env
    std::string element = "PATH_INFO=" + pathInfo;
    env[i] = new char[element.size() + 1];
    env[i] = strcpy(env[i], (const char*)element.c_str());
    i++;
    // Add the QUERY variable in env if needed
    if (this->_query.size()) {
        std::string element = "QUERY_STRING=" + this->_query;
        env[i] = new char[element.size() + 1];
        env[i] = strcpy(env[i], (const char*)element.c_str());
        i++;
    }
    // Set the last element to NULL
    env[i] = NULL;
	return env;
}

/*
 * Execute the CGI program
 * Take two argument: the path to the CGI program and the envp arrays
 * Return the body of the response
 */
std::string	CGI::execute(const std::string& scriptName, char **envp) {
	std::string	tmpBody;

	char **env = this->_createEnv(envp, scriptName);
	if (this->_query.size())
	    this->setBody(this->_query);
	int tmpStdin = dup(STDIN_FILENO);
	int tmpStdout = dup(STDOUT_FILENO);
	FILE	*fileIn = tmpfile();
	FILE	*fileOut = tmpfile();
	long	fdIn = fileno(fileIn);
	long	fdOut = fileno(fileOut);
	int	ret = 1;
	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	pid_t   pid = fork();
	if (pid == -1) {
		std::cerr << "Error: fork() in execute" << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid) {
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		// Create the arg array for execve
        char* const arg[] = {const_cast<char*>(scriptName.c_str()), const_cast<char*>(_body.c_str()), NULL};
        // Call the CGI script with the script and the arguments (variables)
        execve(scriptName.c_str(), arg, env);
		std::cerr << "Error: execve() in execute" << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else {
		char    buffer[65536] = {0};
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		ret = 1;
		while (ret > 0) {
			memset(buffer, 0, 65536);
			ret = read(fdOut, buffer, 65536 - 1);
			tmpBody += buffer;
		}
	}
	dup2(tmpStdin, STDIN_FILENO);
	dup2(tmpStdout, STDOUT_FILENO);
	fclose(fileIn);
	fclose(fileOut);
	close(fdIn);
	close(fdOut);
	close(tmpStdin);
	close(tmpStdout);
	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;
	if (!pid)
		exit(0);
	return (tmpBody);
}

void	CGI::setBody(std::string body) {
	_body = body;
	return ;
}

void	CGI::setScript(std::string cgi) {
	_cgi = cgi;
	return ;
}

void	CGI::setMethod(std::string method) {
	_method = method;
	return ;
}

void	CGI::setPath(std::string file) {
	_file = file;
	return ;
}

void	CGI::setQuery(std::string query) {
	_query = query;
	return ;
}

void	CGI::setAddr(std::string host) {
	_host = host;
	return ;
}

void	CGI::setType(std::string type) {
	_type = type;
	return ;
}

void	CGI::setPort(int    port) {
	_port = port;
	return ;
}
