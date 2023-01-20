#include "../includes/webserv.hpp"

// int main()
// {
// 	ConfigInfo config;
	
// 	config.parse((char *)"./config/default.conf");
// 	std::cout << config.getServers()[8180]->getInfos().at(0)->getServerNames().at(0) << std::endl;
// 	// std::cout << config;
// 	return 0;
// }

// int main()
// {
// 	ServerInfo info;

// 	info.setClientSize("client_max_body_size sqq0qdsq");
// 	// ConfigInfo config((char *)"./config/default.conf");
// 	// std::cout << config;
// 	ConfigInfo config((char *)"./config/default.conf");
// 	std::cout << config;
// 	CGI cgi;
// 	autoindex index;
// 	std::cout << index.renderPage("YoupiBanane", "YoupiBanane", 80, "Youpi");
// 	return 0;
// }

WebServer server;

void	quit(int arg) {
	(void)arg;
	server.setRunning(0);
}

int main(int argc, char **argv)
{
	WebServer	server;
	char	*filename = NULL;
	
	if (argc == 1)
		filename = ((char *)"./config/default.conf");
	else if (argc == 2) {
		if (checkExt(argv[1]) == 1)
			return 1;
		filename = argv[1];
	}
	else
		std::cerr << RED << "Usage : ./webserv [configuration file]" << BLANK << std::endl;
	signal(SIGINT, quit);
	if (server.parsefile(filename))
	{
		if (server.launch() == 1)
			return 1;
	}
	else
		return 1;
	server.clean();
	std::cout << std::endl << GREEN << "The user has stopped the program 👋" << BLANK << std::endl;
	return 0;
}
