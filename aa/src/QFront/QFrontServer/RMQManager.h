#pragma once

#include <string>
#include "std_macro.h"
#include "singletonbase.h"
#include "safethread.h" 
#include "safequeue.h"
#include "AMQPcpp.h"
#include <sys/time.h>
class CRMQManager:public new_singletonbase<CRMQManager>,public safe_thread_base<CRMQManager, 1>
{
	friend class new_singletonbase<CRMQManager>;
protected:
	CRMQManager(){};
public: 
private: 
	void InitInstance();
	virtual ~CRMQManager(void);
private:
	AMQP * m_pRMQ;
	AMQPQueue * m_pReplayQueque;
	AMQPExchange * m_pExchange;
	bool m_bRunning; 
public:
	void InitRMQ();
	void StarConsumer();
	void StopConsumer();
	void Publish(string& strJson, bool bRPC = false, string strRouterKey=string("answer"));
	void PublishLogout(int nID, string strRouterKey = string("logout"));
	pthread_mutex_t m_mutex;

private:
	void Stop();
	void HandleRMQQuequeThread(void* param);
};
