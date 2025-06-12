#pragma once


class GameServer : public std::enable_shared_from_this<GameServer>
{
public:
    GameServer() : _running(true) {};
    ~GameServer() { Shutdown(); }

    std::shared_ptr<GameServer> GetService() {return }

    void        Initialize();  // ��Ʈ��ũ �� ���� ���� �ʱ�ȭ
    void        Run();         // ���� ���� ����
    void        Shutdown();    // ���� ���� �� �ڿ� ����

    void        StartAccept();
    void        ProcessAccept();

    void        AddSession();
    void        ReleaseSession();

private:
    std::atomic_bool            _running = false;
    std::vector<std::thread>    _workerThreads;    // IOCP �۾� ������ Ǯ
    std::shared_ptr<IocpCore>   _iocpCore;
    SessionRef                 _listener;

    std::unordered_map<int, SessionRef> clients;
};

