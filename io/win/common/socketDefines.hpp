#pragma once

#include "Platform.hpp"
#include <atomic>

#define USE_IOCP

// complete event type
enum class SockeIOEvent
{
	SOCKET_IO_EVENT_READ_COMPLETE,
	SOCKET_IO_EVENT_WRITE_END,
	SOCKET_IO_THREAD_SHUT_DOWN,
	NUM_SOCKET_IO_EVENTS
};

// overlapped Recored
struct OverlappedRec
{
	OVERLAPPED overlap;
	SockeIOEvent event;
	std::atomic<size_t> inUse;

	OverlappedRec(SockeIOEvent ev) :event {ev}, inUse{ 0 }
	{
		memset(&overlap, 0, sizeof(overlap));
	}

	inline void Reset(SockeIOEvent ev) {
		memset(&overlap, 0, sizeof(overlap));
		event = ev;
	}

	void Mark() {
		inUse.store(1, std::memory_order_acq_rel);//TODO 
	}
	void Unmark() {
		inUse.store(0, std::memory_order_acq_rel);
	}
};


