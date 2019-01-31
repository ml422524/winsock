#include "BaseServer.h"
#include "log.h"

int main(int argc, char*argv[])
{
	//
	assert(sizeof(u_long) == sizeof(int));
	assert(sizeof(u_long) == 4);

	//
	Log::InitLog();

	//
	BaseServer svr;
	svr.Init(NULL);

	//
	while (TRUE)
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	//
	svr.Deinit();

	//
	return 0;
}