#pragma once
#include "Timer.h"

extern thread_local std::unordered_set<int/*client id*/> view_event_list;

class GameServer : public std::enable_shared_from_this<GameServer>
{
public:
    GameServer() : _running(true) {};
    ~GameServer() { Shutdown(); }

    IocpCoreRef& GetIocpCore() { return _iocpCore; }


    void        Initialize();  // ��Ʈ��ũ �� ���� ���� �ʱ�ȭ
    void        Run();         // ���� ���� ����
    void        Shutdown();    // ���� ���� �� �ڿ� ����

    //void        AddSession(SessionRef session);
    //void        ReleaseSession(SessionRef session);

private:
    std::atomic_bool            _running = false;
    std::vector<std::thread>    _workerThreads;    // IOCP �۾� ������ Ǯ
    std::shared_ptr<IocpCore>   _iocpCore;

    std::unique_ptr<Timer>      _timer;
public:

};

