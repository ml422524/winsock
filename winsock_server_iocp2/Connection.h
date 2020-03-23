#pragma once

#include <WinSock2.h>
#include <memory>

class Connection{
public:
	explicit Connection(const SOCKET sock) : sock_(sock){}
	SOCKET GetSocket() const{
		return sock_;
	}
private:
	SOCKET sock_;
};

typedef std::shared_ptr<Connection> ConnectionPtr;
