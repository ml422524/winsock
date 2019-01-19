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
	//listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	listenSocket_ =  WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket_ == INVALID_SOCKET) {
		WSACleanup();
		return EXE_FAIL;
	}

	// Associate the listening socket with the completion port
	if (NULL == CreateIoCompletionPort((HANDLE)listenSocket_, CompletionPort_, (u_long)0, 0))
	{
		LOG_ERROR("CreateIoCompletionPort error: " << WSAGetLastError());
		return EXE_FAIL;
	}

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
	lpfnAcceptEx_ = NULL; 
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes = 0;
	int iResult = WSAIoctl(listenSocket_, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx, sizeof(GuidAcceptEx),
		&lpfnAcceptEx_, sizeof(lpfnAcceptEx_),
		&dwBytes, NULL, NULL);
	if (iResult == SOCKET_ERROR) {
		LOG_ERROR("WSAIoctl failed with error: " << WSAGetLastError());
		closesocket(listenSocket_);
		WSACleanup();
		return EXE_FAIL;
	}

	//
	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
	iResult = WSAIoctl(listenSocket_, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockAddrs,
		sizeof(GuidGetAcceptExSockAddrs), &lpfnGetAcceptExSockAddrs_, sizeof(lpfnGetAcceptExSockAddrs_),
		&dwBytes, NULL, NULL);
	if (iResult == SOCKET_ERROR)
	{
		LOG_ERROR("WSAIoctl failed with error: " << WSAGetLastError());
		closesocket(listenSocket_);
		WSACleanup();
		return EXE_FAIL;
	}
	
	//
	if (EXE_SUCCESS != PostAccept())
	{
		LOG_ERROR("post an accept operation failed.");
		closesocket(listenSocket_);
		WSACleanup();
		return EXE_FAIL;
	}


	//
	//// Create an accepting socket
	////SOCKET acceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//SOCKET acceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	//if (acceptSocket == INVALID_SOCKET) {
	//	LOG_ERROR("Create accept socket failed with error: " << WSAGetLastError());
	//	closesocket(listenSocket_);
	//	WSACleanup();
	//	return EXE_FAIL;
	//}
	//LOG_DEBUG("create socket: " << acceptSocket);

 //   //
	//LPPER_IO_OPERATE_DATA pPerIoData = (LPPER_IO_OPERATE_DATA)GlobalAlloc(GPTR,
	//	sizeof(PER_IO_OPERATE_DATA));
	//pPerIoData->dataBuf.buf = pPerIoData->buf;
	//pPerIoData->dataBuf.len = DATA_BUFSIZE;
	//pPerIoData->bufLen = DATA_BUFSIZE;
	//pPerIoData->operationType = ACCEPT_POSTED;
	//pPerIoData->socketClient = acceptSocket;

	////
	//BOOL bRetVal = lpfnAcceptEx_(listenSocket_, acceptSocket, pPerIoData->buf,
	//	pPerIoData->bufLen - ((sizeof(sockaddr_in) + 16) * 2), // the actual receive data at the beginning of the buffer
	//	sizeof(sockaddr_in) + 16,
	//	sizeof(sockaddr_in) + 16,
	//	&dwBytes, &pPerIoData->overlapped);
	//if (bRetVal == FALSE)
	//{
	//	if (ERROR_IO_PENDING != WSAGetLastError())
	//	{
	//		LOG_ERROR("AcceptEx failed with error: " << WSAGetLastError());
	//		closesocket(acceptSocket);
	//		closesocket(listenSocket_);
	//		WSACleanup();
	//		GlobalFree(pPerIoData);
	//		return EXE_FAIL;
	//	}
	//}

	// 经试验，以下CreateIoCompletionPort调用可有可无，
	// 完成端口总是可以收到连接。
	// !!! 但是，当有以下调用时，会导致CompletionKey为NULL.!!!
	//if (NULL == CreateIoCompletionPort((HANDLE)acceptSocket, CompletionPort_, (ULONG_PTR)0, 0))
	//{
	//	LOG_ERROR("CreateIoCompletionPort failed.");
	//	return EXE_FAIL;
	//}

	//
	return EXE_SUCCESS;
}

int BaseServer::Deinit()
{
	WSACleanup();
	return EXE_SUCCESS;
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
	BOOL ret;
	while (TRUE)
	{
		ret = GetQueuedCompletionStatus(
			CompletionPort_,
			&bytesTransferred,
			(PULONG_PTR)&pPerHdlData,
			(LPOVERLAPPED*)&pPerIoData,
			INFINITE);

		//
		if (FALSE == ret)
		{
			LOG_ERROR("GetQueuedCompletionStatus return 0, error: " << WSAGetLastError());
			continue;
		}

		//
		if (bytesTransferred == 0)
		{
			//
			if (pPerIoData->operationType == RECV_OPE || pPerIoData->operationType == SEND_OPE)
			{
				LOG_INFO("socket: " << pPerHdlData->socketClient << " closed.");
				closesocket(pPerHdlData->socketClient);
				GlobalFree(pPerHdlData);
				GlobalFree(pPerIoData);
				continue;
			}
		}

		//
		if (pPerIoData->operationType == ACCEPT_OPE)
		{
			//
			SOCKADDR_IN* remote = NULL;
			SOCKADDR_IN* local = NULL;
			int remoteLen = 0;
			int localLen = 0;
			lpfnGetAcceptExSockAddrs_(pPerIoData->buf, 
				0, //pPerIoData->bufLen - (sizeof(SOCKADDR_IN) + 16) * 2,
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
				(LPSOCKADDR*)&local, &localLen, (LPSOCKADDR*)&remote, &remoteLen);
			//
			LOG_DEBUG("client <" << inet_ntoa(remote->sin_addr) << ":" << ntohs(remote->sin_port)
				<< "> come in, data: " << pPerIoData->buf << " len = " << pPerIoData->bytesRecv
				<< " bytesTransferred = " << bytesTransferred
				<< ", socket: " << pPerIoData->socketClient);

			//
			if (EXE_SUCCESS != PostRecvOnAccept(pPerIoData))
			{
				LOG_ERROR("Post recv failed, thread :" << std::this_thread::get_id());
				closesocket(pPerIoData->socketClient);
				GlobalFree(pPerIoData);
			}

			//	
			if (EXE_SUCCESS != PostAccept())
			{
				LOG_ERROR("PostAccept failed, thread:" << std::this_thread::get_id());
			}
		}
		else if (pPerIoData->operationType == RECV_OPE)
		{
			// deal the data received.
			//LOG_DEBUG("recv data: " << pPerIoData->buf << " len = " << pPerIoData->bytesRecv 
				//<< " bytesTransferred = " << bytesTransferred  << " from socket: " << pPerHdlData->socketClient);

			// deal the data received.
			int leftLen = DATA_RECV_BUFSIZE - pPerHdlData->dataLen;
			if (leftLen < bytesTransferred)
			{
				LOG_ERROR("Left space of buffer is not enough.");
				assert(0);
				closesocket(pPerIoData->socketClient);
				GlobalFree(pPerIoData);
				GlobalFree(pPerHdlData);
			}
			else
			{
				//
				memcpy(pPerHdlData->buf + pPerHdlData->dataLen, pPerIoData->buf, bytesTransferred);
				pPerHdlData->dataLen += bytesTransferred;

				//
				if (EXE_SUCCESS != OnReceiveData(pPerHdlData))
				{
					LOG_ERROR("receive data error.");
					closesocket(pPerHdlData->socketClient);
					GlobalFree(pPerHdlData);
					GlobalFree(pPerIoData);
				}
				else
				{
					//
					if (EXE_SUCCESS != PostRecvOnRecv(pPerIoData))
					{
						LOG_ERROR("PostRecvOnRecv failed.");
						closesocket(pPerIoData->socketClient);
						GlobalFree(pPerIoData);
						GlobalFree(pPerHdlData);
					}
				}
			}
		}
		else if (pPerIoData->operationType == SEND_OPE)
		{

		}
		else
		{
			LOG_ERROR("Unknown operation.");
		}
	}
}

int BaseServer::PostAccept()
{
	SOCKET acceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (acceptSocket == INVALID_SOCKET) {
		LOG_ERROR("Create accept socket failed with error: " << WSAGetLastError());
		return EXE_FAIL;
	}
	LOG_DEBUG("create socket: " << acceptSocket);

	//
	LPPER_IO_OPERATE_DATA pPerIoData = (LPPER_IO_OPERATE_DATA)GlobalAlloc(GPTR,
		sizeof(PER_IO_OPERATE_DATA));
	pPerIoData->dataBuf.buf = pPerIoData->buf;
	pPerIoData->dataBuf.len = DATA_BUFSIZE;
	pPerIoData->bufLen = DATA_BUFSIZE;
	pPerIoData->operationType = ACCEPT_OPE;
	pPerIoData->socketClient = acceptSocket;

	//
	DWORD dwBytes = 0;
	BOOL bRetVal = lpfnAcceptEx_(listenSocket_, acceptSocket, pPerIoData->buf,
		0, //pPerIoData->bufLen - ((sizeof(sockaddr_in) + 16) * 2), // the actual receive data at the beginning of the buffer
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		&dwBytes, &pPerIoData->overlapped);
	if (bRetVal == FALSE)
	{
		if (ERROR_IO_PENDING != WSAGetLastError())
		{
			LOG_ERROR("AcceptEx failed with error: " << WSAGetLastError());
			closesocket(acceptSocket);
			GlobalFree(pPerIoData);
			return EXE_FAIL;
		}
	}
	//
	return EXE_SUCCESS;
}

int BaseServer::PostRecvOnAccept(LPPER_IO_OPERATE_DATA pPerIoData)
{
	if (NULL == pPerIoData)
	{
		LOG_ERROR("PER_IO_OPERATE_DATA is empty.");
		assert(0);
		return EXE_FAIL;
	}
	//
	SOCKET socket = pPerIoData->socketClient;

	// 准备单句柄数据
	LPPER_HANDLE_DATA perHdlData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA));
	perHdlData->socketClient = socket;
	perHdlData->dataLen = 0;

	//
	HANDLE hdl = CreateIoCompletionPort((HANDLE)perHdlData->socketClient, CompletionPort_, (ULONG_PTR)perHdlData, 0);
	if (NULL == hdl)
	{
		LOG_ERROR("CreateIoCompletionPort WSAError: " << WSAGetLastError() << ", error: " << GetLastError());
		GlobalFree(perHdlData);
		return EXE_FAIL;
	}
	else
	{
		//
		memset(pPerIoData, 0, sizeof(PER_IO_OPERATE_DATA));
		pPerIoData->socketClient = socket;
		pPerIoData->dataBuf.buf = pPerIoData->buf;
		pPerIoData->dataBuf.len = DATA_BUFSIZE;
		pPerIoData->bufLen = DATA_BUFSIZE;
		pPerIoData->operationType = RECV_OPE;
		int retVal = WSARecv(socket, &pPerIoData->dataBuf, 1, &pPerIoData->bytesRecv, &pPerIoData->flags,
			&pPerIoData->overlapped, NULL);
		if (retVal == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (WSA_IO_PENDING != error)
			{
				LOG_ERROR("WSARecv error: " << error);
				assert(0);
				GlobalFree(perHdlData);
				return EXE_FAIL;
			}
		}
	}
	//
	return EXE_SUCCESS;
}

int BaseServer::PostRecvOnRecv(LPPER_IO_OPERATE_DATA pPerIoData)
{
	//
	if (NULL == pPerIoData)
	{
		LOG_ERROR("LPPER_IO_OPERATE_DATA is empty.");
		assert(0);
		return EXE_FAIL;
	}
	//
	SOCKET socket = pPerIoData->socketClient;

	//
	memset(pPerIoData, 0, sizeof(PER_IO_OPERATE_DATA));
	pPerIoData->socketClient = socket;
	pPerIoData->operationType = RECV_OPE;
	pPerIoData->dataBuf.buf = pPerIoData->buf;
	pPerIoData->dataBuf.len = DATA_BUFSIZE;
	pPerIoData->bufLen = DATA_BUFSIZE;
	int retVal = WSARecv(socket, &pPerIoData->dataBuf, 1, &pPerIoData->bytesRecv, &pPerIoData->flags,
		&pPerIoData->overlapped, NULL);
	if (SOCKET_ERROR == retVal)
	{
		int error = WSAGetLastError();
		if (WSA_IO_PENDING != error)
		{
			LOG_ERROR("WSARecv error: " << error);
			assert(0);
			return EXE_FAIL;
		}
	}
	//
	return EXE_SUCCESS;
}

int BaseServer::OnReceiveData(LPPER_HANDLE_DATA pPerHdlData)
{
	if (NULL == pPerHdlData)
	{
		assert(0);
		return EXE_FAIL;
	}

	//
	int retVal = EXE_SUCCESS;
	DATA_LEN_TYPE msgLen = 0;
	while (pPerHdlData->dataLen >= DATA_HEAD_LEN)
	{
		msgLen = ntohl(*reinterpret_cast<DATA_LEN_TYPE*>(pPerHdlData->buf));
		if (msgLen < 0 || msgLen > DATA_MAX_LEN)
		{
			LOG_ERROR("Invalid msg length: " << msgLen);
			assert(0);
			retVal = EXE_FAIL;
			break;
		}
		else
		{
			int headMsgLen = DATA_HEAD_LEN + msgLen;
			if (pPerHdlData->dataLen >= headMsgLen)
			{
				char headMsgBuffer[DATA_RECV_BUFSIZE];
				memset(headMsgBuffer, 0, DATA_RECV_BUFSIZE);
				char leftBuffer[DATA_RECV_BUFSIZE];
				memset(leftBuffer, 0, DATA_RECV_BUFSIZE);

				//
				int leftLen = DATA_RECV_BUFSIZE - headMsgLen;
				memcpy(headMsgBuffer, pPerHdlData->buf, headMsgLen);
				memcpy(leftBuffer, pPerHdlData->buf + headMsgLen, leftLen);

				//
				memset(pPerHdlData->buf, 0, DATA_RECV_BUFSIZE);
				memcpy(pPerHdlData->buf, leftBuffer, leftLen);

				//
				pPerHdlData->dataLen -= headMsgLen;

				//
				std::string typeName;
				ParseTypeName(headMsgBuffer, typeName);
				LOG_INFO("recv type of " << typeName);
				std::vector<char> binProto;
				ParseBinProto(headMsgBuffer, binProto);

				//
				retVal = EXE_SUCCESS;
			}
			else
			{
				retVal = EXE_SUCCESS;
				break;
			}
		}
	} // end while
	//
	return retVal;
}

//
int BaseServer::ParseCheckSum(const char*buf, int checkSum) const
{
	if (NULL == buf){
		assert(0);
		return EXE_FAIL;
	}
	const MsgHead*msgHead = reinterpret_cast<const MsgHead*>(buf);
	checkSum = ntohl(msgHead->checkSum);
	//
	return EXE_SUCCESS;
}
int BaseServer::ParseTypeName(const char*buf, std::string&typeName) const
{
	if (NULL == buf){
		assert(0);
		return EXE_FAIL;
	}
	const MsgHead*msgHead = reinterpret_cast<const MsgHead*>(buf);
	const int nameLen = ntohl(msgHead->nameLen);
	if (nameLen <= 0 || nameLen >= TYPENAME_LEN){
		assert(0);
		return EXE_FAIL;
	}
	//
	std::string tname(msgHead->name, 0, nameLen);
	typeName = std::move(tname);

	//
	return EXE_SUCCESS;
}
int BaseServer::ParseBinProto(const char*src, std::vector<char>&binProto) const
{
	if (NULL == src){
		assert(0);
		return EXE_FAIL;
	}
	const MsgHead*msgHead = reinterpret_cast<const MsgHead*>(src);
	const int msgLen = ntohl(msgHead->msgLen);
	const int binProtoLen = msgLen - sizeof(MsgHead) + DATA_HEAD_LEN;
	if (binProtoLen <= 0){
		assert(0);
		return EXE_FAIL;
	}	
	std::vector<char> vec(binProtoLen);
	assert(vec.size() == binProtoLen);

	//
	const char *pBinProto = src + sizeof(MsgHead);
	std::copy(pBinProto, pBinProto + binProtoLen, vec.begin());
	//
	binProto = std::move(vec);
	return EXE_SUCCESS;
}