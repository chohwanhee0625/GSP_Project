#include "pch.h"
#include "PacketHandler.h"
#include "Session.h"
#include "DBManager.h"
#include "NPC.h"
#include "Map.h"


PacketHandlerFunc GPacketHandler[100];

bool Handle_INVALID(SessionRef& session, char* packet, int32 len)
{
	std::cerr << "Handle Invalid!!" << std::endl;
	exit(-1);
	return false;
}

bool Handle_LOGIN(SessionRef& session, char* packet, int32 len)
{
	cs_packet_login* login_packet = reinterpret_cast<cs_packet_login*>(packet);

	if (false == login_packet->isdummy) {
		DBEvent dbEvent;
		dbEvent.eventType = DBEventType::DB_EVENT_LOGIN;
		dbEvent.id = session->_id;
		strcpy_s(session->_db_id, login_packet->id);
		DBQueue.push(dbEvent);

		return true;
	}

	// STRESS_TEST client
	sc_packet_avatar_info dummy_info;
	int32 x = distX(gen); int32 y = distY(gen);
	//int32 x = rand() % MAP_WIDTH; int32 y = rand() % MAP_HEIGHT;
	dummy_info.x = x; dummy_info.y = y;
	dummy_info.id = session->_id;
	dummy_info.max_hp = 100; dummy_info.hp = 100;
	dummy_info.level = 1; dummy_info.exp = 0;
	session->OnLogin(dummy_info, login_packet->name);

	return true;
}

bool Handle_PLAYER_MOVE(SessionRef& session, char* packet, int32 len)
{
	cs_packet_move* move_packet = reinterpret_cast<cs_packet_move*>(packet);
	session->last_move_time = move_packet->move_time;
	int16 x = session->x; int16 y = session->y;
	switch (move_packet->direction)	{
	case MOVE_UP:		if (y > 0) y--; break;
	case MOVE_DOWN:		if (y < MAP_HEIGHT - 1) y++; break;
	case MOVE_LEFT:		if (x > 0) x--; break;
	case MOVE_RIGHT:	if (x < MAP_WIDTH - 1) x++; break;
	}

	auto& instance = Map::GetInstance();
	if (false == instance.IsObstacle(x, y)) session->x = x; session->y = y;
	session->updateSector(session->x, session->y);
	std::unordered_set<int> near_list;
	std::unordered_set<int> old_vlist = session->_view_list;
	session->_sl.lock();
	auto sector_list = session->_sector_list;
	session->_sl.unlock();
	for (auto& s_num : sector_list)
	{
		s_locks[s_num.first][s_num.second].lock_shared();
		auto s_clients = sectors[s_num.first][s_num.second];
		s_locks[s_num.first][s_num.second].unlock_shared();
		for (auto& id : s_clients) {
			auto& cl = clients[id];
			if (nullptr == cl) continue;
			if (cl->_state != ST_INGAME) continue;
			if (cl->_id == session->_id) continue;
			if (can_see(session->_id, cl->_id)) {
				near_list.insert(cl->_id);
			}
		}
	}

	session->send_move_player_packet(session->_id);

	for (auto& pl : near_list) {
		auto& cpl = clients[pl];
		if (nullptr == cpl) continue;
		if (true == is_pc(pl)) {
			if (0 != old_vlist.count(pl)) {
				clients[pl]->_view_q.push(ViewEvent{ ViewEventType::Move, session->_id });
				view_event_list.insert(pl);
			}
			else {
				clients[pl]->_view_q.push(ViewEvent{ ViewEventType::Add, session->_id });
				view_event_list.insert(pl);
			}
		}
		else {	// NPC
			NPCRef npc = std::dynamic_pointer_cast<NPC>(clients[pl]);
			npc->wakeup(session->_id);
		}

		if (old_vlist.count(pl) == 0)
			session->send_enter_player_packet(pl);
	}

	for (auto& pl : old_vlist) {
		if (0 == near_list.count(pl)) {
			//std::cout << "Leave " << pl << std::endl;
			session->send_leave_player_packet(pl);
			if (is_pc(pl)) {
				clients[pl]->_view_q.push(ViewEvent{ ViewEventType::Remove, session->_id });
				view_event_list.insert(pl);
			}
			else clients[pl]->sleepdown();
		}
	}

	return true;
}

bool Handle_ATTACK(SessionRef& session, char* packet, int32 len)
{
	cs_packet_attack* move_packet = reinterpret_cast<cs_packet_attack*>(packet);


	return false;
}

bool Handle_CHAT(SessionRef& session, char* packet, int32 len)
{
	cs_packet_chat* chat_packet = reinterpret_cast<cs_packet_chat*>(packet);
	if (chat_packet->message[0] == '\0') return true;

	sc_packet_chat chat_msg;

	return true;
}

bool Handle_SIGN(SessionRef& session, char* packet, int32 len)
{
	cs_packet_sign* sign_packet = reinterpret_cast<cs_packet_sign*>(packet);
	strcpy_s(session->_db_id, sign_packet->id);
	strcpy_s(session->_name, sign_packet->name);

	DBEvent dbEvent;
	dbEvent.eventType = DBEventType::DB_EVENT_SIGN;
	dbEvent.id = session->_id;
	DBQueue.push(dbEvent);

	return true;
}
