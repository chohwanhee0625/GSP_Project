#include "pch.h"
#include "Session.h"
#include "IocpCore.h"
#include "GameServer.h"
#include "Over.h"
#include "PacketHandler.h"
#include "NPC.h"
#include "DBManager.h"

concurrency::concurrent_unordered_map<int, SessionRef>  clients;
concurrency::concurrent_unordered_set<int>              sectors[SECTOR_COUNT_X][SECTOR_COUNT_Y];
std::shared_mutex                                       s_locks[SECTOR_COUNT_X][SECTOR_COUNT_Y];


Session::Session()
	: _id(-1), x(0), y(0)
{
	//_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
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

void Session::dispatchViewEvents()
{
    ViewEvent ev;
    while (_view_q.try_pop(ev)) {
        switch (ev.type) {
        case ViewEventType::Add:
            _view_list.insert(ev.id);
            send_enter_player_packet(ev.id);
            continue;
        case ViewEventType::Move:
            send_move_player_packet(ev.id);
            continue;
        case ViewEventType::Remove:
            if (0 != _view_list.count(ev.id))
                _view_list.erase(ev.id);
            send_leave_player_packet(ev.id);
            continue;
        default:
            continue;
        }
    }
}

void Session::Dispatch(EX_OVER* ex_over, DWORD numOfBytes)
{
    switch (ex_over->eventType)
    {
    case EventType::IO_RECV:
        ProcessRecv(ex_over, numOfBytes);
        break;
    case EventType::IO_SEND:
        delete ex_over;
        //ProcessSend(numOfBytes);
        break;
    }
}

//void Session::StartAccept(std::shared_ptr<GameServer> server)
//{
//    _server = server;
//    if (nullptr == _server.lock()) exit(-1);
//
//    _id = 30000;
//    _server.lock()->GetIocpCore()->Register(_socket, _id);
//
//    SOCKADDR_IN server_addr;
//    memset(&server_addr, 0, sizeof(server_addr));
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_port = htons(GAME_PORT);
//    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
//    bind(_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
//    listen(_socket, SOMAXCONN);
//    SOCKADDR_IN cl_addr;
//    int addr_size = sizeof(cl_addr);
//
//    EX_OVER* ex_over = new EX_OVER();
//    ex_over->eventType = EventType::IO_ACCEPT;
//    ex_over->_id = _id;
//    DoAccept(ex_over);
//}
//
//void Session::DoAccept(EX_OVER* ex_over)
//{
//    SessionRef session = std::make_shared<Session>();
//    session->SetService(_server.lock());
//
//    ZeroMemory(&ex_over->_over, sizeof(WSAOVERLAPPED));
//
//    DWORD bytesReceived = 0;
//    BOOL result = ::AcceptEx(_socket, session->GetSocket(), ex_over->_buf, 0,
//        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytesReceived, &ex_over->_over);
//    if (false == result) {
//        const int32 errorCode = ::WSAGetLastError();
//        if (errorCode != WSA_IO_PENDING) {
//            DoAccept(ex_over);
//        }
//        else {
//            std::cout << "AcceptEx error: " << WSAGetLastError() << std::endl;
//        }
//    }
//}
//
//void Session::ProcessAccept(EX_OVER* ex_over)
//{
//    int32 client_id = ex_over->_id;
//
//    if (SOCKET_ERROR == ::setsockopt(session->GetSocket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
//        reinterpret_cast<char*>(&_socket), sizeof(SOCKET))) {
//        DoAccept(ex_over);
//    }
//
//    SOCKADDR_IN sockAddress;
//    int32 sizeOfSockAddr = sizeof(sockAddress);
//    if (SOCKET_ERROR == ::getpeername(session->GetSocket(),
//        OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr)) {
//        DoAccept(ex_over);
//        return;
//    }
//
//    // OnConnected
//    GetService()->AddSession(session);
//    session->DoRecv();
//
//    //ex_over->session = nullptr;
//    DoAccept(ex_over);
//}

void Session::AllocPlayerData(SOCKET client_sock, uint32 client_id)
{
    {
        _s_lock.lock();
        _state = S_STATE::ST_ALLOC;
        _id = client_id;
        _s_lock.unlock();
    }
    _socket = client_sock;
    x = 0; y = 0;
    _prev_remain = 0;
}

void Session::OnLogin(sc_packet_avatar_info login_info, const char name[MAX_ID_LENGTH])
{
    strcpy_s(_name, name);
    {
        _s_lock.lock();
        this->x = login_info.x; this->y = login_info.y;
        _state = S_STATE::ST_INGAME;
        _s_lock.unlock();
    }
    // Send Login Info
    sc_packet_avatar_info p;
    p = login_info;
    p.size = sizeof(sc_packet_avatar_info);
    p.type = S2C_P_AVATAR_INFO;
    Send(&p);

    updateSector(x, y);
    _sl.lock();
    auto sector_list = _sector_list;
    _sl.unlock();
    for (auto& s_num : sector_list) {
        s_locks[s_num.first][s_num.second].lock_shared();
        auto s_clients = sectors[s_num.first][s_num.second];
        s_locks[s_num.first][s_num.second].unlock_shared();
        for (auto& id : s_clients) {
            auto& pl = clients[id];
            if (nullptr == pl) continue;
            {
                std::lock_guard<std::mutex> ll(pl->_s_lock);
                if (ST_INGAME != pl->_state) continue;
            }
            if (pl->_id == _id) continue;
            if (false == can_see(_id, pl->_id))
                continue;
            if (is_pc(pl->_id)) pl->_view_q.push(ViewEvent{ ViewEventType::Add, _id });
            else pl->wakeup(_id);
            this->send_enter_player_packet(pl->_id);
            _view_list.insert(pl->_id);
        }
    }

}

void Session::Send(void* packet)
{
    EX_OVER* ex_over = new EX_OVER{ reinterpret_cast<char*>(packet) };
    ::WSASend(_socket, &ex_over->_wsaBuf, 1, 0, 0, &ex_over->_over, nullptr);
}

void Session::DoRecv()
{
    DWORD recv_flag = 0;
    memset(&_recv_over._over, 0, sizeof(_recv_over._over));
    _recv_over._wsaBuf.len = BUF_SIZE - _prev_remain;
    _recv_over._wsaBuf.buf = _recv_over._buf + _prev_remain;
    WSARecv(_socket, &_recv_over._wsaBuf, 1, 0, &recv_flag,
        &_recv_over._over, 0);
}

void Session::Disconnect()
{
    //std::cout << "Disconnect" << std::endl;
    std::unordered_set <int> vl = _view_list;
    for (auto& p_id : vl) {
        if (p_id == _id) continue;
        if (true == is_npc(p_id)) {
            auto npc = std::dynamic_pointer_cast<NPC>(clients[p_id]);
            npc->sleepdown();
            continue;
        }
        auto& pl = clients[p_id];
        {
            std::lock_guard<std::mutex> ll(pl->_s_lock);
            if (ST_INGAME != pl->_state) continue;
        }
        pl->_view_q.push(ViewEvent{ ViewEventType::Remove, _id });
    }
    closesocket(_socket);

    DBEvent event;
    event.eventType = DBEventType::DB_EVENT_LOGOUT;
    event.id = _id;
    DBQueue.push(event);

    std::lock_guard<std::mutex> ll(_s_lock);
    _state = ST_FREE;
}

void Session::ProcessRecv(EX_OVER* ex_over, int32 numOfBytes)
{
    SessionRef session = shared_from_this();

    int32 processLen = 0;
    char* p = ex_over->_buf;
    int32 data_size = numOfBytes + _prev_remain;
    while (true) {
        if (p >= ex_over->_buf + data_size) break;  // 모든 데이터를 처리
        char packet_size = p[0];
        if (p + packet_size > ex_over->_buf + data_size) break; // 데이터가 잘린채 남아있다면 스탑
        GPacketHandler[p[1]](session, p, packet_size);  // Process Packet
        p = p + packet_size;
    }
    if (p < ex_over->_buf + data_size) {
        _prev_remain = static_cast<int>(ex_over->_buf + data_size - p);
        ::memcpy(p, ex_over->_buf, _prev_remain);
    }

    DoRecv();
}

void Session::send_enter_player_packet(uint32 client_id)
{
    SessionRef other = clients[client_id];

    sc_packet_enter enter_packet;
    enter_packet.id = client_id;
    strcpy_s(enter_packet.name, other->_name);
    enter_packet.size = sizeof(sc_packet_enter);
    enter_packet.type = S2C_P_ENTER;
    enter_packet.x = other->x;
    enter_packet.y = other->y;
    enter_packet.o_type = other->o_type;
    Send(&enter_packet);
}

void Session::send_move_player_packet(uint32 client_id)
{
    SessionRef other = clients[client_id];

    sc_packet_move move_packet;
    move_packet.id = client_id;
    move_packet.size = sizeof(sc_packet_move);
    move_packet.type = S2C_P_MOVE;
    move_packet.x = other->x;
    move_packet.y = other->y;
    move_packet.move_time = other->last_move_time;
    Send(&move_packet);
}

void Session::send_leave_player_packet(uint32 client_id)
{
    sc_packet_leave leave_packet;
    leave_packet.id = client_id;
    leave_packet.size = sizeof(sc_packet_leave);
    leave_packet.type = S2C_P_LEAVE;
    Send(&leave_packet);
}

void Session::updateSector(int32 newX, int32 newY)
{
    // 자신의 섹터 좌표 계산
    int sx = newX / SECTOR_SIZE;
    int sy = newY / SECTOR_SIZE;

    _sl.lock();
    _sector_list.clear();
    _sector_list.insert({ sx, sy });

    // 시야 범위 만큼 주변 섹터를 순회
    for (int dy = -VIEW_RANGE; dy <= VIEW_RANGE; ++dy) {
        int yy = sy + dy;
        if (yy < 0 || yy >= SECTOR_COUNT_Y) continue;  // 경계 검사

        for (int dx = -VIEW_RANGE; dx <= VIEW_RANGE; ++dx) {
            int xx = sx + dx;
            if (xx < 0 || xx >= SECTOR_COUNT_X) continue;  // 경계 검사

            _sector_list.insert({ xx, yy });
        }
    }
    _sl.unlock();

    // 자신의 섹터가 변경되었다면 sectors 업데이트
    if (sx != sectorX || sy != sectorY)
    {
        s_locks[sectorX][sectorY].lock();
        if (sectorX >= 0 && sectorY >= 0) {
            if (sectors[sectorX][sectorY].count(_id) != 0) {
                sectors[sectorX][sectorY].unsafe_erase(_id);
            }
        }
        sectors[sx][sy].insert(_id);

        s_locks[sectorX][sectorY].unlock();
        sectorX = sx;
        sectorY = sy;
    }
}

bool is_pc(int object_id)
{
    return object_id < MAX_USER;
}

bool is_npc(int object_id)
{
    return !is_pc(object_id);
}

bool can_see(int from, int to)
{
    if (abs(clients[from]->x - clients[to]->x) > VIEW_RANGE) return false;
    return abs(clients[from]->y - clients[to]->y) <= VIEW_RANGE;
}
