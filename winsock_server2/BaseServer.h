#pragma once

#include "BaseServerDef.h"
#include <vector>
#include <thread>

class BaseServer{
public:
	int Init(const char *fileName);
	int Deinit();

	//
	void ServerWorkerThread();
private:
	//
	int PostAccept();

	//
	HANDLE CompletionPort_;
	WSADATA wsd_;
	SYSTEM_INFO systemInfo_;
	SOCKET listenSocket_;
	SOCKADDR_IN internetAddr_;
	std::vector<std::thread> workerThreads_;

	//
	LPFN_ACCEPTEX lpfnAcceptEx_;
	LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockAddrs_;
};
