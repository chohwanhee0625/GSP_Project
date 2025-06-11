#include "pch.h"
#include "Over.h"
#include "Session.h"


EX_OVER::EX_OVER()
{
	_wsaBuf.buf = _buf;
	_wsaBuf.len = MAX_CHAT_LENGTH;
	eventType = EventType::IO_RECV;
	ZeroMemory(&_over, sizeof(_over));
}

EX_OVER::EX_OVER(const char* packet)
{
	_wsaBuf.buf = _buf;
	_wsaBuf.len = packet[0];
	ZeroMemory(&_over, sizeof(_over));
	eventType = EventType::IO_SEND;
	memcpy(_buf, packet, _wsaBuf.len);
}
