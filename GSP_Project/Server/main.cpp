#include "pch.h"
#include "GameServer.h"


int main()
{
	GameServer server;

	server.Initialize();
	server.Run();

	// TODO: ���� ������� DB �۾� ����

	server.Shutdown();
	return 0;
}

