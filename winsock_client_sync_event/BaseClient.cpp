#include "BaseClient.h"
#include <cstdio>
#include <assert.h>
#include "../proto/msg.pb.h"

//
#pragma comment(lib, "ws2_32.lib")

//
typedef u_long DATA_LEN_TYPE;
#define DATA_HEAD_LEN     sizeof(DATA_LEN_TYPE)
#define DATA_BUFSIZE      512
#define DATA_MAX_LEN       (2 * DATA_BUFSIZE)
#define DATA_RECV_BUFSIZE (DATA_MAX_LEN + DATA_HEAD_LEN)
#define TYPENAME_LEN 32
struct MsgHead{
	int nameLen;
	char name[TYPENAME_LEN];
	int checkSum; // a check  value of protobuf data.
};

int sendHelloProto(SOCKET sSocket)
{
	//
	Protocol::Hello hello;
	hello.set_content("helloserver");
	std::string  typeName = Protocol::Hello::descriptor()->full_name();

	//
	std::string buffer;
	hello.SerializeToString(&buffer);

	//
	Protocol::Hello parseHello;
	parseHello.ParseFromString(buffer);

	//
	MsgHead head;
	head.nameLen = typeName.size();
	strcpy(head.name, typeName.c_str());
	head.name[head.nameLen] = '\0';
	head.checkSum = 10;

	//
	head.nameLen = htonl(head.nameLen);
	head.checkSum = htonl(head.checkSum);

	//
	const int msgLen = sizeof(head) + buffer.size();
	const int totalLen = DATA_HEAD_LEN + msgLen;
	char *pdata = new char[totalLen];

	//
	const int netMsgLen = htonl(msgLen);
	memcpy(pdata, &netMsgLen, DATA_HEAD_LEN);
	memcpy(pdata + DATA_HEAD_LEN, &head, sizeof(head));
	memcpy(pdata + DATA_HEAD_LEN + sizeof(head), buffer.c_str(), buffer.size());

	//
	int bytes = send(sSocket, pdata, totalLen, 0);
	if (SOCKET_ERROR == bytes){
		printf("send failed");
	}
	//
	delete pdata;

	//
	return 0;
}

//
int BaseClient::Init(const char* configFile)
{
	//
	WSADATA wsd;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsd))
	{
		printf("WSAStartup failed.\n");
		return EXE_FAIL;
	}

	//
	svrSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == svrSocket_){
		printf("socker failed.\n");
		WSACleanup();
		return EXE_FAIL;
	}

	//
	sockEvent_ = WSACreateEvent();
	if (NULL == sockEvent_)
	{
		printf("create event failed.\n");
		closesocket(svrSocket_);
		WSACleanup();
		return EXE_FAIL;
	}
	totalEvents_++;

	//设置服务器地址
	svrAddr_.sin_family = AF_INET;
	svrAddr_.sin_addr.s_addr = inet_addr("127.0.0.1");
	svrAddr_.sin_port = htons((short)27015);
	
	//连接服务器
	int retVal = connect(svrSocket_, (LPSOCKADDR)&svrAddr_, sizeof(svrAddr_));
	if (SOCKET_ERROR == retVal){
		printf("connect failed, error: %d.\n", WSAGetLastError());
		closesocket(svrSocket_);
		WSACleanup();
		return EXE_FAIL;
	}

	//
	if (SOCKET_ERROR == WSAEventSelect(svrSocket_, sockEvent_, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE))
	{
		printf("WSAEventSelect error: %d.\n", WSAGetLastError());
		closesocket(svrSocket_);
		WSACleanup();
		return EXE_FAIL;
	}
	//
	return EXE_SUCCESS;
}

//
int BaseClient::DoLoop()
{
	WSANETWORKEVENTS wsaNetworkEvent;
	for (;;)
	{
		auto retVal = WSAWaitForMultipleEvents(totalEvents_, &sockEvent_, FALSE, WSA_INFINITE, FALSE);
		if (WSA_WAIT_FAILED == retVal)
		{
			int error = WSAGetLastError();
			printf("WSAWaitForMultipleEvents error: %d.\n", error);
			continue;
		}
		
		//
		if (WSA_WAIT_EVENT_0 <= retVal && retVal < WSA_WAIT_EVENT_0 + totalEvents_)
		{
			// reset event.
			WSAResetEvent(sockEvent_);

			//
			int retVal = WSAEnumNetworkEvents(svrSocket_, sockEvent_, &wsaNetworkEvent);
			if (0 == retVal)
			{
				if (wsaNetworkEvent.lNetworkEvents & FD_CONNECT)
				{
					if (wsaNetworkEvent.iErrorCode[FD_CONNECT_BIT] != 0)
					{
						printf("FD_CONNECT error: %d.\n", wsaNetworkEvent.iErrorCode[FD_CONNECT_BIT]);
					}
					else
					{
						printf("FD_CONNECT success.\n");
						sendHelloProto(svrSocket_);
					}
				}
				else if (wsaNetworkEvent.lNetworkEvents & FD_CLOSE)
				{
					if (wsaNetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
					{
						printf("FD_CLOSE error: %d.\n", wsaNetworkEvent.iErrorCode[FD_CLOSE_BIT]);
					}
					else
					{
						printf("FD_CLOSE success.\n");
					}
				}
				else if (wsaNetworkEvent.lNetworkEvents & FD_READ)
				{
					if (wsaNetworkEvent.iErrorCode[FD_READ_BIT] != 0)
					{
						printf("FD_READ error: %d.\n", wsaNetworkEvent.iErrorCode[FD_READ_BIT]);
					}
					else
					{
						const int bufLen = 1024;
						char buf[bufLen];
						ZeroMemory(buf, sizeof(buf));
						int bytes = recv(svrSocket_, buf, bufLen, 0);
						if (bytes == SOCKET_ERROR)
						{
							printf("recv data error: %d.\n", WSAGetLastError());
						}
						else
						{	
							// decode the received data.

							//
							printf("FD_READ success, recv %d bytes.\n", bytes);
						}

					}
				}
				else if (wsaNetworkEvent.lNetworkEvents & FD_WRITE)
				{
					if (wsaNetworkEvent.iErrorCode[FD_WRITE_BIT] != 0)
					{
						printf("FD_WRITE error: %d.\n", wsaNetworkEvent.iErrorCode[FD_WRITE_BIT]);
					}
					else
					{
						// do write operation.

						//
						printf("FD_WRITE success.\n");
					}
				}
				else
				{
				}
			}
			else if (SOCKET_ERROR == retVal)
			{
				printf("WSAEnumNetworkEvents error: %d.\n", WSAGetLastError());
			}
			else
			{
				assert(0);
			}
		}
		else
		{
			printf("Unexpected return value: %d.\n", retVal);
		}
	}

	//
	return 0;
}