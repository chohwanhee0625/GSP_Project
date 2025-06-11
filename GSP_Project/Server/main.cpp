#include "pch.h"
#include "GameServer.h"


int main()
{
	GameServer server;

	server.Initialize();
	server.Run();

	// TODO: 메인 쓰레드는 DB 작업 실행

	server.Shutdown();
	return 0;
}

