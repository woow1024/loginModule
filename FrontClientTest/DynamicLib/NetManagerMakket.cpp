#include "stdafx.h"
#include "NetManagerMakket.h"
#include "ManagerMarket.h"

static int g_dwSeqAllocator = 1;
static DWORD g_time;
CNetManagerMakket::~CNetManagerMakket(void)
{
	ClearAllPacket();
}

void CNetManagerMakket::InitInstance()
{
	m_bRunning = FALSE;
}


UINT CNetManagerMakket::HandleSocketSendThread()
{
	while(m_bRunning)
	{
		if(FALSE == m_cSSLSocket.IsConnected())
		{//断线
			Sleep(1000);
			continue;
		}
		Market * pOnePacket = m_cPacketList.pop();
		if(NULL == pOnePacket)
		{
			Sleep(10);
			continue;
		}
		int nHeadLen = sizeof(pOnePacket->tPackeHead); 
		//int nBodyLen = pOnePacket->tPackeHead.wLen - nHeadLen;
		int nBodyLen = pOnePacket->tPackeHead.dwTotalLen - nHeadLen;
		::N2HPacket(pOnePacket, FALSE);
		int nSendByte = 0;
		int nTotalLen = nHeadLen + nBodyLen;
#if 0
		nSendByte = m_cSSLSocket.Send((char *)&pOnePacket->tPackeHead, nHeadLen);
		nSendByte = m_cSSLSocket.Send(pOnePacket->pszBody, nBodyLen);
#else
		char * pszSendBuffer = new char[nTotalLen];
		memcpy(pszSendBuffer, &pOnePacket->tPackeHead, nHeadLen);
		memcpy(pszSendBuffer + nHeadLen, pOnePacket->pszBody, nBodyLen);
		g_time = ::GetTickCount();
		nSendByte = m_cSSLSocket.Send(pszSendBuffer, nTotalLen);
		if(nSendByte >= 0)
		{
#if _DEBUG
			TRACE("Send->socket:%d,body:%s\n",m_cSSLSocket.GetSocket(),pszSendBuffer+52);
#else
			CManagerMarket::GetInstance()->FormatLog(0,"Send->socket:%d,body:%s\n",m_cSSLSocket.GetSocket(),pszSendBuffer+52);
#endif
			
			CManagerMarket::GetInstance()->PostTSMessage(htonl(pOnePacket->tPackeHead.dwCmd),0,(LPARAM)(LPCTSTR)(pszSendBuffer+52),FALSE);
		}
		G_FREE_ARRAY_MEM(pszSendBuffer);
#endif

	}
	return 1;
}

UINT CNetManagerMakket::HandleSocketRecvThread()
{
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 10000;
	while(m_bRunning)
	{
		if(FALSE == m_cSSLSocket.IsConnected())
		{//断线
			Sleep(1000);
			continue;
		}
		ReadOnePacket();
// 		SOCKET hSocket = m_cSSLSocket.GetSocket();
// 		fd_set fdread;
// 		fd_set fdwrite;
// 		FD_ZERO(&fdread);
// 		FD_ZERO(&fdwrite);
// 		FD_SET(hSocket, &fdread);
// 		FD_SET(hSocket, &fdwrite);
// 
// 		int ret = select(0, &fdread, NULL, NULL, NULL/*&timeout*/);
// 		if(ret < 0)
// 		{
// 			m_cSSLSocket.CloseSocket();
// 		}
// 		if (FD_ISSET(hSocket, &fdread)) // fd可读了
// 		{ 
// 			ReadOnePacket();
// 		}

	}
	return 1;
}

UINT CNetManagerMakket::HandleAllSocketBeatThread()
{
	int nCount = 0;
	while(m_bRunning)
	{
		++ nCount;
		if(0 == nCount % 30 )
		{//30秒钟发送一次心跳
			SendHeartPacket();
		}
		CheckSocketConnect();
		if(0 == nCount % 300)
		{
			//();
		}
		Sleep(1000);
	}
	return 1;
}

void CNetManagerMakket::ClearAllPacket()
{
	while(m_cPacketList.IsEmpty() == FALSE)
	{
		Market *pOnePacket = m_cPacketList.pop();
		if (pOnePacket == NULL)
		{
			break;
		}
		G_FREE_ONE_MEM(pOnePacket);
	}
}

void CNetManagerMakket::CheckSocketConnect()
{

}

void CNetManagerMakket::SendHeartPacket()
{

}

void CNetManagerMakket::CheckMarketState()
{
	if(FALSE == m_cSSLSocket.IsConnected())
	{
		return;
	}
	//this->PushOnePacket(GWTS_CMD_MARKET_STATE, string(""));	
}

void CNetManagerMakket::ReadOnePacket()
{
	Market * pOnePacket = new Market;
	BOOL bOK = TRUE;
	int nHeadLen = sizeof(tagTTPacketMarketHead);
	SOCKET hSocket = m_cSSLSocket.GetSocket();
	int nReadLen = m_cSSLSocket.Read((char *)&pOnePacket->tPackeHead, nHeadLen,0);
	if(nReadLen <= 0)
	{
		bOK = FALSE;
	}
	if( bOK )
	{
		//int nPacketLen = ::ntohs(pOnePacket->tPackeHead.wLen);
		int nPacketLen = ::ntohl(pOnePacket->tPackeHead.dwTotalLen);
		int nBodyLen = nPacketLen - nHeadLen;
		if(nBodyLen > 0)
		{
			pOnePacket->pszBody = new char[nBodyLen];
			nReadLen = m_cSSLSocket.Read(pOnePacket->pszBody, nBodyLen,1);
			string strJson = ::UTF8ToGB(pOnePacket->pszBody); 
			strcpy(pOnePacket->pszBody,strJson.c_str());
			if(nReadLen < 0)
			{
				bOK = FALSE;
			}
			else
			{
				DWORD endtime = ::GetTickCount();
				CManagerMarket::GetInstance()->PostTSMessage(10000,(WPARAM)(endtime-g_time),NULL,FALSE);
#if _DEBUG
				TRACE("Recv->socket:%d,body:%s\n",m_cSSLSocket.GetSocket(),pOnePacket->pszBody);
#else
				CManagerMarket::GetInstance()->FormatLog(0,"Recv->socket:%d,body:%s\n",m_cSSLSocket.GetSocket(),pOnePacket->pszBody);
#endif
			}
		}
	}
	if(FALSE == bOK)
	{
		G_FREE_ONE_MEM(pOnePacket);
	}
	else
	{
		::N2HPacket(pOnePacket);
		CManagerMarket::GetInstance()->PushOnePacket(pOnePacket);
	}
}

BOOL CNetManagerMakket::ConnectToTS(const char *strIp,int iPort)
{
	return m_cSSLSocket.ConnectServer(strIp,iPort);
}

void CNetManagerMakket::Start()
{
	if (m_bRunning)
	{
		return;
	}
	else
	{
		SafeStartThread(&CNetManagerMakket::HandleSocketSendThread,0);
		SafeStartThread(&CNetManagerMakket::HandleSocketRecvThread,1);
		SafeStartThread(&CNetManagerMakket::HandleAllSocketBeatThread,2);
		m_bRunning = TRUE;
	}
}

void CNetManagerMakket::Stop()
{
	if (m_bRunning == FALSE)
	{
		return;
	}
	this->ClearAllPacket();
	m_bRunning = FALSE;

}

void CNetManagerMakket::PushOnePacket(Market *pOnePacket)
{
	if(NULL == pOnePacket)
	{
		return;
	}
	m_cPacketList.push(pOnePacket);
}

void CNetManagerMakket::PushOnePacket(int iCmd,string &strJson)
{
// 	string strJsonTemp = strJson;
// 	if(strJson.empty())
// 	{
// 		strJsonTemp = "{}";
// 	}
// 	string strUTF8Json = ::GBToUTF8(strJsonTemp.c_str());
// 	Market * pOnePacket = new Market;
// 	tagTTPacketMarketHead & tPacketHead = pOnePacket->tPackeHead;
// 	tPacketHead.wSeq = g_dwSeqAllocator ++;
// 	tPacketHead.bStartFlag = 0xFF;
// 	int nBodyLen = strUTF8Json.length();
// 	tPacketHead.wLen = sizeof(tPacketHead) + nBodyLen;
// 	tPacketHead.wCmd = iCmd;
// 	if(FALSE == strJson.empty())
// 	{
// 		pOnePacket->pszBody = new char[nBodyLen];
// 		memcpy(pOnePacket->pszBody,  strUTF8Json.c_str(), nBodyLen);
// 		//pOnePacket->pszBody[nBodyLen -1] = '\0';
// 	}
// 	this->PushOnePacket(pOnePacket);


	int nPacketLen = sizeof(tagTTPacketMarketHead) + strJson.length() + 1;
	int a = sizeof(tagTTPacketMarketHead);
	char *pszBuffer = new char[nPacketLen];
	ZeroMemory(pszBuffer, nPacketLen);
	Market * pOnePacket = (Market *)pszBuffer;
	tagTTPacketMarketHead & tPacketHead = pOnePacket->tPackeHead;
	tPacketHead.dwTotalLen = nPacketLen;
	tPacketHead.byStartFlag = 0x7F;
	tPacketHead.dwSeqID = g_dwSeqAllocator++;
	tPacketHead.dwCmd = iCmd;
	pOnePacket->pszBody = new char[strJson.length()];
	memcpy(pOnePacket->pszBody, strJson.c_str(), strJson.length());
	
	this->PushOnePacket(pOnePacket);

#ifdef _DEBUG
	//WriteDebugMessage("发送 cmd:%d seq:%d json:%s", pOnePacket->tPackeHead.wCmd, pOnePacket->tPackeHead.wSeq, strUTF8Json.c_str());	
#endif 
}
