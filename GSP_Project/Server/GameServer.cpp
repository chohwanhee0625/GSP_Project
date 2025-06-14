#include "pch.h"
#include "GameServer.h"
#include "IocpCore.h"
#include "Session.h"
#include "PacketHandler.h"
#include "DBManager.h"
#include "Timer.h"
#include "NPC.h"


thread_local std::unordered_set<int/*client id*/> view_event_list;

void GameServer::Initialize()
{
    WSADATA wsaData;
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
    }

    _iocpCore = std::make_shared<IocpCore>(shared_from_this());
    if (!_iocpCore || _iocpCore->GetHandle() == nullptr)
    {
        std::cerr << "Failed to create IOCP handle." << std::endl;
    }

    PacketHandler::Init();

	auto& dbManager = DBManager::GetInstance();
    if (false == dbManager.DBConnection()) {
        std::cerr << "[DB] Faile to connect to DB" << std::endl;
    }

    // TODO: NPC 초기화, 맵데이터 초기화
    _timer = std::make_unique<Timer>(_iocpCore);


    std::cout << "[NPC] Initialize begin" << std::endl;
    for (int i = MAX_USER; i < MAX_USER + NUM_MONSTER; ++i) {
		NPCRef npc = std::make_shared<NPC>();
        npc->SetService(shared_from_this());
		npc->Initalize(i);
		clients[i] = npc;
    }
    std::cout << "[NPC] Initialize end" << std::endl;

    _iocpCore->StartAccept();

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
                _iocpCore->Dispatch();

                // Process ViewEvents
                for (auto& client_id : view_event_list) {
                    SessionRef session = clients[client_id];
                    if (nullptr == session) continue;
                    {
                        std::lock_guard<std::mutex> sl(session->_s_lock);
                        if (ST_INGAME != session->_state) continue;
                    }
                    session->dispatchViewEvents();
                }
            }
            });
    }

    // Timer Thread
    _timer->Start();

    // DB Thread
    auto& dbManager = DBManager::GetInstance();
    while (true == _running.load()) {

    }

}

void GameServer::Shutdown()
{
    _running.store(false);
    std::cout << "[GameServer] Shutting down..." << std::endl;

    // 작업 스레드 종료 대기
    for (auto& thread : _workerThreads) {
            thread.join();
    }

    _workerThreads.clear();
    std::cout << "[GameServer] Shutdown complete." << std::endl;
}

//void GameServer::AddSession(SessionRef session)
//{
//    uint32 client_id = GClientIDGen++;
//    clients[client_id] = session;
//    session->AllocPlayerData(client_id);
//}

//void GameServer::ReleaseSession(SessionRef session)
//{
//
//}
//
