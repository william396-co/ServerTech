#pragma once
#include <chrono>

namespace utils {

	using namespace std::chrono;
	using namespace std::chrono_literals;
	// time second
	time_t now() {
		return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
	}

	// millisecond
	time_t now_ms() {
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}

	// microseconds
	time_t now_us() {
		return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
	}

	// nanoseconds
	time_t now_ns() {
		return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
	}
}