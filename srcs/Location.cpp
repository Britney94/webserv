#include "../includes/webserv.hpp"

Location::Location(void) {
	uri = "";
	root = "";
	index = "index.html";
	cgi = "";
	allow[0] = 0;
	allow[1] = 0;
	allow[2] = 0;
	clientSize = -1;
};

Location::Location(const Location& copy) {
 	uri = copy.uri;
 	root = copy.root;
 	index = copy.index;
 	cgi = copy.cgi;
 	allow[0] = copy.allow[0];
 	allow[1] = copy.allow[1];
 	allow[2] = copy.allow[2];
 	clientSize = copy.clientSize;
 	// for (std::vector<Location>::iterator it = copy.loc.begin(); it != copy.loc.end(); it++) {
	// 	loc.push_back(*it);
	// }
	loc = copy.loc;
};

Location& Location::operator=(const Location& copy) {
 	uri = copy.uri;
 	root = copy.root;
 	index = copy.index;
 	cgi = copy.cgi;
 	allow[0] = copy.allow[0];
 	allow[1] = copy.allow[1];
 	allow[2] = copy.allow[2];
 	clientSize = copy.clientSize;
 	// for (std::vector<Location>::iterator it = copy.loc.begin(); it != copy.loc.end(); it++) {
	// 	loc.push_back(*it);
	// }
	loc = copy.loc;
	return *this;
};

Location::~Location() {
	return ;
}