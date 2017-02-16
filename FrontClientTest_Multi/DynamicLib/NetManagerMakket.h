#pragma once
#include "singletonbase.h"
#include "SafeThread.h"
#include "safequeue.h"
#include "SSLSocket.h"
#include "PublicTypes.h"

class CNetManagerMakket : public new_singletonbase<CNetManagerMakket>,
						  public SafeThreadBase<CNetManagerMakket,4>
{
	IMPLEMENT_NEW_SINGLETON_MODE(CNetManagerMakket)
public:
	virtual ~CNetManagerMakket(void);
private:
	CSafeQueue<Market*> m_cPacketList;
	CSSLSocket	m_cSSLSocket;
	BOOL		m_bRunning;
private:
	void	InitInstance();
	UINT	HandleSocketSendThread();
	UINT	HandleSocketRecvThread();
	UINT	HandleAllSocketBeatThread();
	void	ClearAllPacket();
	void	CheckSocketConnect();
	void	SendHeartPacket();
	void	CheckMarketState();
	int	ReadOnePacket(SOCKET sock);
public:
	BOOL	ConnectToTS(const char *strIp,int iPort,int iTotal);
	void	Start();
	void	Stop();
	void	PushOnePacket(Market *pOnePacket);
	void	PushOnePacket(int iCmd,string &strJson);
};
