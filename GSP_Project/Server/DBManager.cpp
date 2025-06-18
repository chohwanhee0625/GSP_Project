#include "pch.h"
#include "DBManager.h"
#include "Session.h"

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

bool DBManager::DBFindById(const char* id, char* name, short* x, short* y, short* hp, short* level, int* exp, char* flag)
{
	if (hdbc == SQL_NULL_HDBC) {
		return false;
	}

	SQLRETURN retcode;
	SQLWCHAR user_id[MAX_ID_LENGTH + 1], user_name[MAX_ID_LENGTH + 1];
	SQLINTEGER recent_x, recent_y, Hp, Level, Exp;
	SQLCHAR Logged;
	SQLLEN    cbId, cbName = 0, cbX = 0, cbY = 0, cbHp, cbLevel, cbExp, cbLog;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	const wchar_t* sqlQuery = L"{CALL find_and_login_user(?)}";
	retcode = SQLPrepare(hstmt, (SQLWCHAR*)sqlQuery, SQL_NTS);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	wchar_t wid[21] = { 0 };
	ConvertCharToWide(id, wid, 21);
	cbId = SQL_NTS;
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR,
		20, 0, (SQLPOINTER)wid, sizeof(wid), &cbId);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	// Bind columns user_id | user_name | recent_x | recent_y | hp | level | exp | is_login |
	retcode = SQLBindCol(hstmt, 1, SQL_C_WCHAR, user_name, sizeof(user_name), &cbName);
	retcode = SQLBindCol(hstmt, 2, SQL_C_LONG, &Level, 0, &cbLevel);
	retcode = SQLBindCol(hstmt, 3, SQL_C_LONG, &Exp, 0, &cbExp);
	retcode = SQLBindCol(hstmt, 4, SQL_C_LONG, &recent_x, 0, &cbX);
	retcode = SQLBindCol(hstmt, 5, SQL_C_LONG, &recent_y, 0, &cbY);
	retcode = SQLBindCol(hstmt, 6, SQL_C_LONG, &Hp, 0, &cbHp);
	retcode = SQLBindCol(hstmt, 7, SQL_C_BIT, &Logged, sizeof(Logged), &cbLog);

	retcode = SQLExecute(hstmt);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	retcode = SQLFetch(hstmt);
	if (retcode == SQL_NO_DATA) {
		// 아이디가 없음
		*flag = LOGIN_NOEX;
		return false;
	}
	else if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		int needed = WideCharToMultiByte(CP_UTF8, 0, user_name, -1, nullptr, 0, nullptr, nullptr);
		if (needed > MAX_ID_LENGTH + 1) {
			// name 버퍼가 충분하지 않거나, 잘라야 한다면 에러 처리
			*flag = LOGIN_ERR;
			return false;
		}
		if (1 == Logged) {
			*flag = LOGIN_USING;
			return false;
		}
		WideCharToMultiByte(CP_UTF8, 0, user_name, -1, name, needed, nullptr, nullptr);
		*x = recent_x; *y = recent_y; *hp = Hp; *level = Level; *exp = Exp;
		return true;
	}
	else {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	return true;
}

bool DBManager::DBLogOutById(const char* id)
{
	if (hdbc == SQL_NULL_HDBC) {
		return false;
	}

	SQLRETURN retcode;
	SQLLEN    cbId;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	const wchar_t* sqlQuery = L"{CALL user_logout(?)}";
	retcode = SQLPrepare(hstmt, (SQLWCHAR*)sqlQuery, SQL_NTS);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	wchar_t wid[21] = { 0 };
	ConvertCharToWide(id, wid, 21);
	cbId = SQL_NTS;
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR,
		20, 0, (SQLPOINTER)wid, sizeof(wid), &cbId);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	retcode = SQLExecute(hstmt);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	return true;
}

bool DBManager::DBSignById(const char* id, const char* name)
{
	if (hdbc == SQL_NULL_HDBC) {
		return false;
	}

	SQLRETURN retcode;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	if (!SQL_SUCCEEDED(retcode)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	static const wchar_t* sql = L"{CALL dbo.add_new_user(?, ?)}";
	retcode = SQLPrepare(hstmt, (SQLWCHAR*)sql, SQL_NTS);
	if (!SQL_SUCCEEDED(retcode)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return false;
	}

	wchar_t wid[21] = { 0 };
	ConvertCharToWide(id, wid, 21);
	SQLLEN cbId = SQL_NTS;
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR,
		20, 0, (SQLPOINTER)wid, sizeof(wid), &cbId);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	wchar_t wname[21] = { 0 };
	ConvertCharToWide(name, wname, 21);
	SQLLEN cbName = SQL_NTS;
	retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR,
		20, 0, (SQLPOINTER)wname, sizeof(wname), &cbName);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}


	retcode = SQLExecute(hstmt);
	if (!SQL_SUCCEEDED(retcode)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return false;
	}

	return true;
}



bool DBManager::DBUpdateStatus(const char* id, short x, short y, short hp, short level, int exp)
{
	if (hdbc == SQL_NULL_HDBC) {
		return false;
	}

	SQLRETURN retcode;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	const wchar_t* sqlQuery = L"{CALL update_user_status(?, ?, ?, ?, ?, ?)}";
	retcode = SQLPrepare(hstmt, (SQLWCHAR*)sqlQuery, SQL_NTS);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	// id
	wchar_t wid[21] = { 0 };
	ConvertCharToWide(id, wid, 21);
	SQLLEN cbId = SQL_NTS;
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR,
		20, 0, (SQLPOINTER)wid, sizeof(wid), &cbId);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	// level
	SQLLEN cbLevel = 0;
	retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
		0, 0, &level, 0, &cbLevel);
	if (!SQL_SUCCEEDED(retcode)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return false;
	}

	// exp
	SQLLEN cbExp = 0;
	retcode = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
		0, 0, &exp, 0, &cbExp);
	if (!SQL_SUCCEEDED(retcode)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return false;
	}

	// x
		// 3-4) @x INT
	SQLLEN cbX = 0;
	SQLINTEGER xi = x;
	retcode = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
		0, 0, &xi, 0, &cbX);
	if (!SQL_SUCCEEDED(retcode)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return false;
	}
	// y
	SQLLEN cbY = 0;
	SQLINTEGER yi = y;
	retcode = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
		0, 0, &yi, 0, &cbY);
	if (!SQL_SUCCEEDED(retcode)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return false;
	}

	// hp
	SQLLEN cbHp = 0;
	SQLINTEGER hpi = hp;
	retcode = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
		0, 0, &hpi, 0, &cbHp);
	if (!SQL_SUCCEEDED(retcode)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		return false;
	}

	retcode = SQLExecute(hstmt);
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return false;
	}

	return true;
}

bool DBManager::DBDisconnect()
{
	return false;
}

void DBManager::ProcessDB()	// 메인스레드 1개에서 동작
{
	DBEvent dbEvent;
	while (DBQueue.try_pop(dbEvent)) {
		switch (dbEvent.eventType) {
		case DBEventType::DB_EVENT_LOGIN:
		{
			// Handle login event
			SessionRef session = clients[dbEvent.id];
			char name[MAX_ID_LENGTH];
			short x = 0, y = 0;
			short hp; short level; int exp;
			char flag;
			if (true == DBFindById(session->_db_id, name, &x, &y, &hp, &level, &exp, &flag)) {
				sc_packet_avatar_info login_info;
				login_info.id = session->_id;
				login_info.x = x; login_info.y = y;
				std::string result{ name };
				result.erase(remove(result.begin(), result.end(), ' '), result.end());
				strcpy_s(login_info.name, result.c_str());
				login_info.level = level;
				login_info.exp = exp;
				login_info.max_hp = 100;
				login_info.hp = hp;
				session->OnLogin(login_info, login_info.name);
			}
			else {
				sc_packet_login_fail login_fail;
				login_fail.size = sizeof(sc_packet_login_fail);
				login_fail.type = S2C_P_LOGIN_FAIL;
				login_fail.id = dbEvent.id;
				login_fail.reason = flag;
				clients[dbEvent.id]->Send(&login_fail);
			}
		}
			break;
		case DBEventType::DB_EVENT_LOGOUT:
		{
			// Handle logout event
			auto client = clients[dbEvent.id];
			DBUpdateStatus(client->_db_id, client->x, client->y, client->hp, client->level, client->exp);
			DBLogOutById(client->_db_id);
		}
			break;
		case DBEventType::DB_EVENT_STAT_CHANGE:
		{
			// Handle stat change event
			auto client = clients[dbEvent.id];
			DBUpdateStatus(client->_db_id, client->x, client->y, client->hp, client->level, client->exp);
		}
			break;
		case DBEventType::DB_EVENT_SIGN:
		{
			auto client = clients[dbEvent.id];
			if (true == DBSignById(client->_db_id, client->_name)) {
				DBEvent event;
				event.eventType = DBEventType::DB_EVENT_LOGIN;
				event.id = client->_id;
				DBQueue.push(event);
			}
			else {
				std::cout << "Sign Error" << std::endl;
			}
		}
			break;
		default:
			break;
		}
	}
}

