#include "pch.h"
#include "Over.h"
#include "Session.h"
#include "IocpCore.h"

IocpCore::IocpCore()
{
    _iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    if (_iocpHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "IOCP Handle Failed" << std::endl;
        exit(-1);
    }
}

IocpCore::~IocpCore()
{
    ::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(SOCKET socket)
{
    return ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), _iocpHandle, /*key*/0, 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
    DWORD numOfBytes = 0;
    ULONG_PTR key = 0;
    EX_OVER* ex_over = nullptr;

    if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT & key,
        OUT reinterpret_cast<LPOVERLAPPED*>(&ex_over), timeoutMs))
    {
        SessionRef session = ex_over->owner;
        if (nullptr == session) return false;

        if ((0 == numOfBytes) && ((ex_over->eventType == EventType::IO_RECV) || (ex_over->eventType == EventType::IO_SEND))) {
            session->Disconnect(static_cast<int>(key));
            if (ex_over->eventType == EventType::IO_SEND) delete ex_over;
            return true;
        }


        if (ex_over->eventType == EventType::IO_RECV || ex_over->eventType == EventType::IO_SEND)
        {
            session->Dispatch(ex_over, numOfBytes);
        }
        else
        {
            switch (ex_over->eventType)
            {
            //case EventType::IO_ACCEPT:
            //    ProcessAccept(ex_over, numOfBytes);
            //    break;
            //case EventType::NPC_MOVE:
            //    break;
            }
        }
    }

    return true;
}
