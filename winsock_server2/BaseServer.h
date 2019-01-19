#pragma once

#include "BaseServerDef.h"
#include <vector>
#include <thread>

class BaseServer{
public:
	//
	int Init(const char *fileName);
	int Deinit();

	//
	void ServerWorkerThread();
private:
	//
	int OnReceiveData(LPPER_HANDLE_DATA);

	//
	int PostAccept();
	int PostRecvOnAccept(LPPER_IO_OPERATE_DATA);
	int PostRecvOnRecv(LPPER_IO_OPERATE_DATA);

	//
	int ParseCheckSum(const char*buf, int checkSum) const;
	int ParseTypeName(const char*buf, std::string&typeName) const;
	int ParseBinProto(const char*src, std::vector<char>&binProto) const;

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
