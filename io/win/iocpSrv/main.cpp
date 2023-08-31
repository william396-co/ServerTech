#include <functional>
#include <utility>
#include <memory>
#include <iostream>
#include <thread>
#include "ServerSocket.hpp"
#include "SocketMgr.hpp"
#include "Socket.hpp"
#include "ListenSocket.hpp"
#include "thread_guard.hpp"

#pragma comment(lib,"ws2_32.lib")

constexpr auto MAX_BUFF_SIZE = 1024;
constexpr auto port = 9527;

void MainLoop() {
	while (true) {

		std::cout << "Server running\n";
	}
}

int main()
{
	// Init WSADATA
	SocketMgr::instance();

	std::unique_ptr<ListenSocket<ServerSocket> > sl
		= std::make_unique<ListenSocket<ServerSocket>>("127.0.0.1", port);

	// listen thread working
	std::thread listenRun(&ListenSocket<ServerSocket>::run, sl.get());
	thread_guard glistenRun(listenRun);

	// Main Loop for Application
	std::thread mainRun(&MainLoop);
	thread_guard gmainRun(mainRun);

	// IOCP workthread start	
	SocketMgr::instance().SpawnWorkerThreads();

	return 0;
}
