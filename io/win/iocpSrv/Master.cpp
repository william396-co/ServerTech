#include "Master.hpp"

#include <signal.h>
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
#include "print.hpp"
#include "joining_thread.hpp"

#pragma comment(lib,"ws2_32.lib")

constexpr auto MAX_BUFF_SIZE = 1024;
constexpr auto port = 9527;

using ListenHandle = ListenSocket<ServerSocket>;

Master::Master() :m_stopEvent{ false } {

}

Master::~Master() {

}

bool Master::Run(int argc, char** argv)
{
	// Init WSADATA
	SocketMgr::instance();

	std::unique_ptr<ListenHandle> listenfd
		= std::make_unique<ListenSocket<ServerSocket>>("127.0.0.1", port);

	// Main Loop for Application

	auto MainLoop = [&]() {
		time_t start = time(nullptr);
		time_t curr = time(nullptr);
		while (!m_stopEvent) {

			if (time(nullptr) - start > 1) {			// print per second
				printlnEx("Server running [", std::this_thread::get_id(), "]");
				curr = time(nullptr);
			}

			if (time(nullptr) - start > 5) {
				m_stopEvent = true;
			}
		}

		listenfd->Close();
		SocketMgr::instance().CloseAll();
		SocketMgr::instance().ShutdownThreads();
		printlnEx("finished mainLoop [", std::this_thread::get_id(),"]");
	};

	// listen thread working
	joining_thread listenRun(&ListenHandle::run, listenfd.get());
	
	// Main Loop for Application
	joining_thread mainRun(MainLoop);
	
	// IOCP workthread start	
	SocketMgr::instance().SpawnWorkerThreads();

	
	printlnEx("finished all [",std::this_thread::get_id(),"]");

	return true;
}

void Master::_OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
		//sWorld.Rehash(true);
		break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		m_stopEvent = true;
		break;
	}

	//signal(s, _OnSignal);
}