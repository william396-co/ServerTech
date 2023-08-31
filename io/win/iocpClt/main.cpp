#include "util.hpp"
#include <iostream>
#include <string>

#pragma comment(lib,"ws2_32.lib")


const char* hostname = "127.0.0.1";
const char* port = "9527";

int main()
{
	std::cout << "Hello IOCP(client)\n";

	SOCKET clientfd = open_clientfd(hostname, port);
	if (clientfd == INVALID_SOCKET) {
		std::cerr << "open_clientfd failed\n";
		return 1;
	}


	std::string buf;
	while (getline(std::cin, buf)) {
		send(clientfd, buf.c_str(), (int)buf.size(), 0);
	}
	return 0;
}