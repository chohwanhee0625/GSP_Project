#pragma once
#include <sqlext.h>  


enum DBEventType {
	DB_EVENT_LOGIN,
	DB_EVENT_LOGOUT,
	DB_EVENT_STAT_CHANGE
};
struct DBEvent {
	DBEventType eventType;
	int32 id;  // 업데이트 대상 User ID
};

extern concurrency::concurrent_queue<DBEvent> DBQueue;

class DBManager
{
public:
	static DBManager& GetInstance() {
		static DBManager instance;
		return instance;
	}

	bool DBConnection();
	bool DBFindById(const char* id, char* name, short* x, short* y);
	bool DBUpdatePosition(const char* id, short x, short y);
	bool DBDisconnect();

	void ProcessDB();

private:
	DBManager() = default;
	~DBManager() { DBDisconnect(); }

	DBManager(const DBManager&) = delete;
	DBManager& operator=(const DBManager&) = delete;

	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
	void show_error() { printf("error\n"); }

private:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;

};

