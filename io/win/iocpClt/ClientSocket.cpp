#include "ClientSocket.hpp"
#include "print.hpp"
#include "../common/OpCode.hpp"

ClientSocket::ClientSocket() :Socket{ INVALID_SOCKET,1024 * 64,1024 * 512 },
m_remaining{ 0 },
m_opcode{ 0 },
m_last_ping{ time(nullptr) }
{

}

ClientSocket::~ClientSocket()
{

}

void ClientSocket::OnRead()
{
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


void ClientSocket::OnDisconnect() {

	println(__FUNCTION__, GetRemoteAddress(), GetRemotePort());
}

void ClientSocket::OnConnect() {

	println(__FUNCTION__, GetRemoteAddress(), GetRemotePort());
}

void ClientSocket::HandlePacket(PacketBuffer& recvData) {

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

void ClientSocket::HandleMessage(PacketBuffer& recvData) {

}

void ClientSocket::SendPacket(PacketBuffer* data) {
	MsgHeader header(data->GetOpcode(), (uint32)data->size());
	Send((uint8*)&header, sizeof(header)) && Send(data->contents(), (uint32)data->size());
}

void ClientSocket::HandlePing(PacketBuffer& recvData) {

}
