#include <WinSock2.h>
#include <winsock.h>
#include <thread>
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <chrono>
#include <queue>
#include <array>
#include <memory>
#include <string>
#include <iostream>

#include "NetworkModule.h"

#include "../Server/protocol.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;
using namespace chrono;

void error_display(const char* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L"¿¡·¯" << lpMsgBuf << std::endl;

	MessageBox(0, lpMsgBuf, L"ERROR", 0);
	LocalFree(lpMsgBuf);
	// while (true);
}

void InitializeNetwork(std::string server_ip)
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);


}
