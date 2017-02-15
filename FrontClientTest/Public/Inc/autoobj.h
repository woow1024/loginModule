#pragma once

class CAutoCritical
{
public:
	CAutoCritical(LPCRITICAL_SECTION lpCriticalSection = NULL)
	{
		m_lpCriticalSection = NULL;

		if (lpCriticalSection)
		{
			Lock(lpCriticalSection);
		}
	}

	~CAutoCritical()
	{
		UnLock();
	}

	void UnLock()
	{
		if (m_lpCriticalSection != NULL)
		{
			LeaveCriticalSection(m_lpCriticalSection);
		}

		m_lpCriticalSection = NULL;
	}

	void Lock(LPCRITICAL_SECTION lpCriticalSection)
	{
		UnLock();

		m_lpCriticalSection = lpCriticalSection;
		EnterCriticalSection(m_lpCriticalSection);
	}

private:
	LPCRITICAL_SECTION m_lpCriticalSection;
};

class CAutoSemaphore
{
public:
	CAutoSemaphore()
	{
		m_handle = NULL;
		m_dwLastError = 0;
	}
	virtual ~CAutoSemaphore()
	{
		if (m_handle)
			ReleaseSemaphore(m_handle,1,NULL);
		CloseHandle(m_handle);
	};

	BOOL CreateSemaphore(LPCTSTR lpName)
	{
		if (lpName == NULL) return FALSE;

		m_handle = ::CreateSemaphore(NULL, 1, 1, lpName);
		m_dwLastError = GetLastError();
		if (m_handle)
		{
			if (m_dwLastError == ERROR_ALREADY_EXISTS)
			{
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
private:
	DWORD m_dwLastError;
	HANDLE m_handle;
};

class CAutoResource
{
public:
	CAutoResource(HINSTANCE hResInstance = AfxGetResourceHandle())
	{
		m_hResInstance = AfxGetResourceHandle();

		AfxSetResourceHandle(hResInstance);
	}

	~CAutoResource()
	{
		AfxSetResourceHandle(m_hResInstance);
	}

private:
	HINSTANCE m_hResInstance;
};



#define  INIT_LOCK(a)   ::InitializeCriticalSection(&a);

#define  DEL_LOCK(a)	::DeleteCriticalSection(&a);

#define  ENTER_LOCK(a)  CAutoCritical cAILock(&a);

class CCriticalSectionBase
{
public: 
	CCriticalSectionBase()
	{
		INIT_LOCK(m_cSection)
	}
	~CCriticalSectionBase()
	{
		DEL_LOCK(m_cSection)
	}
protected:
	CRITICAL_SECTION m_cSection;
};