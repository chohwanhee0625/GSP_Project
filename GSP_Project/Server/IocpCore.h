#pragma once


class IocpCore : public std::enable_shared_from_this<IocpCore>
{
public:
	IocpCore();
	~IocpCore();
	HANDLE		GetHandle() { return _iocpHandle; }

	bool		Register(SOCKET socket);
	bool		Dispatch(uint32 timeoutMs = INFINITE);	// CP�� �����ϸ鼭 ����ϴ� �Լ�, �۾������� ��û����

private:
	HANDLE		_iocpHandle;
};

