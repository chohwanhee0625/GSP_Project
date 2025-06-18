#pragma once


using PacketHandlerFunc = std::function<bool(SessionRef&, char*, int32)>;
extern PacketHandlerFunc GPacketHandler[100];

bool Handle_INVALID(SessionRef& session, char* packet, int32 len);
bool Handle_LOGIN(SessionRef& session, char* packet, int32 len);
bool Handle_PLAYER_MOVE(SessionRef& session, char* packet, int32 len);
bool Handle_ATTACK(SessionRef& session, char* packet, int32 len);
bool Handle_CHAT(SessionRef& session, char* packet, int32 len);
bool Handle_SIGN(SessionRef& session, char* packet, int32 len);

class PacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < 100; ++i) GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[C2S_P_LOGIN] = Handle_LOGIN;
		GPacketHandler[C2S_P_MOVE] = Handle_PLAYER_MOVE;
		GPacketHandler[C2S_P_ATTACK] = Handle_ATTACK;
		GPacketHandler[C2S_P_CHAT] = Handle_CHAT;
		GPacketHandler[C2S_P_SIGN] = Handle_SIGN;

	}
};

