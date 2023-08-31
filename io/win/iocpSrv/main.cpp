#include <functional>
#include <utility>
#include <memory>
#include <iostream>
#include <thread>
#include "ServerSocket.hpp"
#include "SocketMgr.hpp"
#include "Socket.hpp"
#include "ListenSocket.hpp"
//#include "threadpool.hpp"

#pragma comment(lib,"ws2_32.lib")

constexpr auto MAX_BUFF_SIZE = 1024;
constexpr auto port = 9527;

int main()
{
	//ThreadPool pool(10);
	//pool.init();

	std::unique_ptr<ListenSocket<ServerSocket> > sl
		= std::make_unique<ListenSocket<ServerSocket>>("127.0.0.1", port);

	// IOCP workthread start
	SocketMgr::instance().SpawnWorkerThreads();

	// listen thread working
	std::thread th(&ListenSocket<ServerSocket>::run, sl.get());
	th.join();

	while (true) {

		std::cout << "Server running\n";
	}

	//pool.shutdown();

	return 0;
}
