#include "BaseClient.h"
#include <memory>

//
int main(int argc, char*argv[])
{
	//
	std::shared_ptr<BaseClient> clientPtr(new BaseClient());
	clientPtr->Init(NULL); // now use the default ip:port writed in code.
	clientPtr->DoLoop();

	//
	return 0;
}