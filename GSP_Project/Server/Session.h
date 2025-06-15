#pragma once
#include "Over.h"
#include "GameServer.h"

class EX_OVER; class GameServer;

enum S_STATE { ST_FREE, ST_ALLOC, ST_INGAME };
enum class ViewEventType { Add, Move, Remove };
struct ViewEvent {
	ViewEventType type;
	int       id;      // 상대 플레이어 ID
};

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session();
	~Session();

	SOCKET		GetSocket() { return _socket; }
	virtual void Dispatch(EX_OVER* ex_over, DWORD numOfBytes);
	//void		StartAccept(std::shared_ptr<GameServer> server);
	void		dispatchViewEvents();

private:
	//void		DoAccept(EX_OVER* ex_over);
	//void        ProcessAccept(EX_OVER* ex_over);

public:
	void		AllocPlayerData(SOCKET client_sock, uint32 client_id);
	void		OnLogin(sc_packet_avatar_info login_info, const char name[MAX_ID_LENGTH]);
	void		Send(void* packet);
	void		DoRecv();
	void		Disconnect();

	void SetService(std::shared_ptr<GameServer> server) { _server = server; }
	std::shared_ptr<GameServer> GetService() { return _server.lock(); }

private:
	void		ProcessRecv(EX_OVER* ex_over, int32 numOfBytes);
	//void		ProcessSend(int32 numOfBytes);

public:
	void send_enter_player_packet(uint32 client_id);
	void send_move_player_packet(uint32 client_id);
	void send_leave_player_packet(uint32 client_id);
	virtual void wakeup(int waker) {};
	virtual void sleepdown() {};
	void updateSector(int32 newX, int32 newY);

public:
	std::atomic_bool	_is_active = false;
	S_STATE				_state = S_STATE::ST_FREE;
	std::mutex			_s_lock;

	int					_id;
	short				x, y;
	char				o_type = 0;
	char				_name[MAX_ID_LENGTH] = {};
	char				_db_id[MAX_ID_LENGTH] = {};

	int					_prev_remain = 0;
	long long			last_move_time = 0;

	std::unordered_set <int>					_view_list;
	concurrency::concurrent_queue<ViewEvent>	_view_q;
	std::mutex									_vl;

	int sectorX{ 0 }, sectorY{ 0 };
	std::unordered_set<std::pair<int, int>>		_sector_list;
	std::mutex									_sl;

protected:
	SOCKET										_socket = INVALID_SOCKET;
	EX_OVER										_recv_over;

	std::weak_ptr<GameServer>					_server;
};

extern concurrency::concurrent_unordered_map<int, SessionRef>  clients;
extern concurrency::concurrent_unordered_set<int>              sectors[SECTOR_COUNT_X][SECTOR_COUNT_Y];
extern std::shared_mutex                                       s_locks[SECTOR_COUNT_X][SECTOR_COUNT_Y];

bool is_pc(int object_id);
bool is_npc(int object_id);
bool can_see(int from, int to);
