#pragma once


template <class T> class lockableobject
{
public:
	lockableobject()
	{
		::InitializeCriticalSection(&csLock);
	}
	~lockableobject()
	{
		::DeleteCriticalSection(&csLock);
	}

	LPCRITICAL_SECTION getlock() { return &csLock; }

	T* get() const { return &t; }
	T* operator ->() const { return get(); }
	T& operator *() const { return t; }

	T* get() { return &t; }
	T* operator ->() { return get(); }
	T& operator *() { return t; }

	void lock() 
	{
		::EnterCriticalSection(&csLock);
	}

	void unlock()
	{
		::LeaveCriticalSection(&csLock);
	}

private:
	CRITICAL_SECTION		csLock;
	T						t;
};