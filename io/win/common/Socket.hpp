#pragma once

#include "Platform.hpp"
#include "CircularBuffer.hpp"
#include "socketDefines.hpp"
#include "util.hpp"

#include <mutex>
#include <string>

class Socket
{
public:
	// Constructor. If fd = 0, it will be assigned
	Socket(SOCKET fd, uint32 sendBufSize, uint32 recvBufSize);

	// Destructor
	virtual ~Socket() {}

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
	inline void BurstBegin() { m_wMtx.lock(); }

	// Burst system - Adds bytes to output buffer.
	bool BurstSend(const uint8* Bytes, uint32 Size);

	// Burst system - Pushes event to queue - do at the end of write events.
	void BurstPush();

	// Burst system - Unlocks the sending mutex.
	inline void BurstEnd() { m_wMtx.unlock(); }

	/* Platform-specific methods */
	void SetupReadEvent();
	void ReadCallback(uint32 len);
	void WriteCallback();

	// Set completion port that this socket will be assigned to.
	inline void SetCompletionPort(HANDLE cp) {
		m_completionPort = cp;
	}
	// Assigns the socket to his completion port.(bind fd to completionPort)
	inline void AssignToCompletionPort() {
		if (m_fd && m_completionPort) {
			CreateIoCompletionPort(HANDLE(m_fd), m_completionPort, (ULONG_PTR)this, 0);
		}
	}

	inline const sockaddr_in& GetRemoteStruct()const { return m_client; }
	inline const in_addr& GetRemoteAddress()const { return m_client.sin_addr; }
	inline const SOCKET& GetFd()const { return m_fd; }
	inline uint32 GetRemotePort()const { return ntohs(m_client.sin_port); }
	void Delete();

	inline void IncSendLock() { m_writeLock.fetch_add(1); }
	inline void DecSendLock() { m_writeLock.fetch_sub(1); }
	inline bool AcquireSendLock() {
		return m_writeLock.exchange(1, std::memory_order_acq_rel) != 0 ? false : true;
	}
	inline OverlappedRec& getReadEvent() { return m_readEvent; }
	inline OverlappedRec& getWriteEvent() { return m_writeEvent; }
	inline CircularBuffer& getReadBuffer() { return m_readBuffer; }
	inline CircularBuffer& getWriteBuffer() { return m_writeBuffer; }
protected:
	void _OnConnect();

	size_t m_BytesSent, m_BytesRecved;
	sockaddr_in m_client;
	std::mutex m_wMtx, m_rMtx;

	CircularBuffer m_readBuffer;
	CircularBuffer m_writeBuffer;
private:
	SOCKET m_fd;
	OverlappedRec m_readEvent;
	OverlappedRec m_writeEvent;
	HANDLE m_completionPort;// IOCP handle
	std::atomic<int> m_writeLock;
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