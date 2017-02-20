#pragma once
#include <string>
#include "packethead.h"
#include "std_macro.h"
#include "singletonbase.h"
#include "safethread.h" 
#include "safequeue.h"
#include "epollserver.h"
#include "binarybuffe.h"
#include <sys/time.h>
 
struct tagServerInfo
{
	string strIP;
	int nPort;
	string strUser;
	string strPWD;
	string strKey;
	tagServerInfo()
	{
		nPort = 0;
	}
};

typedef CSafeQueue<tagPacket*> ListDataPacket;
class CDCManager:public new_singletonbase<CDCManager>,public safe_thread_base<CDCManager, 4>
{
	friend class new_singletonbase<CDCManager>;
protected:
	CDCManager(){};
	//IMPLEMENT_NEW_SINGLETON_MODE(CDCManager)
private:
	virtual ~CDCManager(void);
	void InitInstance();
public:
	BOOL ReadyConnect();
	void Start();
	void Stop();
	void Test_Publish();

protected: 
	
private:
	BOOL ConnectCC();
	BOOL ListenFS();
	BOOL InitRMQ();

	void ClearAllRecvDataPacket();
	void ClearAllSendPacket();
	
 
	void HanldeAllPacketThread(void* param);
	void HanlleHeartBeatThread(void* param);
	void HanlleAllSendThread(void* param);	//�û��������߳�
	void HandleLogoutThread(void *param);	//����socket�Ͽ�logout

	void RemoveDeadSocket();
public:
	void SendOnePacket(DWORD dwCmdID, BYTE* pButter, int nBufferLen, DWORD dwAppendID);
	void SendOnePacket(tagPacket * pOnePacket); //�����������͵����Ͷ��У����ͻ�Ӧ 

	void PushOnePacket(tagPacket * pOnePacket);	//���յ����ݰ��͵����ն��У�������

	void SendHeartBeatToClient(tagPacket * pOnePacket);
	void GetLoginResponse(string & strURL, string& strArg, CBinaryBuffer& cSendBuffer);
	void GetUserTokenResponse(string & strURL, string& strArg, CBinaryBuffer& cSendBuffer);
	void OnLoginRespons(string& strJson);

	struct timeval start,end;
private:
	void OnMessageHttpAuthorReq(tagPacket * pOnePacket); 
	void OnMessageLoginReq(tagPacket * pOnePacket); 
	void PushPacketIntoQueque(tagPacket * pOnePacket, ListDataPacket& lstPacket, bool bAddTag);
	void ReadXML();

private: 
	net_event_callback * m_pEpollServerCallback;
	epollserver * m_pFrontServer;

	ListDataPacket m_lstRecvPacket; 

	ListDataPacket m_lstSendPacket; 
	string m_strTokenKey;
	BOOL m_bRunning; 
	string m_strAuthorURL;
};


//==========ҵ��ص��ӿ�===================
//===========1 web ��Ȩ==================== 
int _http_authorize_on_data_recv(int s, char* data, size_t len, DWORD pAppendData); 

int OnDataRead(int s);