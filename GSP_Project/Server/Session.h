#pragma once

class EX_OVER;

enum S_STATE { ST_FREE, ST_ALLOC, ST_INGAME };
struct ViewEvent {
	EventType type;
	int       id;      // 상대 플레이어 ID
};

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session();
	~Session();

	HANDLE		GetHandle() { return reinterpret_cast<HANDLE>(_socket); }
	void		Dispatch(EX_OVER* ex_over, DWORD numOfBytes);

public:
	void Send(const char* packet);

private:
	void DoSend();
	void DoRecv();

	void OnConnected();
	void OnDisconnected();

public:
	std::atomic_bool	_is_active = false;
	S_STATE				_state = S_STATE::ST_FREE;

	int					_id;
	short				x, y;
	char				_name[MAX_ID_LENGTH];
	int					_prev_remain;


	std::unordered_set <int>					_view_list;
	concurrency::concurrent_queue<ViewEvent>	_view_q;
	std::mutex									_vl;


	long long	last_move_time;

private:
	SOCKET				_socket = INVALID_SOCKET;

	int sectorX{ 0 }, sectorY{ 0 };
	std::unordered_set<std::pair<int, int>>		_sector_list;
	std::mutex									_sl;


	char _db_id[MAX_ID_LENGTH];

};

