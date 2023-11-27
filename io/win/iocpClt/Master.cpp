#include "Master.hpp"

#include <signal.h>
#include <functional>
#include <utility>
#include <memory>
#include <iostream>
#include <thread>
#include "ClientSocket.hpp"
#include "SocketMgr.hpp"
#include "Socket.hpp"
#include "ListenSocket.hpp"
#include "thread_guard.hpp"
#include "print.hpp"
#include "joining_thread.hpp"
#include "chrono_util.hpp"

#pragma comment(lib,"ws2_32.lib")

constexpr auto MAX_BUFF_SIZE = 1024;
constexpr auto hostname = "172.28.10.59";
constexpr auto port = 9527;

Master::Master() :m_stopEvent{ false } {

}

Master::~Master() {

}

bool Master::Run(int argc, char** argv)
{
	// Init WSADATA
	SocketMgr::instance();

	std::unique_ptr<ClientSocket> client = std::make_unique<ClientSocket>();
	if (!client->Connect(hostname, port)) {
		std::cerr << "connect[" << hostname << ":" << port << "] failed\n";
		return 0;
	}

	std::string buf;
	while (getline(std::cin, buf)) {
		client->Send((uint8*)buf.data(), (uint32)buf.size());
	}

	// IOCP workthread start	
	SocketMgr::instance().SpawnWorkerThreads();


	//	printlnEx("finished all [",std::this_thread::get_id(),"]");

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