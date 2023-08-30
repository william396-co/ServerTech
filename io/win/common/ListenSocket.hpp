#pragma once

#include <WinSock2.h>
#include <thread>
#include <string>
#include "type.hpp"
#include "util.hpp"
#include "SocketOps.hpp"

class ListenSocket {
public:
	ListenSocket(const char* host, uint32 port) {
		
		m_fd = open_listenfd(std::to_string(port).c_str());
		if (m_fd) {
			SocketOps::ReuseAddr(m_fd);
			SocketOps::Blocking(m_fd);
			SocketOps::SetTimeout(m_fd, 60);
		}
	}
	~ListenSocket() {
		Close();
	}
private:
	void Close() {
		SocketOps::CloseSocket(m_fd);
	}
private:
	SOCKET m_fd;
	struct sockaddr_in m_address;
	struct sockaddr_in m_tempAddres;

};