#include "../includes/webserv.hpp"

WebServer server;

void	quit(int arg) {
	(void)arg;
}

int main(int argc, char **argv) {
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
	if (server.parsefile(filename)) {
		if (server.launch() == 1)
			return 1;
	}
	else {
		server.clean();
		return 1;
	}
	server.clean();
	std::cout << std::endl << BHGRN << "The user has stopped the program." << BLANK << std::endl;
	return 0;
}
