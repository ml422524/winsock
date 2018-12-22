#include "BaseErrorCode.h"
#include "BaseServer.h"
#include "log.h"

#pragma comment(lib, "ws2_32.lib")

int BaseServer::Init(const char *fileName)
{
	WSADATA wsd;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsd))
	{			
		return EXE_FAIL;
	}
	//
	CompletionPort_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (NULL == CompletionPort_)
	{
		WSACleanup();
		return EXE_FAIL;
	}

	//
	GetSystemInfo(&systemInfo_);
	for (int i = 0; i < 2 * systemInfo_.dwNumberOfProcessors; ++i)
	{ 
		workerThreads_.push_back(std::move(std::thread(&BaseServer::ServerWorkerThread, this)));
	}

	//
	listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket_ == INVALID_SOCKET) {
		WSACleanup();
		return EXE_FAIL;
	}

	// Associate the listening socket with the completion port
	CreateIoCompletionPort((HANDLE)listenSocket_, CompletionPort_, (u_long)0, 0);

	//
	int port = 27015;
	const char *ip = "127.0.0.1";
	//
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(ip);
	service.sin_port = htons(port);
	if (bind(listenSocket_, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR) {
		closesocket(listenSocket_);
		WSACleanup();
		return EXE_FAIL;
	}

	// Start listening on the listening socket
	if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR) {
		closesocket(listenSocket_);
		WSACleanup();
		return EXE_FAIL;
	}

	//
	acceptThreads_.push_back(std::move(std::thread(&BaseServer::AcceptThread, this)));

	//
	return EXE_SUCCESS;
}

int BaseServer::Deinit()
{
	WSACleanup();
	return EXE_SUCCESS;
}

//
void BaseServer::AcceptThread()
{
	//
	while (TRUE)
	{
		PER_HANDLE_DATA *PerHdlData = NULL;
		SOCKADDR_IN saRemote;
		int remoteLen = sizeof(saRemote);
		SOCKET accept = WSAAccept(listenSocket_, (PSOCKADDR)&saRemote, &remoteLen, NULL, NULL);

		//
		PerHdlData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA));
		PerHdlData->socket = accept;
		memcpy(&PerHdlData->clientAddr, &saRemote, remoteLen);

		//
		CreateIoCompletionPort((HANDLE)accept, CompletionPort_, (DWORD)PerHdlData, 0);

		//
		LPPER_IO_OPERATE_DATA pPerIoData = (LPPER_IO_OPERATE_DATA)GlobalAlloc(GPTR,
			sizeof(PER_IO_OPERATE_DATA));
		pPerIoData->dataBuf.buf = pPerIoData->buf;
		pPerIoData->dataBuf.len = DATA_BUFSIZE;
		pPerIoData->operationType = RECV_POSTED;
		WSARecv(accept, &pPerIoData->dataBuf, 1, &pPerIoData->bytesRecv, &pPerIoData->flags,
			&pPerIoData->overlapped, NULL);
	}
}

//
void BaseServer::ServerWorkerThread()
{
	DWORD bytesTransferred;
	LPOVERLAPPED pOverlapped;
	LPPER_HANDLE_DATA pPerHdlData;
	LPPER_IO_OPERATE_DATA pPerIoData;
	DWORD sendBytes, recvBytes;
	DWORD flags;
	int ret;
	while (TRUE)
	{
		ret = GetQueuedCompletionStatus(
			CompletionPort_,
			&bytesTransferred,
			(LPDWORD)&pPerHdlData,
			(LPOVERLAPPED*)&pPerIoData,
			INFINITE);

		//
		if (0 == ret)
		{
			continue;
		}

		//
		if (bytesTransferred == 0)
		{
			//
			if (pPerIoData->operationType == RECV_POSTED)
			{
				LOG_INFO("socket: " << pPerHdlData->socket << " closed.");
				//
				closesocket(pPerHdlData->socket);
				GlobalFree(pPerHdlData);
				GlobalFree(pPerIoData);
				continue;
			}
		}

		//
		if (pPerIoData->operationType == RECV_POSTED)
		{
			// deal the data received.
			
			// re-post a recv operate.
			WSARecv(pPerHdlData->socket, &pPerIoData->dataBuf, 1, &pPerIoData->bytesRecv, &pPerIoData->flags,
				&pPerIoData->overlapped, NULL);

			//
			LOG_INFO("re-post a WSARecv.");

		}
	}
}