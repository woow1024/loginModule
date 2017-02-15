#pragma once

#include "safequeue.h"
#include "safethread.h"  

template </* typename classT,*/ typename tagT > class CSafeQueueThread 
: public  safe_thread_base< CSafeQueueThread<tagT>, 1>
{
public:
	CSafeQueueThread ()
	{
		m_bRunning = false;
	}
	virtual ~CSafeQueueThread()
	{
		ClearAllData();
	}
protected:
	void Start()
	{
		if(m_bRunning)
		{
			return;
		}
		m_bRunning = true; 
		safe_start_thread(&CSafeQueueThread::HanldePacketThread, 0, 0);
	}

	void Stop()
	{
		if(false == m_bRunning)
		{
			return;
		}
		m_bRunning = false; 
		CSafeQueueThread::join_all_thread();
	}
	int GetSize()
	{
		return m_cDataQueque.size();
	}
	virtual void ClearAllData()
	{

	}

	virtual bool HandOnePacket(tagT onePacket)
	{
		return false;
	}

	void HanldePacketThread(void* param)
	{
		bool bOK = false;
		tagT pOnePacket;
		while(m_bRunning)
		{  
			bOK = m_cDataQueque.popEx(pOnePacket); 
			if(false == bOK)
			{
#ifdef _WIN32
					Sleep(20);
#else
					usleep(5000);
#endif 

				
				continue;
			}
			HandOnePacket(pOnePacket);
		}

	}
	

protected:
	CSafeQueue<tagT> m_cDataQueque;

	bool m_bRunning; 
	
};

