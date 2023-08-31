#pragma once
#include <atomic>

/* Implementation Selection */
#ifdef WIN32		// Easy
#define CONFIG_USE_IOCP

// complete event type
enum class SockeIOEvent
{
	SOCKET_IO_NONE,
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
	std::atomic_flag inUse = ATOMIC_FLAG_INIT;

	OverlappedRec(SockeIOEvent ev) :event {ev}, inUse{ 0 }
	{
		memset(&overlap, 0, sizeof(overlap));
	}

	OverlappedRec()
		:OverlappedRec(SockeIOEvent::SOCKET_IO_NONE) {}

	inline void Reset(SockeIOEvent ev) {
		memset(&overlap, 0, sizeof(overlap));
		event = ev;
	}

	void Mark() {		
		while (inUse.test_and_set(std::memory_order_acq_rel));
	}
	void Unmark() {
		inUse.clear(std::memory_order_relaxed);
	}
};


#endif