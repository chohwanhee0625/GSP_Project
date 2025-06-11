#include "pch.h"
#include "GameServer.h"
#include "IocpCore.h"


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

    // TODO: 

    std::cout << "[GameServer] Network and IOCP initialization complete." << std::endl;
}

void GameServer::Run()
{
    _running.store(true);

    unsigned int workerCount = std::thread::hardware_concurrency() - 1;
    _workerThreads.resize(workerCount);
    std::cout << "[GameServer] Running..." << std::endl;

    for (unsigned int i = 0; i < workerCount; ++i)
    {
        _workerThreads.emplace_back([this]() {
            while (true == _running.load())
            {
                _iocpCore->Dispatch(10);
            }
            });
    }


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
