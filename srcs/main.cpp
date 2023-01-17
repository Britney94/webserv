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

int main(int argc, char **argv)
{
	WebServer	server;
	char	*filename;
	if (argc == 1)
		filename = ((char *)"./config/default.conf");
	else if (argc == 2)
		filename = argv[1];
	else
		std::cerr << "Usage : ./webserv [configuration file]" << std::endl;
	if (server.parsefile(filename)) {
		server.launch();
	}
	server.clean();
	return 0;
}
