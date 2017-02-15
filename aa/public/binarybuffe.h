#ifndef _BINARY_BUFFER_H
#define _BINARY_BUFFER_H

#include <iostream>
using namespace std;
#include<string.h>

class CBinaryBuffer
{
public:
	CBinaryBuffer(int nOrgSize, int nReallocsize)
	{
		m_nBufferSize = 0;
		m_nReallocSize = nReallocsize;
		m_nAllocSize = nOrgSize;
		if(m_nAllocSize <= 0)
		{
			m_nAllocSize = 1024;
		}
		if(m_nReallocSize <= 0)
		{
			m_nReallocSize = 1024;
		}
		m_pszBuffer = new char[m_nAllocSize];
		memset(m_pszBuffer, 0, m_nAllocSize);
	}

	CBinaryBuffer(CBinaryBuffer& cB2)
	{
		m_nBufferSize = cB2.GetLength();
		m_pszBuffer = new char[m_nBufferSize];
		memcpy(m_pszBuffer, cB2.GetData(), m_nBufferSize);
	}
	CBinaryBuffer& operator=(CBinaryBuffer& cB2)
	{
		m_nBufferSize = cB2.GetLength();
		m_pszBuffer = new char[m_nBufferSize];
		memcpy(m_pszBuffer, cB2.GetData(), m_nBufferSize);
		return *this;
	}
	~CBinaryBuffer()
	{
		delete [] m_pszBuffer;
		m_pszBuffer = NULL;
	}
public:
	const char * GetData()
	{
		return m_pszBuffer;
	}
	int GetLength()
	{
		return m_nBufferSize;
	}
	void Clear() 
	{
		memset(m_pszBuffer, 0, m_nAllocSize);
		m_nBufferSize = 0;
	}
	bool Write(const char* str, size_t len)
	{
		if(len <= 0 || 0 == str)
		{
			return false;
		}
		if(m_nBufferSize + len > m_nAllocSize)
		{
			m_nAllocSize = m_nBufferSize + len + m_nReallocSize;
			char * pszTemp = new char[m_nAllocSize ];
			memcpy(pszTemp, m_pszBuffer, m_nBufferSize);
			delete [] m_pszBuffer;
			m_pszBuffer = pszTemp;
		}
		memcpy(m_pszBuffer + m_nBufferSize, str, len);
		m_nBufferSize += len;
		return true;
	}
	bool Write(int i)
	{
		char * pBuffer = (char*)&i;
		return Write(pBuffer, sizeof(i));
	}
	bool Write(short i)
	{
		char * pBuffer = (char*)&i;
		return Write(pBuffer, sizeof(i));
	}
	bool Write(char c)
	{
		char * pBuffer = (char*)&c;
		return Write(pBuffer, sizeof(c));

	}
	bool Write(string& strVal)
	{
		return Write(strVal.c_str(), strVal.length());
	}
private:
	char * m_pszBuffer;
	int m_nBufferSize;
	int m_nReallocSize;
	int m_nAllocSize;
};

#endif