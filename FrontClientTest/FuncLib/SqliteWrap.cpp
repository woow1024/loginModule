#include "StdAfx.h"
#include "SqliteWrap.h"
#include <cassert>
#include <iostream>
using namespace std;

//#define  ASSERT		assert

#pragma comment(lib, "sqlite3.lib")

SqliteWrap::SqliteWrap(void) : m_pDatabase(NULL)
{
}

SqliteWrap::~SqliteWrap(void)
{
	Close();
}

BOOL SqliteWrap::Open(LPCTSTR lpFileName, BOOL bCreateIfNotExsit)
{
	Close();

	int nFlags = SQLITE_OPEN_READWRITE;

	if (bCreateIfNotExsit)
	{
		nFlags |= SQLITE_OPEN_CREATE;
	}

	int nRet = sqlite3_open_v2(SqliteString(lpFileName), &m_pDatabase, nFlags, NULL);
	
	return nRet == SQLITE_OK;
}

void SqliteWrap::Close()
{
	if (m_pDatabase)
	{
		sqlite3_close(m_pDatabase);
		m_pDatabase = NULL;
	}
}

BOOL SqliteWrap::Exec(LPCTSTR lpSQL)
{
	SqliteString strSQL(lpSQL);

	int nRet = sqlite3_exec(m_pDatabase, strSQL, NULL, NULL, NULL);

	return nRet == SQLITE_OK;
}

BOOL SqliteWrap::BeginTransaction()
{
	int nRet = sqlite3_exec(m_pDatabase, "begin transaction", NULL, NULL, NULL);

	return nRet == SQLITE_OK;
}

BOOL SqliteWrap::CommitTransaction()
{
	int nRet = sqlite3_exec(m_pDatabase, "commit transaction", NULL, NULL, NULL);

	return nRet == SQLITE_OK;
}

BOOL SqliteWrap::RollbackTransaction()
{
	int nRet = sqlite3_exec(m_pDatabase, "rollback transaction", NULL, NULL, NULL);

	return nRet == SQLITE_OK;
}

BOOL SqliteWrap::TableSnapshot(LPCTSTR lpSQL, CSRINGVECVEC& vecSnapshot, BOOL bIncludeTitle)
{
	SqliteString strSQL(lpSQL);

	int nCols = 0, nRows = 0;
	char** pResult = NULL;

	int nRet = sqlite3_get_table(m_pDatabase, strSQL, &pResult, &nRows, &nCols, NULL);

	if (nRet != SQLITE_OK)
	{
		goto FUNCTIONEXIT;
	}

	if (nRows == 0 && !bIncludeTitle)
	{
		goto FUNCTIONEXIT;
	}

	if (nCols == 0)
	{
		goto FUNCTIONEXIT;
	}

	vecSnapshot.resize(nRows + bIncludeTitle);

	for (int n = 0 + !bIncludeTitle; n <= nRows; ++n)
	{
		CSRINGVEC& vec = vecSnapshot[n - !bIncludeTitle];
		vec.reserve(nCols);

		for (int i = 0; i < nCols; ++i)
		{
#ifdef UNICODE
			SqliteString strValue(pResult[n * nCols + i], TRUE);
			CString str(strValue.AsString());
			vec.push_back(CString(strValue.AsString()));
#else
			SqliteString strValue(pResult[n * nCols + i], TRUE);
			vec.push_back(strValue.AsString());
#endif
		}
	}

FUNCTIONEXIT:
	if (pResult != NULL)
	{
		sqlite3_free_table(pResult);
	}

	return nRet == SQLITE_OK;
}

SqliteString::SqliteString(const char* lpText, BOOL bUtf8) : m_ptrBuffer(1024)
{
	if (bUtf8)
	{
		m_ptrBuffer.setdata(lpText, strlen(lpText) + 1);
	}
	else
	{
		CStringW strTmp(lpText);
		WString2Buffer(strTmp, strTmp.GetLength());
	}
}

SqliteString::SqliteString(const wchar_t* lpText) : m_ptrBuffer(1024)
{
	 WString2Buffer(lpText, wcslen(lpText));
}

SqliteString::SqliteString(const unsigned char* lpText)
{
	char* pData = (char*)lpText;
	m_ptrBuffer.setdata(pData, strlen(pData) + 1);
}

BOOL SqliteString::WString2Buffer(const wchar_t* lpData, int nLen)
{
	ASSERT(lpData != NULL);

	if (lpData == NULL)
	{
		return FALSE;
	}

	DWORD dwSize = WideCharToMultiByte(CP_UTF8, 0, lpData, nLen, NULL, 0, NULL, NULL);
	m_ptrBuffer.alloc(dwSize);
	dwSize = WideCharToMultiByte(CP_UTF8, 0, lpData, nLen, m_ptrBuffer.getbuffer(), m_ptrBuffer.getlength(), NULL, NULL);
	ASSERT(dwSize == m_ptrBuffer.getlength());

	return dwSize > 0;
}

CStringA SqliteString::AsString()
{
	if (m_ptrBuffer.getlength() <= 0)
	{
		return CStringA();
	}

	DWORD dwSize = MultiByteToWideChar(CP_UTF8, 0, m_ptrBuffer.getbuffer(), m_ptrBuffer.getlength(), NULL, 0);
	smart_buffer buffer(dwSize * sizeof(wchar_t));
	dwSize = MultiByteToWideChar(CP_UTF8, 0, m_ptrBuffer.getbuffer(), m_ptrBuffer.getlength(), (wchar_t*)buffer.getbuffer(), dwSize);

	return CStringA((const wchar_t*)buffer.getbuffer());
}

SqliteQueryTask::SqliteQueryTask(SqliteWrap* pSqliteWrap) : m_pSqliteWrap(pSqliteWrap)
{
	m_pStmt = NULL;
}

SqliteQueryTask::~SqliteQueryTask()
{
	TaskFinish();
}

void SqliteQueryTask::TaskFinish()
{
	if (m_pStmt != NULL)
	{
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}
}

BOOL SqliteQueryTask::TaskBegin(LPCTSTR lpSQL)
{
	if (!IsDatabaseValid())
	{
		return FALSE;
	}
	
	TaskFinish();

	SqliteString strSQL(lpSQL);
	int nRet = sqlite3_prepare_v2(m_pSqliteWrap->GetDatabase(), strSQL, strSQL.Length(), &m_pStmt, NULL);

	if (nRet != SQLITE_OK)
	{
		return FALSE;
	}

	return TRUE;
}

int SqliteQueryTask::GetColCount()
{
	if (!IsStmtValid())
	{
		return 0;
	}

	return sqlite3_column_count(m_pStmt);
}

int SqliteQueryTask::GetRowCount()
{
	if (!IsStmtValid())
	{
		return 0;
	}

	return 0;
}

BOOL SqliteQueryTask::MoveNextRow()
{
	return sqlite3_step(m_pStmt) == SQLITE_ROW;
}

CString SqliteQueryTask::ValueAsString(int nCol)
{
	SqliteString strValue(sqlite3_column_text(m_pStmt, nCol));

#ifdef UNICODE
	return CString(strValue.AsString());
#else
	return strValue.AsString();
#endif
}

int SqliteQueryTask::ValueAsInt(int nCol)
{
	int nColType = sqlite3_column_type(m_pStmt, nCol);

	return sqlite3_column_int(m_pStmt, nCol);
}

__int64 SqliteQueryTask::ValueAsInt64(int nCol)
{
	return sqlite3_column_int64(m_pStmt, nCol);
}

double SqliteQueryTask::ValueAsDouble(int nCol)
{
	return sqlite3_column_double(m_pStmt, nCol);
}


