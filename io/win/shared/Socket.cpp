#include "Socket.hpp"
#include "SocketOps.hpp"
#include "SocketMgr.hpp"
#include <ws2def.h>
#include <iostream>

Socket::Socket(SOCKET fd, uint32 sendbufSize, uint32 recvBufSize)
	:m_fd(fd),
	m_BytesRecved(0),
	m_BytesSent(0),
	m_completionPort(0),
	m_connected(false),
	m_deleted(false),
	m_client()
{
	m_readBuffer.Allocate(recvBufSize);
	m_writeBuffer.Allocate(sendbufSize);

	m_fd = (m_fd == 0) ? SocketOps::CreateTCPFileDescriptor() : m_fd;
}

bool Socket::Connect(const char* Address, uint32 Port) {

	m_fd = open_clientfd(Address, std::to_string(Port).c_str());
	if (m_fd <= 0) {
		return false;
	}

	// at this point the connection was established
#ifdef CONFIG_USE_IOCP
	m_completionPort = SocketMgr::instance().GetCompletionPort();
#endif

	_OnConnect();
	return true;
}

void Socket::Accept(sockaddr_in* address) {
	memcpy(&m_client, address, sizeof(sockaddr_in));
	_OnConnect();
}

void Socket::_OnConnect() {

	SocketOps::Nonblocking(m_fd);
	SocketOps::DisableBuffering(m_fd);

	m_connected = true;

	// IOCP stuff
	AssignToCompletionPort();
	SetupReadEvent();

	SocketMgr::instance().AddSocket(this);

	// call virtual onConnect
	OnConnect();
}

bool Socket::Send(const uint8* Bytes, uint32 Size) {

	bool rv = false;
	std::lock_guard<std::mutex> lock(m_writeMutex);
	rv = m_writeBuffer.Write(Bytes, Size);
	if (rv)
		BurstPush();
	return rv;
}

bool Socket::BurstSend(const uint8* Bytes, uint32 Size) {
	return m_writeBuffer.Write(Bytes, Size);
}

void Socket::BurstPush()
{
	if (AcquireSendLock())
		WriteCallback();
}

void Socket::Disconnect()
{
	if (!m_connected)
		return;

	m_connected = false;

	printEx("Socket::Disconnect on socket ", m_fd);

	SocketMgr::instance().DelSocket(this);
	// call virtual onDisconnect
	OnDisconnect();

	if (!IsDeleted())
		Delete();	
}

void Socket::Delete() {

	if (!m_deleted)return;

	m_deleted = true;

	printEx("Socket::Delete on socket ", m_fd);

	if (IsConnected())
		Disconnect();

	SocketOps::CloseSocket(m_fd);
}

void Socket::WriteCallback()
{
	if (IsDeleted() || !IsConnected())
		return;

	std::lock_guard<std::mutex> lock(m_writeMutex);

	if (m_writeBuffer.GetContiguiousBytes()) {

		DWORD wlen = 0;
		DWORD flags = 0;
		WSABUF buf;
		buf.len = (ULONG)m_writeBuffer.GetContiguiousBytes();
		buf.buf = (char*)m_writeBuffer.GetBufferStart();

		m_writeEvent.Mark();
		m_writeEvent.Reset(SockeIOEvent::SOCKET_IO_EVENT_WRITE_END);
		if (int r = WSASend(m_fd, &buf, 1, &wlen, flags, &m_writeEvent.overlap, 0) == SOCKET_ERROR) {

			if (WSAGetLastError() != WSA_IO_PENDING) {
				m_writeEvent.Unmark();
				DecSendLock();//??
				Disconnect();
			}
		}
		m_BytesSent += wlen;
	}
	else {
		DecSendLock();
	}
}

void Socket::SetupReadEvent()
{
	if (IsDeleted() || !IsConnected())
		return;

	std::lock_guard<std::mutex> lock(m_readMutex);
	DWORD rlen = 0;
	DWORD flags = 0;
	WSABUF buf;
	buf.len = (ULONG)m_readBuffer.GetSpace();
	buf.buf = (char*)m_readBuffer.GetBuffer();

	m_readEvent.Mark();
	m_readEvent.Reset(SockeIOEvent::SOCKET_IO_EVENT_READ_COMPLETE);
	if (WSARecv(m_fd, &buf, 1, &rlen, &flags, &m_readEvent.overlap, 0) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			m_readEvent.Unmark();
			Disconnect();
		}
	}
	m_BytesRecved += rlen;
}