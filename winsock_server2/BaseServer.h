#pragma once

#include <vector>
#include <thread>
#include "BaseServerDef.h"
#include "proto/msg.pb.h"
#include "BaseErrorCode.h"

class BaseServer{
public:
	//
	typedef ::google::protobuf::Message Message;
	typedef ::google::protobuf::Descriptor Descriptor;
	typedef std::shared_ptr<Message> MessagePtr;

	//
	int Init(const char *fileName);
	int Deinit();

	//
	void OnHello(MessagePtr ptr);
	void OnGetName(MessagePtr ptr);
	void OnDefault(MessagePtr ptr);
private:
	//
	void ServerWorkerThread();

	//
	int OnReceiveData(LPPER_HANDLE_DATA);
	int OnMessage(const std::string&typeName, const std::string&binProto);

	//
	int PostAccept();
	int PostRecvOnAccept(LPPER_IO_OPERATE_DATA);
	int PostRecvOnRecv(LPPER_IO_OPERATE_DATA);

	//
	int ParseMsgLen(const char * buf, int&msgLen) const;
	int ParseCheckSum(const char*buf, int& checkSum) const;
	int ParseTypeName(const char*buf, std::string&typeName) const;
	int ParseBinProto(const char*src, std::string&binProto) const;

	//
	template<typename _MessageType>
	int RegisterMessageCallBack(std::function<void(std::shared_ptr<Message>)> cb);
	int RegisterDefaultMessageCallBack(std::function<void(MessagePtr)> cb);

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
	std::map<const Descriptor*, std::function<void(std::shared_ptr<Message>)>> dispatcher_;
	std::function<void(std::shared_ptr<Message>)> defaultCallBack_;
};

//
template<typename _MessageType>
int BaseServer::RegisterMessageCallBack(std::function<void(std::shared_ptr<Message>)> cb)
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

inline int BaseServer::RegisterDefaultMessageCallBack(std::function<void(MessagePtr)> cb)
{
	defaultCallBack_ = cb;
	return EXE_SUCCESS;
}