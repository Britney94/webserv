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

WebServer	server;

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void	quit(int arg) {
	(void)arg;
	server.setRunning(0);
}

int main(int argc, char **argv)
{
	char	*filename = NULL;
	if (argc == 1)
		filename = ((char *)"./config/default.conf");
	else if (argc == 2)
		filename = argv[1];
	else
		std::cerr << "Usage : ./webserv [configuration file]" << std::endl;
	signal(SIGINT, quit);
	if (server.parsefile(filename)) {
		server.launch();
	}
	server.clean();
	std::cout << std::endl << GREEN << "The user has stopped the program 👋" << BLANK << std::endl;
	return 0;
}
