#include "pch.h"
#include "NPC.h"
#include "Session.h"
#include "LUA.h"
#include "GameServer.h"
#include "IocpCore.h"
#include "Over.h"


void NPC::Dispatch(EX_OVER* ex_over, DWORD numOfBytes)
{
	using namespace std::chrono;
	switch (ex_over->eventType)
	{
	case EventType::NPC_MOVE:
		if (_target_id < 0)	MoveTo(_init_x, _init_y);
		else MoveTo(clients[_target_id]->x, clients[_target_id]->y);
		break;
	case EventType::NPC_CHASE:
		Chase();
		break;
	case EventType::NPC_RETURN:
		break;
	}
	delete ex_over;
}

void NPC::Initalize(int32 npc_id)
{
	x = distX(gen); y = distY(gen);
	_init_x = x; _init_y = y;
	_target_id = -1;
	_state = S_STATE::ST_INGAME;
	_id = npc_id;
	updateSector(x, y);
	_is_active = false;
	sprintf_s(_name, "NPC%d", npc_id);

	// TODO: LUA ��ũ��Ʈ �ε� �� �ʱ�ȭ
}

void NPC::wakeup(int waker)
{
	short dx = abs(clients[waker]->x - x);
	short dy = abs(clients[waker]->y - y);
	if (dx + dy > _max_range) return;

	bool temp = false;
	if (true == std::atomic_compare_exchange_strong(&_is_active, &temp, true)) {
		// ù ��׷� �÷��̾ �Ѿư�����
		_target_id = waker;
		EX_OVER* ex_over = new EX_OVER();
		ex_over->eventType = EventType::NPC_CHASE;
		ex_over->_ai_target_id = waker;
		_server.lock()->GetIocpCore()->PQCS(ex_over, _id);
	}
}

void NPC::sleepdown()
{
	ReturnToInitPos();
}

void NPC::Chase()
{
	if (true == _is_active) {
		//std::cout << "Find Agro - " << _id << std::endl;
		RegistNPCMove();
	}
}

void NPC::MoveTo(short t_x, short t_y)
{
	if (false == _is_active) return;
	short in_dx = abs(_init_x - x);
	short in_dy = abs(_init_y - y);
	if (false == _returning && in_dx + in_dy > _max_range) {
		sleepdown();
		return;
	}
	
	std::unordered_set<int> old_vl;
	_sl.lock();
	auto sector_list = _sector_list;
	_sl.unlock();

	for (auto& s_num : sector_list)
	{
		s_locks[s_num.first][s_num.second].lock_shared();
		auto s_clients = sectors[s_num.first][s_num.second];
		s_locks[s_num.first][s_num.second].unlock_shared();
		for (auto& id : s_clients) {
			auto& obj = clients[id];
			if (nullptr == obj) continue;
			if (ST_INGAME != obj->_state) continue;
			if (true == is_npc(obj->_id)) continue;
			if (true == can_see(_id, obj->_id))
				old_vl.insert(obj->_id);
		}
	}

	// TEMP: LUA ���� ����
	int dx = abs(t_x - x);
	int dy = abs(t_y - y);

	if (dx >= dy) {
		if (t_x > x) x++;
		else if (t_x < x) x--;
	}
	else {
		if (t_y > y) y++;
		else if (t_y < y) y--;
	}

	updateSector(x, y);
	sector_list.clear();
	_sl.lock();
	sector_list = _sector_list; 
	_sl.unlock();

	std::unordered_set<int> new_vl;
	for (auto& s_num : sector_list)
	{
		s_locks[s_num.first][s_num.second].lock_shared();
		auto s_clients = sectors[s_num.first][s_num.second];
		s_locks[s_num.first][s_num.second].unlock_shared();
		for (auto& id : s_clients) {
			auto& obj = clients[id];
			if (nullptr == obj) continue;
			if (ST_INGAME != obj->_state) continue;
			if (true == is_npc(obj->_id)) continue;
			if (true == can_see(_id, obj->_id))
				new_vl.insert(obj->_id);
			//else npc->sleepdown();
		}
	}

	for (auto pl : new_vl) {
		if (0 == old_vl.count(pl)) {
			// �÷��̾��� �þ߿� ����
			clients[pl]->_view_q.push(ViewEvent{ ViewEventType::Add, _id });
			view_event_list.insert(pl);
		}
		else {
			// �÷��̾ ��� ���� ����.
			clients[pl]->_view_q.push(ViewEvent{ ViewEventType::Move, _id });
			view_event_list.insert(pl);
		}
	}

	for (auto pl : old_vl) {
		if (0 == new_vl.count(pl)) {
			clients[pl]->_view_q.push(ViewEvent{ ViewEventType::Remove, _id });
			view_event_list.insert(pl);
		}
	}

	// ��׷� Ǯ���� �ʱ� ��ġ�� ���� Ȯ��
	if (_target_id < 0 && x == _init_x && y == _init_y) {
		//std::cout << "�����ڸ� ����" << std::endl;
		bool temp = true;
		if (true == std::atomic_compare_exchange_strong(&_is_active, &temp, false)) {
			_returning = false;
			// �� �ƹ��͵� ���ص� �ɵ�?
			//EX_OVER* ex_over = new EX_OVER();
			//ex_over->eventType = EventType::NPC_RETURN;
			//_server.lock()->GetIocpCore()->PQCS(ex_over, _id);
		}
	}
	else {
		RegistNPCMove();
	}
}

void NPC::ReturnToInitPos()
{
	if (true == _is_active) {
		_target_id = -1;
		_returning = true;
		RegistNPCMove();
		//std::cout << "ReturnToInitPos - " << _id << std::endl;
	}
}

void NPC::RegistNPCMove()
{
	using namespace std::chrono;

	timer_lock.lock();
	TimerQueue.emplace(TimerEvent{ _id, high_resolution_clock::now() + seconds(1),
		T_EVENT_TYPE::EV_MOVE, _target_id });
	timer_lock.unlock();
}
