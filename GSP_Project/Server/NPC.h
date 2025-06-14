#pragma once
#include "Session.h"

class NPC : public Session
{
public:
	NPC() = default;
	~NPC() = default;

	void Dispatch(EX_OVER* ex_over, DWORD numOfBytes) override;

	void Initalize(int32 npc_id);
	void Chase();
	void MoveTo(short t_x, short t_y);
	void ReturnToInitPos();
	void wakeup(int waker) override;


public:
	std::atomic_int	_target_id = -1;
	short			_init_x, _init_y;	// ���� ���� ���� const
	short			_max_range = 20;	// ��׷� �ִ� ����, Temp(LUA ���� ����)

	lua_State*		_L;
	std::mutex		_ll;
};

