#include "SocketMgr.hpp"
#include <stdexcept>
#include <thread>
#include <algorithm>
#include <functional>
#include <iostream>

#include "print.hpp"

void HandleReadComplete(Socket* s, uint32 len) {

	if (!s->IsDeleted()) {
		s->getReadEvent().Unmark();
		if (len) {
			s->getReadBuffer().IncrementWritten(len);
			s->OnRead();
			s->SetupReadEvent();
		}
		else {
			s->Delete();
		}
	}
}

void HandleWriteComplete(Socket* s, uint32 len) {

	if (!s->IsDeleted()) {
		s->getWriteEvent().Unmark();
		s->BurstBegin();
		s->getWriteBuffer().Remove(len);
		if (s->getWriteBuffer().GetContiguiousBytes() > 0)
			s->WriteCallback();
		else
			s->DecSendLock();
		s->BurstEnd();
	}
}

// 工作线程
void WorkerRun(HANDLE iocpHandle) {

	DWORD len;
	Socket* s;
	OverlappedRec* ov;
	LPOVERLAPPED ol_ptr;

	while (true) {

		if (!GetQueuedCompletionStatus(iocpHandle, &len, (PULONG_PTR)&s, &ol_ptr, INFINITE))
			continue;

		ov = CONTAINING_RECORD(ol_ptr, OverlappedRec, overlap);
		if (ov->event == SockeIOEvent::SOCKET_IO_THREAD_SHUT_DOWN) {	
			printlnEx("finished workthread [", std::this_thread::get_id(), "]");
			delete ov;
			break;// finished this thread
		}

		switch (ov->event)
		{
		case SockeIOEvent::SOCKET_IO_EVENT_READ_COMPLETE:
			HandleReadComplete(s, len);
			break;
		case SockeIOEvent::SOCKET_IO_EVENT_WRITE_END:
			HandleWriteComplete(s, len);
			break;
		default:
			break;
		}
	}
}


SocketMgr::SocketMgr() :threadCnt{ 0 } {

	WSADATA wsaData;
	if (int ret = WSAStartup(MAKEWORD(2, 0), &wsaData)) {
		throw "WSAStartup failed";
	}
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
}

SocketMgr::~SocketMgr()
{
	WSACleanup();
}

void SocketMgr::CloseAll()
{
	std::vector<Socket*> toKill;
	toKill.reserve(socketList.size());
	size_t i = 0;
	std::unique_lock<std::mutex> lock(sockMtx);
	for (auto const& it : socketList) {
		toKill[i++] = it;
	}
	lock.unlock();

	for (auto it : toKill) {
		it->Disconnect();
	}
}

void SocketMgr::SpawnWorkerThreads() {
	
	/* 一般地，我们创建CPU核心数*2的工作线程，
	*  使得在某个线程 Sleep() 或WSAWaitForMultipleEvents() 将线程挂起时（此时不占用CPU时间片），
	*  CPU的内核仍旧有线程在工作，减少线程调度的时间，保证程序的执行效率）。*/

	threadCnt = std::thread::hardware_concurrency() * 2;//CpuNum * 2

	for (int i = 0; i != threadCnt; ++i) {
		workerThreads.emplace_back(WorkerRun, m_completionPort);
	}
	std::for_each(workerThreads.begin(), workerThreads.end(), std::mem_fn(&std::thread::join));
}

void SocketMgr::ShutdownThreads() {
	for (int i = 0; i != threadCnt; ++i) {
		OverlappedRec* ov = new OverlappedRec(SockeIOEvent::SOCKET_IO_THREAD_SHUT_DOWN);
		PostQueuedCompletionStatus(m_completionPort, 0, (ULONG_PTR)0, &ov->overlap);
	}
}