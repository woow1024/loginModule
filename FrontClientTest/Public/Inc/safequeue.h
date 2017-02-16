#pragma once

#include <queue>
#include <algorithm>
#include <vector>
#include "autoobj.h"
/// A thread safe monitored queue
template < typename T > class CSafeQueue
{
public:
	CSafeQueue()
	{
		INIT_LOCK(m_cSection);
	}
	~CSafeQueue()
	{
		DEL_LOCK(m_cSection);
	}
	void push( const T& value )
	{
		ENTER_LOCK(m_cSection);
		m_queue.push( value );
	}

	T pop()
	{
		static T tmp;
		ENTER_LOCK(m_cSection);
		if ( !m_queue.size() ) return tmp;
		T value = m_queue.front();
		m_queue.pop();
		return value;
	}
	
	bool popEx(T& tVal)
	{
		ENTER_LOCK(m_cSection);
		if ( 0 == m_queue.size() )
		{
			return false;
		}
		tVal = m_queue.front();
		m_queue.pop();
		return true;
	}

	bool IsEmpty()
	{
		ENTER_LOCK(m_cSection);
		return (0 == m_queue.size());
	} 
	int size()
	{
		ENTER_LOCK(m_cSection);
		return m_queue.size();
	}
private:
	CRITICAL_SECTION m_cSection;
	std::queue < T > m_queue;
};


template <typename T> void ClearVectorItemPtr( std::vector<T>& vecPtr )
{
	int nItemNum = vecPtr.size();
	for(int ii = 0; ii < nItemNum; ii ++)
	{
		T itePtr = vecPtr[ii];
		delete itePtr;
	}
	vecPtr.clear();
};