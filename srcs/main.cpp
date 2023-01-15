#include "../includes/webserv.hpp"

// int main()
// {
// 	ConfigInfo config;
	
// 	config.parse((char *)"./config/default.conf");
// 	std::cout << "triple out" << std::endl;
// 	std::cout << config;
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
