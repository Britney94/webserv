#include "../includes/webserv.hpp"

int main()
{
	ConfigInfo config;
	// std::cout << "Config [" << config << "]" << std::endl;
	return 0;
}

// int	main()
// {
// 	ServerInfo	si();
//	Server		serv(si, 8080);

// 	std::cout << "Si [" << si << "]" << std::endl;
// 	std::cout << "Serv [" << serv << "]" << std::endl;
// 	return 0;
// }

// int main(int argc, char **argv)
// {
// 	WebServer	server;
// 	char	*filename;
// 	if (argc == 1)
// 		filename = "./config/default.conf";
// 	else if (argc == 2)
// 		filename = argv[1];
// 	else
// 		std::cerr << "Usage : ./webserv [configuration file]" << std::endl;
// 	server.parsefile(filename);
// 	server.launch();
// 	server.clean();
// 	return 0;
// }
