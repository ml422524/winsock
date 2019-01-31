#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>

//
typedef u_long DATA_LEN_TYPE;

//
#define DATA_HEAD_LEN     sizeof(DATA_LEN_TYPE)
#define DATA_BUFSIZE      (1024*8)
#define DATA_MAX_LEN       (2 * DATA_BUFSIZE)
#define DATA_RECV_BUFSIZE (DATA_MAX_LEN + DATA_HEAD_LEN)
#define TYPENAME_LEN 32

//
struct MsgHead{
	int nameLen;
	char name[TYPENAME_LEN];
	int checkSum; // a check  value of protobuf data.
};

//typedef char BYTE;
//typedef char *PBYTE;

enum OpeType{
	RECV_OPE = 1,
	SEND_OPE = 2,
	ACCEPT_OPE = 3
};

typedef struct _PER_HANDLE_DATA
{
	SOCKET socketClient;
	SOCKADDR_IN clientAddr;
	char buf[DATA_RECV_BUFSIZE];
	int dataLen;
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

//
enum CallBackErrorCode{
	CALL_BACK_EXIST = 0,
	CALL_BACK_REG_SUCCESS
};

