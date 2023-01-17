#include "../includes/webserv.hpp"

CGI::CGI()//Request &request, RequestConfig &config):
{
	// this->_body = request.getBody();
	this->_setEnv();//request, config);
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

void	CGI::_setEnv(){//Request &request, RequestConfig &config) {
	// std::map<std::string, std::string>	headers = request.getHeaders();
	// if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
	// 	this->_env["AUTH_TYPE"] = headers["Authorization"];
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	// this->_env["SCRIPT_NAME"] = config.getPath();
	// this->_env["SCRIPT_FILENAME"] = config.getPath();
	// this->_env["REQUEST_METHOD"] = request.getMethod();
	// this->_env["CONTENT_LENGTH"] = to_string(this->_body.length());
	// this->_env["CONTENT_TYPE"] = headers["Content-Type"];
	// this->_env["PATH_INFO"] = request.getPath();
	// this->_env["PATH_TRANSLATED"] = request.getPath();
	// this->_env["QUERY_STRING"] = request.getQuery();
	// this->_env["REMOTEaddr"] = to_string(config.getHostPort().host);
	// this->_env["REMOTE_IDENT"] = headers["Authorization"];
	// this->_env["REMOTE_USER"] = headers["Authorization"];
	// this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();
	// if (headers.find("Hostname") != headers.end())
	// 	this->_env["SERVER_NAME"] = headers["Hostname"];
	// else
	// 	this->_env["SERVER_NAME"] = this->_env["REMOTEaddr"];
	// this->_env["SERVER_PORT"] = to_string(config.getHostPort().port);
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Weebserv/1.0";
	// this->_env.insert(config.getCgiParam().begin(), config.getCgiParam().end());
}

char	**CGI::_convertEnv() const {
	char	**env = new char*[this->_env.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); i != this->_env.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

std::string	CGI::execute(const std::string& scriptName) { //ScriptName = path to script (cgi_pass test_us/cgi_tester)
	pid_t	pid;
	int	tmpStdin;
	int	tmpStdout;
	char	**env;
	std::string	tmpBody;
	env = this->_convertEnv();
	tmpStdin = dup(STDIN_FILENO);
	tmpStdout = dup(STDOUT_FILENO);
	FILE	*fileIn = tmpfile();
	FILE	*fileOut = tmpfile();
	long	fdIn = fileno(fileIn);
	long	fdOut = fileno(fileOut);
	int	ret = 1;
	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	pid = fork();
	if (pid == -1)
	{
		std::cerr << "Error: fork() in execute" << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		char * const * nll = NULL;
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(scriptName.c_str(), nll, env);
		std::cerr << "Error: execve() in execute" << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char	buffer[65536] = {0};
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		ret = 1;
		while (ret > 0)
		{
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
