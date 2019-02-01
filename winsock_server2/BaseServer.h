#pragma once

#include <vector>
#include <thread>
#include "BaseServerDef.h"
#include "proto/msg.pb.h"
#include "BaseErrorCode.h"
#include "Connection.h"

class BaseServer{
public:
	//
	typedef ::google::protobuf::Message Message;
	typedef ::google::protobuf::Descriptor Descriptor;
	typedef std::shared_ptr<Message> MessagePtr;
	typedef std::function<void(const ConnectionPtr& conPtr, std::shared_ptr<Message>)> CallBack;

	//
	BaseServer() {}
	~BaseServer();

	//
	int Init(const char *fileName);

	//
	void OnHello(const ConnectionPtr& conPtr, MessagePtr ptr);
	void OnGetName(const ConnectionPtr& conPtr, MessagePtr ptr);
	void OnDefault(const ConnectionPtr& conPtr, MessagePtr ptr);

	//
	int Notify(const ConnectionPtr& conPtr, const std::string&protoName, const void*data, const int dataLen);
private:
	//
	void ServerWorkerThread();

	//
	int OnReceiveData(LPPER_HANDLE_DATA);
	int OnMessage(const ConnectionPtr& conPtr, const std::string&typeName, const std::string&binProto);

	//
	int PostAccept();
	int PostRecvOnAccept(LPPER_IO_OPERATE_DATA);
	int PostRecvOnRecv(LPPER_IO_OPERATE_DATA);
	int PostSend(LPPER_IO_OPERATE_DATA);

	//
	int ParseMsgLen(const char * buf, int&msgLen) const;
	int ParseCheckSum(const char*buf, int& checkSum) const;
	int ParseTypeName(const char*buf, std::string&typeName) const;
	int ParseBinProto(const char*src, std::string&binProto) const;

	//
	BOOL IsSocketAlive(SOCKET sock);

	//
	template<typename _MessageType>
	int RegisterMessageCallBack(CallBack cb);
	int RegisterDefaultMessageCallBack(CallBack cb);

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

	//
	std::map<const Descriptor*, CallBack> dispatcher_;
	CallBack defaultCallBack_;
};

//
template<typename _MessageType>
int BaseServer::RegisterMessageCallBack(CallBack cb)
{
	const Descriptor*descriptor = _MessageType::descriptor();
	auto it = dispatcher_.find(descriptor);
	if (it != dispatcher_.end())
	{
		assert(0);
		return CALL_BACK_EXIST;
	}
	dispatcher_[descriptor] = cb;
	//
	return CALL_BACK_REG_SUCCESS;
}

inline int BaseServer::RegisterDefaultMessageCallBack(CallBack cb)
{
	defaultCallBack_ = cb;
	return EXE_SUCCESS;
}