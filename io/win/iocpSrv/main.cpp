

#include "../common/util.h"
#include <unordered_set>
#include <vector>
#include <thread>
#include <atomic>
#include <algorithm>
#include <functional>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

constexpr auto MAX_BUFF_SIZE = 1024;
const char* port = "9527";

typedef struct _PER_IO_OPERATION_DATA {
	WSAOVERLAPPED Overlapped;
	WSABUF DataBuf;
	char Buffer[MAX_BUFF_SIZE];
	DWORD ByteSend;
	DWORD ByteRecv;
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;


DWORD getThreadNum()
{
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	// threadNum = cpuNum * 2
	return  system_info.dwNumberOfProcessors * 2;
}

bool initWsaData() {
	WSAData wsaData;
	if (int ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		std::cerr << "WSAStartup failed: " << ret << "\n";
		return false;
	}
	return true;
}

void done()
{
	WSACleanup();
}
using SocketMap = std::unordered_set<SOCKET>;
SocketMap socketList;

void listenFunc(SOCKET listenfd, HANDLE iocpHandle) {

	while (true) {

		sockaddr_in clientAddr;
		int addrlen;
		SOCKET clientfd = accept(listenfd, (sockaddr*)&clientAddr, &addrlen);
		if (clientfd == INVALID_SOCKET) {
			std::cerr << "accept failed:" << WSAGetLastError() << "\n";
			continue;
		}


		/* bind clientfd to IOCP Handle */
		HANDLE clientHandle = CreateIoCompletionPort((HANDLE)clientfd, iocpHandle, 0, 0);
		if (clientHandle == NULL) {
			std::cerr << "createIoCompletionPort failed:" << GetLastError() << "\n";
			closesocket(clientfd);
			continue;
		}


		// Create Overlapped
		LPPER_IO_OPERATION_DATA perIoData = new PER_IO_OPERATION_DATA{};
		memset(&(perIoData->Overlapped), 0, sizeof(perIoData->Overlapped));
		perIoData->DataBuf.len = MAX_BUFF_SIZE;
		perIoData->DataBuf.buf = perIoData->Buffer;

		DWORD byteRecv = 0;
		DWORD flags = 0;

		// recieve data from client
		if (WSARecv(clientfd, &(perIoData->DataBuf), 1, &byteRecv, &flags, &(perIoData->Overlapped), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				std::cerr << "WSARecv failed:" << WSAGetLastError << "\n";
				delete perIoData;
				closesocket(clientfd);
				continue;
			}
		}

		// add client socket to map
		socketList.emplace(clientfd);
		std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "\n";
	}

	CloseHandle(iocpHandle);
	closesocket(listenfd);
}

void wokerFunc(HANDLE iocpHandle) {

	LPOVERLAPPED overlapped = nullptr;

	DWORD recv_bytes = 0;
	ULONG_PTR cl_key = 0;
	while (true) {
		// block mode
		BOOL ret = GetQueuedCompletionStatus(iocpHandle, &recv_bytes, &cl_key, &overlapped, INFINITE);

		if (!ret && ERROR_NETNAME_DELETED == GetLastError()) {
			break;
		}
	}
}

int main()
{
	std::cout << "Hello CMake." << endl;

	atexit(done);

	if (!initWsaData()) {
		return 1;
	}


	DWORD threadNum = getThreadNum();

	SOCKET listenfd = open_listenfd(port);
	if (listenfd == INVALID_SOCKET) {
		std::cerr << "open_listenfd failed!\n";
		return 1;
	}

	/* Create IOCP Handle */
	HANDLE iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, threadNum);
	if (iocpHandle == NULL) {
		std::cout << "CreateIoCompletionPort failed:" << GetLastError() << "\n";
		return 1;
	}

	/* Bind socket to IOCP Handle */
	if (CreateIoCompletionPort((HANDLE)listenfd, iocpHandle, 0, 0) == NULL) {
		CloseHandle(iocpHandle);
		closesocket(listenfd);
		return 1;
	}
	
	std::cout << "Server start\n";

	std::thread lt(listenFunc,listenfd, iocpHandle);
	lt.join();
		
	std::vector<std::thread> workthreads;
	for (DWORD i = 0; i != threadNum;++i) {
		workthreads.emplace_back(wokerFunc, iocpHandle);
	}
	std::for_each(workthreads.begin(), workthreads.end(), std::mem_fn(&std::thread::join));
		
	return 0;
}
