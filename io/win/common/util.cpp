#include "util.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>


SOCKET open_clientfd(const char* hostname, const char* port) {
	SOCKET clientfd;
	int rc;
	struct addrinfo hints, * listp, * p;

	/* Get a list of potential server addresses */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;/* Open a connection */
	hints.ai_flags = AI_NUMERICSERV;/* using a numeric port arg */
	hints.ai_flags |= AI_ADDRCONFIG;/* Recommand for conneciton */
	if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0) {
		std::cerr << "getaddrinfo failed(" << hostname << ":" << port << "):" << gai_strerror(rc) << "\n";
		return -2;
	}

	/* Walk the list for one that we can successfully connect to */
	for (p = listp; p; p = p->ai_next) {
		/* Create a socket descriptor */
		if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;

		/* Connect to the server */
		if (connect(clientfd, p->ai_addr, (int)p->ai_addrlen) != 1)
			break;
		if (closesocket(clientfd) < 0) {
			std::cerr << "open_clientfd: close failed:" << strerror(errno) << "\n";
			return -1;
		}
	}

	/* Clean up */
	freeaddrinfo(listp);
	if (!p)/* All connects failed */
		return -1;
	else/* The last connect succeeded */
		return clientfd;
}

constexpr auto LISTENQ = 1024;
SOCKET open_listenfd(const char* port) {

	struct addrinfo hints, * listp, * p;
	SOCKET listenfd;
	int rc, optval = 1;

	/* Get a list of potential server addresses */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;/* Open a connection */
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;/* on any IP address */
	hints.ai_flags |= AI_NUMERICSERV;/* using port number */
	if ((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0) {
		std::cerr << "getaddrinfo failed(port" << port << "):" << gai_strerror(rc) << "\n";
		return -2;
	}

	/* Walk the list for one that we can bind to */
	for (p = listp; p; p = p->ai_next) {
		/* Create a socket descriptor */
		if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;

		/* Eliminates "Address already in use" error from bind */
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(int));

		/* Bind the descriptor to the address */
		if (bind(listenfd, p->ai_addr, (int)p->ai_addrlen) == 0)
			break;

		if (closesocket(listenfd) < 0) {
			std::cerr << "open_listenfd close failed:" << strerror(errno) << "\n";
			return -1;
		}
	}

	/* clean up*/
	freeaddrinfo(listp);
	if (!p) /* No address worked */
		return -1;
	/* Make it a listening socket ready to accept connection requests */
	if (listen(listenfd, LISTENQ) < 0) {
		closesocket(listenfd);
		return -1;
	}
	return listenfd;
}