#pragma once
#include "Over.h"


class IocpCore : public std::enable_shared_from_this<IocpCore>
{
public:
	IocpCore(std::shared_ptr<GameServer> server);
	~IocpCore();
	HANDLE		GetHandle() { return _iocpHandle; }

	bool		Register(SOCKET socket, int32 key);
	bool		Dispatch(uint32 timeoutMs = INFINITE);	// CP를 감시하면서 대기하는 함수, 작업스레드 원청느낌

	void		StartAccept();

private:
	HANDLE						_iocpHandle;
	SOCKET						_listen_sock;
	SOCKET						_client_sock;
	EX_OVER						_over;

	std::weak_ptr<GameServer>	_server;
};

