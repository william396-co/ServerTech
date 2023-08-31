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
		while (!m_stopEvent) {
			std::cout << "Server running\n";

		}

		listenfd->Close();
		SocketMgr::instance().CloseAll();
		SocketMgr::instance().ShutdownThreads();
	};

	// listen thread working
	std::thread listenRun(&ListenHandle::run, listenfd.get());
	thread_guard glistenRun(listenRun);

	// Main Loop for Application
	std::thread mainRun(MainLoop);
	thread_guard gmainRun(mainRun);

	// IOCP workthread start	
	SocketMgr::instance().SpawnWorkerThreads();
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
		Master::m_stopEvent = true;
		break;
	}

	//signal(s, _OnSignal);
}