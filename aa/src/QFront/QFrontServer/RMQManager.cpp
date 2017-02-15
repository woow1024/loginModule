#include <netdb.h>
#include <sys/socket.h>

#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/time.h>
#include "RMQManager.h"
#include "DCManager.h"

static char szConsumerTag[] = "FS_tag";
#define RMQ_USERNAME "cc"
#define RMQ_PASSWORD "123"
int onCancel(AMQPMessage * message )
{
	return 0;
}

int  onMessage( AMQPMessage * message  )
{
	printf("login respose receive....\n");
	uint32_t nLen = 0;
	char * data = message->getMessage(&nLen);
	if (data)
		cout << data << endl;
//	printf(data);
	string strJson = data;
	CDCManager::GetInstance()->OnLoginRespons(strJson);
	//cout << "#" << i << " tag="<< message->getDeliveryTag() << " content-type:"<< message->getHeader("Content-type") ;
	//cout << " encoding:"<< message->getHeader("Content-encoding")<< " mode="<<message->getHeader("Delivery-mode")<<endl;
	return 0;
};

void CRMQManager::InitInstance()
{
	m_pRMQ = NULL;
	m_pReplayQueque = NULL;
	m_pExchange = NULL;
	m_bRunning = false;
	pthread_mutex_init(&m_mutex,NULL);
}

string GetLocalHostIP()
{
	char hname[128];
	struct hostent *hent;
	gethostname(hname, sizeof(hname));
	printf("hostname=[%s]\n",hname);
	hent = gethostbyname(hname);
	if( hent == NULL )
	{
	   printf("gethostbyname error,%s,%s",hname,strerror(errno));
	   return "error";

	}
	else
	{
	  char *ptr;
	  for( int i=0; hent->h_addr_list[i]; i++ )
	  {
		sprintf(hname, "%s", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
	   }
	  string strIP = hname;
	  return strIP;
	 }
}

CRMQManager::~CRMQManager(void)
{	
	this->Stop();
}

#define __NR_gettid 186 
void CRMQManager::InitRMQ()
{
	Stop();
	DELETE_PTR(m_pRMQ);	
	std::string conn="cc:123@localhost:5672/test";
	m_pRMQ = new AMQP(conn);
	int nThreadID = (long int)syscall(__NR_gettid);
	string strIP = GetLocalHostIP();

	//´´½¨Exchange
	char szExchangeKey[] = "FSExchange1";
	cout<<szExchangeKey<<endl;
	m_pExchange = m_pRMQ->createExchange();
	m_pExchange->Declare(szExchangeKey, "direct");


	

	
//	AMQPQueue * pCenterQueque = m_pRMQ->createQueue();
//	pCenterQueque->Declare("FSCenterBus2");	
//	pCenterQueque->Bind(szExchangeKey, "answer");


	char szKey[128];
	sprintf(szKey, "FS.%s_14", "test111");

	m_pReplayQueque = m_pRMQ->createQueue(szKey);
	m_pReplayQueque->Declare(); //ÅÅËû
	m_pReplayQueque->Bind(szExchangeKey, "FSReplay");
	m_pReplayQueque->setConsumerTag(szConsumerTag);
	m_pReplayQueque->addEvent(AMQP_MESSAGE, onMessage );
	m_pReplayQueque->addEvent(AMQP_CANCEL, onCancel );

}

void CRMQManager::StarConsumer()
{
	if(m_bRunning)
	{
		return;
	}
	m_bRunning = true;
	safe_start_thread(&CRMQManager::HandleRMQQuequeThread, NULL,0);

}


void CRMQManager::StopConsumer()
{
	if(m_pReplayQueque)
	{
		m_pReplayQueque->Cancel(szConsumerTag);
	}
}

void CRMQManager::Stop()
{
	StopConsumer();
	m_bRunning = false;
	join_all_thread();
}




void CRMQManager::HandleRMQQuequeThread(void* param)
{
	m_pReplayQueque->Consume(AMQP_NOACK | AMQP_EXCLUSIVE );//
}

void CRMQManager::Publish(string& strJson, bool bRPC, string strRouterKey)
{

	gettimeofday( &start, NULL );
	if(NULL == m_pRMQ)
	{
		return;
	}  
	pthread_mutex_lock(&m_mutex);
	string strReplayQueque;
	//ex->setHeader("Delivery-mode", 2);
	m_pExchange->setHeader("Content-type", "application/json");
	m_pExchange->setHeader("Content-encoding", "UTF-8");
	if(bRPC)
	{ 
		strReplayQueque = m_pReplayQueque->getName();
		m_pExchange->setHeader("Reply-to", strReplayQueque);
	}
	strReplayQueque = m_pReplayQueque->getName();
	cout << "queue name :"<<strReplayQueque << endl;
	m_pExchange->Publish(strJson, strRouterKey);
	pthread_mutex_unlock(&m_mutex);
}
