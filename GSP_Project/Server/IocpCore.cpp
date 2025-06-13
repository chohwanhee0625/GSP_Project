#include "pch.h"
#include "Over.h"
#include "Session.h"
#include "IocpCore.h"


IocpCore::IocpCore(std::shared_ptr<GameServer> server)
{
    _server = server;
    _iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    if (_iocpHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "IOCP Handle Failed" << std::endl;
        exit(-1);
    }
}

IocpCore::~IocpCore()
{
    ::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(SOCKET socket, int32 key)
{
    return ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), _iocpHandle, /*key*/key, 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
    DWORD numOfBytes = 0;
    ULONG_PTR key = 0;
    EX_OVER* ex_over = nullptr;

    BOOL ret = ::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT & key,
        OUT reinterpret_cast<LPOVERLAPPED*>(&ex_over), timeoutMs);
    if (FALSE == ret)
    {
        if (ex_over->eventType == EventType::IO_ACCEPT) std::cout << "Accept Error";
        else {
            std::cout << "GQCS Error on client[" << key << "]\n";
            //Disconnect(static_cast<int>(key));
            if (ex_over->eventType == EventType::IO_SEND) delete ex_over;
            return false;
        }
    }

    if ((0 == numOfBytes) && ((ex_over->eventType == EventType::IO_RECV) || (ex_over->eventType == EventType::IO_SEND))) {
        //Disconnect(static_cast<int>(key));
        if (ex_over->eventType == EventType::IO_SEND) delete ex_over;
        return true;
    }

    switch (ex_over->eventType)
    {
    case EventType::IO_ACCEPT:
    {
        uint32 client_id = GClientIDGen++;
        if (client_id > MAX_USER) { std::cout << "Max user exceeded.\n"; break; }
        SessionRef session = std::make_shared<Session>();
        clients[client_id] = session;
        session->AllocPlayerData(_client_sock, client_id);
        Register(_client_sock, client_id);
        session->DoRecv();

        _client_sock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
        ZeroMemory(&_over._over, sizeof(_over._over));
        AcceptEx(_listen_sock, _client_sock, _over._buf, 0,
            sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, &_over._over);
    }
        break;
    case EventType::IO_RECV:
    case EventType::IO_SEND:
    {
        int32 client_id = static_cast<int>(key);
        clients[client_id]->Dispatch(ex_over, numOfBytes);
    }
        break;
        //case EventType::NPC_MOVE:
        //    break;
    default:
        break;
    }


    return true;
}

void IocpCore::StartAccept()
{
    _listen_sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    Register(_listen_sock, 30000);

    SOCKADDR_IN server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(GAME_PORT);
    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(_listen_sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
    listen(_listen_sock, SOMAXCONN);
    SOCKADDR_IN cl_addr;
    int addr_size = sizeof(cl_addr);

    ZeroMemory(&_over._over, sizeof(_over._over));
    _over.eventType = EventType::IO_ACCEPT;
    _client_sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    AcceptEx(_listen_sock, _client_sock, _over._buf, 0,
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, &_over._over);
}
