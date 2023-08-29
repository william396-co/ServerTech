#include "Socket.hpp"
#include "SocketOps.hpp"

#include <iostream>

Socket::Socket(SOCKET fd, uint32 sendbufSize, uint32 recvBufSize)
	:m_fd(fd),
	m_BytesRecved(0),
	m_BytesSent(0),
	m_completionPort(0),
	m_client()
{
	readBuffer.Allocate(recvBufSize);
	writeBuffer.Allocate(sendbufSize);

	m_fd = (m_fd == 0) ? SocketOps::CreateTCPFileDescriptor() : m_fd;
}

bool Socket::Connect(const char* Address, uint32 Port) {

	struct addrinfo hints, * listp, * p;
	int rc;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM; /* Open a connect */
	hints.ai_flags = AI_NUMERICSERV;/* using a numeric port arg */
	hints.ai_flags |= AI_ADDRCONFIG;/* Recommand for connections */
	if ((rc = getaddrinfo(Address, std::to_string(Port).c_str(), &hints, &listp)) != 0) {
		std::cerr << "getaddrinfo failed(" << Address << ":" << Port << "):" << gai_strerror(rc) << "\n";
		return false;
	}

	for (p = listp; p; p = p->ai_next) {

		if (connect(m_fd, p->ai_addr, (int)p->ai_addrlen) != 1)
			break;
		if (closesocket(m_fd) < 0) {
			std::cerr << "open_clientfd: close failed:" << strerror(errno) << "\n";
			return false;
		}
	}

	freeaddrinfo(listp);
	if (!p)
		return false;

	// at this point the connection was established
#ifdef CONFIG_USE_IOCP
	//m_completionPort = sSocketMgr.GetCompletionPort();
#endif

	_OnConnect();
	return true;
}

void Socket::Accept(sockaddr_in* address) {
	memcpy(&m_client, address, sizeof(sockaddr_in));
	_OnConnect();
}

void Socket::_OnConnect() {

}

bool Socket::Send(const uint8* Bytes, uint32 Size) {

	bool rv = false;
	std::lock_guard<std::mutex> lock(m_wMtx);
	rv = writeBuffer.Write(Bytes, Size);
	if (rv)
		BurstPush();
	return rv;
}

bool Socket::BurstSend(const uint8* Bytes, uint32 Size) {
	return writeBuffer.Write(Bytes, Size);
}

void Socket::BurstPush()
{
}

void Socket::Disconnect()
{

	// call virtual onDisconnect
	OnDisconnect();
}

void Socket::Delete() {

}