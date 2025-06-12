#include "pch.h"
#include "GameServer.h"


int main()
{
	setlocale(LC_ALL, "korean");
	std::wcout.imbue(std::locale("korean"));

	GameServer server;

	server.Initialize();
	server.Run();

	server.Shutdown();
	return 0;
}

