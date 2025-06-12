#include "pch.h"
#include "GameServer.h"
#include "IocpCore.h"
#include "Session.h"


void GameServer::Initialize()
{
    WSADATA wsaData;
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
    }

    _iocpCore = std::make_shared<IocpCore>();
    if (!_iocpCore || _iocpCore->GetHandle() == nullptr)
    {
        std::cerr << "Failed to create IOCP handle." << std::endl;
    }


    // TODO: DB 초기화, NPC 초기화, 맵데이터 초기화


    StartAccept();

    std::cout << "[GameServer] Network and IOCP initialization complete." << std::endl;
}

void GameServer::Run()
{
    _running.store(true);

    unsigned int workerCount = std::thread::hardware_concurrency() - 1;
    _workerThreads.resize(workerCount);
    std::cout << "[GameServer] Running..." << std::endl;

    
    // IOCP worker Thread
    for (unsigned int i = 0; i < workerCount; ++i)
    {
        _workerThreads.emplace_back([this]() {
            while (true == _running.load())
            {
                _iocpCore->Dispatch(10);
            }
            });
    }

    // Timer Thread


    // DB Thread


}

void GameServer::Shutdown()
{
    _running.store(false);
    std::cout << "[GameServer] Shutting down..." << std::endl;

    // 작업 스레드 종료 대기
    for (auto& thread : _workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    _workerThreads.clear();
    std::cout << "[GameServer] Shutdown complete." << std::endl;
}

void GameServer::StartAccept()
{
    _listener = std::make_shared<Session>();
    if (nullptr == _listener) { std::cerr << "Listener Fail\n"; exit(-1); }

    std::shared_ptr<GameServer> server = std::static_pointer_cast<GameServer>(shared_from_this());
    if (nullptr == server) exit(-1);
    _listener->SetService(server);

    _iocpCore->Register(_listener->GetHandle());

    SOCKADDR_IN server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(GAME_PORT);
    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(_listener->GetHandle(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
    listen(_listener->GetHandle(), SOMAXCONN);
    SOCKADDR_IN cl_addr;
    int addr_size = sizeof(cl_addr);
}
