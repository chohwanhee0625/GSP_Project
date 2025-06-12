#include "pch.h"
#include "Session.h"
#include "IocpCore.h"
#include "Over.h"



Session::Session()
	: _id(-1), x(0), y(0)
{
	_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

}

Session::~Session()
{
	if (INVALID_SOCKET != _socket)
	{
		::closesocket(_socket);
		_socket = INVALID_SOCKET;
	}
	_state = S_STATE::ST_FREE;
	_is_active = false;
	_server.reset();
}

void Session::Dispatch(EX_OVER* ex_over, DWORD numOfBytes)
{
	
}


