#pragma once
#include "Timer.h"

extern thread_local std::unordered_set<int/*client id*/> view_event_list;

class GameServer : public std::enable_shared_from_this<GameServer>
{
public:
    GameServer() : _running(true) {};
    ~GameServer() { Shutdown(); }

    IocpCoreRef& GetIocpCore() { return _iocpCore; }


    void        Initialize();  // 네트워크 및 게임 관련 초기화
    void        Run();         // 서버 메인 루프
    void        Shutdown();    // 서버 종료 및 자원 해제

    //void        AddSession(SessionRef session);
    //void        ReleaseSession(SessionRef session);

private:
    std::atomic_bool            _running = false;
    std::vector<std::thread>    _workerThreads;    // IOCP 작업 스레드 풀
    std::shared_ptr<IocpCore>   _iocpCore;

    std::unique_ptr<Timer>      _timer;
public:

};

