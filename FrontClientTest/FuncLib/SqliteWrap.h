#pragma once

#include "sqlite3.h"
#include <Windows.h>

#include <atlstr.h>

#undef GW_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define GW_EXT_CLASS _declspec(dllexport) 
#else
#define GW_EXT_CLASS _declspec(dllimport)
#endif

#include <vector>
using namespace std;
class smart_buffer				// auto malloc and free
{
public:
	smart_buffer(int len = 0)
	{
		inner_buff	   = NULL;
		inner_len	   = 0;
		inner_capacity = 0;

		if (len > 0)
		{
			alloc(len);
		}
	}

	~smart_buffer()
	{
		release();
	}

	void release()
	{
		free(inner_buff);

		inner_buff     = NULL;
		inner_len      = 0;
		inner_capacity = 0;
	}

	BOOL alloc(size_t len)
	{
		if (len <= inner_capacity)
		{
			inner_len = len;
			memset(inner_buff, 0, inner_len);
			return TRUE;
		}

		release();

		inner_buff = (char*)malloc(len);

		if (inner_buff == NULL)
		{
			return FALSE;
		}

		inner_len		 = len;
		inner_capacity	 = len;

		memset(inner_buff, 0, inner_len);
		return TRUE;
	}

	void setdata(const char * pData, int len)
	{
		alloc(len);
		memcpy(inner_buff, pData, len);
	}

	operator char*()
	{
		return inner_buff;
	}

	char* getbuffer()
	{
		return inner_buff;
	}

	size_t getlength()
	{
		return inner_len;
	}

	bool isvliadpointer(void* address)
	{
		return address > inner_buff && address < (inner_buff + inner_len);
	}
private:
	char* inner_buff;
	size_t	inner_len;
	size_t  inner_capacity;
};

class GW_EXT_CLASS SqliteString
{
public:
	SqliteString(const char* lpText, BOOL bUtf8 = FALSE);
	SqliteString(const unsigned char* lpText);
	SqliteString(const wchar_t* lpText);

	inline UINT Length() { return m_ptrBuffer.getlength(); }
	inline const char* Buffer() { return m_ptrBuffer.getbuffer(); }

	operator const char*() { return m_ptrBuffer.getbuffer(); }

	CStringA AsString();
	const char* AsUTF8() { return m_ptrBuffer.getbuffer(); }
private:
	BOOL	WString2Buffer(const wchar_t* lpData, int nLen);
private:
	smart_buffer m_ptrBuffer;
};

typedef vector<CString>		CSRINGVEC;
typedef vector<CSRINGVEC>	CSRINGVECVEC;

template<typename T> class nocopyclass
{
public:
	nocopyclass() {}
private:
	nocopyclass(const nocopyclass&);
	void operator =(const nocopyclass&);
protected:
	~nocopyclass() {}
};

class GW_EXT_CLASS SqliteWrap : public nocopyclass<SqliteWrap>
{
public:
	SqliteWrap(void);
	~SqliteWrap(void);

public:
	BOOL	Open(LPCTSTR lpFileName, BOOL bCreateIfNotExsit = FALSE);
	void	Close();

	sqlite3* GetDatabase() const { return m_pDatabase; }

	BOOL	Exec(LPCTSTR lpSQL);

	BOOL	BeginTransaction();
	BOOL	CommitTransaction();
	BOOL	RollbackTransaction();

	BOOL	TableSnapshot(LPCTSTR lpSQL, CSRINGVECVEC& vecSnapshot, BOOL bIncludeTitle = FALSE);
private:
	sqlite3*		m_pDatabase;
};

class GW_EXT_CLASS SqliteQueryTask : public nocopyclass<SqliteQueryTask>
{
public:
	SqliteQueryTask(SqliteWrap* pSqliteWrap);
	~SqliteQueryTask();

	BOOL TaskBegin(LPCTSTR lpSQL);
	void TaskFinish();

	BOOL MoveNextRow();
	int	 GetRowCount();
	int  GetColCount();
	
	BOOL MoveTo(int nRow);

	CString ValueAsString(int nCol);
	int ValueAsInt(int nCol);
	__int64 ValueAsInt64(int nCol);
	double ValueAsDouble(int nCol);
private:
	inline BOOL IsDatabaseValid() const { return m_pSqliteWrap != NULL; }
	inline BOOL IsStmtValid() const { return m_pStmt != NULL; }
private:
	SqliteWrap*	m_pSqliteWrap;
	sqlite3_stmt* m_pStmt;
};