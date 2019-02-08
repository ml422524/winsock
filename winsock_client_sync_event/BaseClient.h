#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#define EXE_SUCCESS 0
#define EXE_FAIL -1

class BaseClient{
public:
	BaseClient() : svrSocket_(INVALID_SOCKET), sockEvent_(NULL), totalEvents_(0) {}

	//
	int Init(const char*configFile);
	int DoLoop();
private:
	SOCKET svrSocket_;
	WSAEVENT sockEvent_;
	int totalEvents_;
	SOCKADDR_IN svrAddr_;
};
