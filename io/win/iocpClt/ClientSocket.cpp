#include "ClientSocket.hpp"

ClientSocket::ClientSocket() :Socket{INVALID_SOCKET,1024*64,1024*512},
	m_remaining{0},
	m_opcode{0},
	m_last_ping{time(nullptr)}
{

}

ClientSocket::~ClientSocket()
{

}