#pragma once


class IocpCore : public std::enable_shared_from_this<IocpCore>
{
public:
	IocpCore();
	~IocpCore();
	HANDLE		GetHandle() { return _iocpHandle; }

	bool		Register(SOCKET socket);
	bool		Dispatch(uint32 timeoutMs = INFINITE);	// CP를 감시하면서 대기하는 함수, 작업스레드 원청느낌

private:
	HANDLE		_iocpHandle;
};

