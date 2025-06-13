#include "pch.h"
#include "GameServer.h"


int main()
{
	setlocale(LC_ALL, "korean");
	std::wcout.imbue(std::locale("korean"));

	std::shared_ptr<GameServer> server = std::make_shared<GameServer>();

	server->Initialize();
	server->Run();

	server->Shutdown();
	return 0;
}

