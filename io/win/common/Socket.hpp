#pragma once

#include "Platform.hpp"
#include "CircularBuffer.hpp"
#include "socketDefines.hpp"
#include "util.h"

#include <mutex>
#include <string>

class Socket
{
public:
	// Constructor. If fd = 0, it will be assigned
	Socket(SOCKET fd, uint32 sendBufSize, uint32 recvBufSize);

	// Destructor
	virtual ~Socket();

	bool Connect(const char* host, uint32 port);

	void Disconnect();

	// Accept from the already-set fd.
	void Accept(sockaddr_in* address);

	/* Implementable methods */

	// Called when data is received.
	virtual void OnRead() {}

	// Called when a connection is first successfully established.
	virtual void OnConnect() {}

	// Called when the socket is disconnected from the client (either forcibly or by the connection dropping)
	virtual void OnDisconnect() {}

	/* Sending Operations */

	// Locks sending mutex, adds bytes, unlocks mutex.
	bool Send(const uint8* Bytes, uint32 Size);

	// Burst system - Locks the sending mutex.
	void BurstBegin() { m_wMtx.lock(); }

	// Burst system - Adds bytes to output buffer.
	bool BurstSend(const uint8* Bytes, uint32 Size);

	// Burst system - Pushes event to queue - do at the end of write events.
	void BurstPush();

	// Burst system - Unlocks the sending mutex.
	void BurstEnd() { m_wMtx.unlock(); }

	/* Platform-specific methods */
	void SetupReadEvent();
	void ReadCallback(uint32 len);
	void WriteCallback();


private:
	SOCKET m_fd;
	OverlappedRec m_readEvent;
	OverlappedRec m_writeEvent;
	HANDLE m_completionPort;// IOCP handle
	std::mutex m_wMtx, m_rMtx;
};

/** Connect to a server.
* @param hostname Hostname or IP address to connect to
* @param port Port to connect to
* @return templated type if successful, otherwise null
*/

template<typename T>
T* connectTCPSocket(const char* host, u_short port) {

	SOCKET fd = open_clientfd(host, std::to_string(port).c_str());
	if (fd) {
		T* s = new T{};
		if (s) {
			return s;
		}
	}
	return nullptr;
}