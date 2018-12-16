#pragma once

#include <WinSock2.h>
#include <Windows.h>

#define DATA_BUFSIZE 1024

enum OpeType{
	RECV_POSTED = 1,
	SEND_POSTED = 2
};

typedef struct _PER_HANDLE_DATA
{
	SOCKET socket;
	SOCKADDR_STORAGE clientAddr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct _PER_IO_OPERATE_DATA{
	OVERLAPPED overlapped;
	WSABUF dataBuf;
	char buf[DATA_BUFSIZE];
	int  bufLen;
	DWORD bytesRecv;
	DWORD flags;
	int operationType;
}PER_IO_OPERATE_DATA, *LPPER_IO_OPERATE_DATA;