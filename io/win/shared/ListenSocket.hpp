#pragma once
#include <WinSock2.h>
#include <thread>
#include <string>
#include <type_traits>
#include <shared_mutex>
#include <mutex>

#include "type.hpp"
#include "util.hpp"
#include "Socket.hpp"
#include "SocketOps.hpp"
#include "SocketMgr.hpp"
#include "print.hpp"

template<typename T>
using EnableIsSocket = std::enable_if_t<std::is_constructible_v<T, Socket>>;

template<typename T>
class ListenSocket {
public:
	ListenSocket(const char* host, uint32 port)
		:m_fd{ 0 },
		m_cp{ nullptr },
		m_address{},
		m_tempAddres{},
		m_IsOpen{ false }
	{
		m_fd = open_listenfd(std::to_string(port).c_str());
		if (m_fd <= 0) {
			return;
		}

		//		SocketOps::ReuseAddr(m_fd);
		SocketOps::Blocking(m_fd);
		SocketOps::SetTimeout(m_fd, 60);
		m_IsOpen = true;

		// IOCP
		m_cp = SocketMgr::instance().GetCompletionPort();
	}

	void run()
	{
		socklen_t len = 0;
		while (m_IsOpen) {
			SOCKET s = WSAAccept(m_fd, (sockaddr*)&m_tempAddres, (socklen_t*)&len, nullptr, NULL);
			if (s == INVALID_SOCKET)continue;

			if (!m_IsOpen)
				break;

			T* socket = new T{ s };
			socket->SetCompletionPort(m_cp);
			socket->Accept(&m_tempAddres);
		}

		println("finished ListenThread ", std::this_thread::get_id());
	}

	~ListenSocket() {
		Close();
	}

	void Close() {

		if (m_IsOpen) {
			SocketOps::CloseSocket(m_fd);			
			m_IsOpen.store(false);
		}
	}
private:
	SOCKET m_fd;
	HANDLE m_cp;
	struct sockaddr_in m_address;
	struct sockaddr_in m_tempAddres;
	std::atomic_bool m_IsOpen; // multithread change value
};