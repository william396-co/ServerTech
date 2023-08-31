#pragma once

#include "Platform.hpp"
#include "Socket.hpp"
#include "PacketBuffer.hpp"

#include <atomic>

#pragma pack(push, 1)
struct MsgHeader
{
	uint16 opcode;
	uint32 size;

	MsgHeader(uint16 op, uint32 sz) :opcode{ op }, size(sz) {}
};
#pragma pack(pop)

class ServerSocket : public Socket
{
public:
	ServerSocket(SOCKET fd);
	~ServerSocket();

	virtual void OnRead()override;
	virtual void OnDisconnect()override;
	virtual void OnConnect()override;

	void SendPacket(PacketBuffer* data);
	void HandlePacket(PacketBuffer& recvData);

	void HandlePing(PacketBuffer& recvData);
	void HandleMessage(PacketBuffer& recvData);
private:
	time_t m_last_ping;
	uint32 m_remaining;
	uint16 m_opcode;
};