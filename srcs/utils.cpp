#include <iostream>
#include <sstream>
#include <string>

#include "../includes/webserv.hpp"

int checkExt(char *filename) {
	std::string str(filename);
	if (str.find(".conf") == std::string::npos) {
		std::cerr << RED << "Error: configuration file must be a .conf file" << BLANK << std::endl;
		return 1;
	}
	return 0;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string toString(int num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

int has(std::string to_search, std::string to_find) {
    for (int pos = 0; pos < (int)to_search.length(); pos++) {
        if (to_search[pos] == to_find[0]) {
            int i = 0;
            while (to_find[i] && to_search[pos + i] == to_find[i]) {
                i++;
            }
            if (to_find[i] == '\0') {
                return pos;
            }
        }
    }
    return -1;
}