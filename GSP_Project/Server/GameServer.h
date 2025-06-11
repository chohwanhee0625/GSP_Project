#pragma once


class GameServer
{
public:
    GameServer() : _running(true) {};
    ~GameServer() { Shutdown(); }

    void        Initialize();  // ��Ʈ��ũ �� ���� ���� �ʱ�ȭ
    void        Run();         // ���� ���� ����
    void        Shutdown();    // ���� ���� �� �ڿ� ����

    void        AddSession();
    void        ReleaseSession();


private:
    std::atomic_bool            _running = false;
    std::vector<std::thread>    _workerThreads;    // IOCP �۾� ������ Ǯ
    std::shared_ptr<IocpCore>   _iocpCore;
};

