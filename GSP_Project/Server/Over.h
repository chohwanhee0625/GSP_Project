#pragma once

class Session;


enum class EventType : uint8
{
    IO_ACCEPT,
    IO_RECV,
    IO_SEND,
	NPC_MOVE,
};

class EX_OVER
{
public:
	EX_OVER();
	EX_OVER(const char* packet);

public:
	WSAOVERLAPPED _over;
	WSABUF _wsaBuf;
	char _buf[MAX_CHAT_LENGTH] = {};

	EventType	eventType;
	SessionRef	owner;
};

