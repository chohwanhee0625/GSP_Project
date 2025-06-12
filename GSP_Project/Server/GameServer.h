#pragma once


class GameServer : public std::enable_shared_from_this<GameServer>
{
public:
    GameServer() : _running(true) {};
    ~GameServer() { Shutdown(); }

    std::shared_ptr<GameServer> GetService() {return }

    void        Initialize();  // 네트워크 및 게임 관련 초기화
    void        Run();         // 서버 메인 루프
    void        Shutdown();    // 서버 종료 및 자원 해제

    void        StartAccept();
    void        ProcessAccept();

    void        AddSession();
    void        ReleaseSession();

private:
    std::atomic_bool            _running = false;
    std::vector<std::thread>    _workerThreads;    // IOCP 작업 스레드 풀
    std::shared_ptr<IocpCore>   _iocpCore;
    SessionRef                 _listener;

    std::unordered_map<int, SessionRef> clients;
};

