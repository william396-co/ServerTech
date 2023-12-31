#pragma once
#include "singleton.hpp"
#include "Socket.hpp"
#include "type.hpp"

#include <vector>
#include <atomic>
#include <set>
#include <mutex>
#include <Windows.h>
#include "joining_thread.hpp"

class SocketMgr : public Singleton<SocketMgr>
{
public:
	SocketMgr();
	~SocketMgr();

	inline HANDLE GetCompletionPort()const { return m_completionPort; }
	void CloseAll();
	inline void AddSocket(Socket* s) {
		std::lock_guard<std::mutex> lock(sockMtx);
		socketList.emplace(s);
	}
	inline void DelSocket(Socket* s) {
		std::lock_guard<std::mutex> lock(sockMtx);
		socketList.erase(s);
	}
	void SpawnWorkerThreads();
	void ShutdownThreads();
private:
	HANDLE m_completionPort;
	std::set<Socket*> socketList;
	std::mutex sockMtx;
	int threadCnt;
	std::vector<joining_thread> workerThreads;
};