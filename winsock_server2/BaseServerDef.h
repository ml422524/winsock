#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>

#define DATA_BUFSIZE 1024

enum OpeType{
	RECV_POSTED = 1,
	SEND_POSTED = 2,
	ACCEPT_POSTED = 3
};

typedef struct _PER_HANDLE_DATA
{
	SOCKET socketClient;
	SOCKADDR_IN clientAddr;
	char buf[2 * DATA_BUFSIZE];
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct _PER_IO_OPERATE_DATA{
	OVERLAPPED overlapped;
	WSABUF dataBuf;
	char buf[DATA_BUFSIZE];
	int  bufLen;
	DWORD bytesRecv;
	DWORD bytesSend;
	DWORD flags;
	int operationType;
	SOCKET socketClient;
}PER_IO_OPERATE_DATA, *LPPER_IO_OPERATE_DATA;