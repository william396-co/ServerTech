#pragma once

#include "ByteBuffer.hpp"

constexpr auto DEFAULT_PACKET_SIZE = 128u;

class PacketBuffer : public ByteBuffer
{
public:
	inline PacketBuffer() : ByteBuffer(DEFAULT_PACKET_SIZE), m_opcode(0) { }
	inline PacketBuffer(uint16 opcode, size_t res) : ByteBuffer(res), m_opcode(opcode) {}
	inline PacketBuffer(size_t res) : ByteBuffer(res), m_opcode(0) { }
	inline PacketBuffer(const PacketBuffer& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode) {}

	//! Clear packet and set opcode all in one mighty blow
	inline void Initialize(uint16 opcode)
	{
		clear();
		m_opcode = opcode;
	}

	inline uint16 GetOpcode() const { return m_opcode; }
	inline void SetOpcode(uint16 opcode) { m_opcode = opcode; }

protected:
	uint16 m_opcode;
};
