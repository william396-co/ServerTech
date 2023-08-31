#include "ServerSocket.hpp"
#include "OpCode.hpp"

ServerSocket::ServerSocket(SOCKET fd)
	:Socket(fd, 1024 * 64, 1024 * 512),
	m_remaining{ 0 },
	m_opcode{ 0 },
	m_last_ping{ time(nullptr) }
{

}

ServerSocket::~ServerSocket()
{

}

void ServerSocket::OnRead() {
	while (true) {

		if (!m_remaining) {

			if (m_readBuffer.GetSize() < 6)
				return; // no header

			m_readBuffer.Read((uint8*)&m_opcode, 2);
			m_readBuffer.Read((uint8*)&m_remaining, 4);

			// byteorder from netorder to local byteorder
		}

		if (m_readBuffer.GetSize() < m_remaining)
			return;

		PacketBuffer buff{ m_opcode,m_remaining };
		if (m_remaining) {
			buff.resize(m_remaining);
			m_readBuffer.Read((uint8*)buff.contents(), m_remaining);
		}

		HandlePacket(buff);
		m_remaining = 0;
		m_opcode = 0;
	}
}

void ServerSocket::OnDisconnect() {

}

void ServerSocket::OnConnect() {

}

void ServerSocket::SendPacket(PacketBuffer* data) {
	MsgHeader header(data->GetOpcode(), (uint32)data->size());
	Send((uint8*)&header, sizeof(header)) && Send(data->contents(), (uint32)data->size());
}

void ServerSocket::HandlePacket(PacketBuffer& recvData) {

	switch (recvData.GetOpcode())
	{
	case RCMSG_PING:
		HandlePing(recvData); break;
	case RCMSG_MSG:
		HandleMessage(recvData);
		break;
	default:
		break;
	}
}

void ServerSocket::HandlePing(PacketBuffer& recvData) {

	PacketBuffer data(RSMSG_PING, 4);
	SendPacket(&data);
	m_last_ping = time(nullptr);
}

void ServerSocket::HandleMessage(PacketBuffer& recvData) {

}