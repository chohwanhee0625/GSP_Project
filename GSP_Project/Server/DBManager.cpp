#include "pch.h"
#include "DBManager.h"

concurrency::concurrent_queue<DBEvent> DBQueue;

void DBManager::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER iError;
	WCHAR wszMessage[1000];
	WCHAR wszState[SQL_SQLSTATE_SIZE + 1];
	if (RetCode == SQL_INVALID_HANDLE) {
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}
	while (SQLGetDiagRec(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5)) {
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}

bool DBManager::DBConnection()
{
	SQLRETURN retcode;

	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		std::wcerr << L"SQLAllocHandle ENV failed\n";
		return false;
	}

	// Set the ODBC version environment attribute  
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		std::wcerr << L"SQLSetEnvAttr failed\n";
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		henv = SQL_NULL_HENV;
		return false;
	}

	// Allocate connection handle  
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		std::wcerr << L"SQLAllocHandle DBC failed\n";
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		henv = SQL_NULL_HENV;
		return false;
	}

	// Set login timeout to 5 seconds  
	SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

	// Connect to data source  
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"gsp2020182040odbc", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		std::wcerr << L"SQLConnect failed\n";
		HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = SQL_NULL_HDBC;
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		henv = SQL_NULL_HENV;
		return false;
	}

	std::cout << "[DB] Connection Success" << std::endl;
	return true;
}

bool DBManager::DBFindById(const char* id, char* name, short* x, short* y)
{


	return true;
}

bool DBManager::DBUpdatePosition(const char* id, short x, short y)
{
	return false;
}

bool DBManager::DBDisconnect()
{
	return false;
}

void DBManager::ProcessDB()
{
	DBEvent dbEvent;
	while (DBQueue.try_pop(dbEvent)) {
		switch (dbEvent.eventType) {
		case DBEventType::DB_EVENT_LOGIN:
			// Handle login event
			break;
		case DBEventType::DB_EVENT_LOGOUT:
			// Handle logout event
			break;
		case DBEventType::DB_EVENT_STAT_CHANGE:
			// Handle stat change event
			break;
		default:
			break;
		}
	}
}

