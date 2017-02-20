#include "DCManager.h"
#include "autolock.h"
#include "mem_macro.h"
#include <iostream> 
#include "net.h" 
#include <sys/time.h>


#include<json/json.h>
#include "RMQManager.h"

#include "HttpRequestProcess.h"
#include "logwriter.h"
#include "../tinyxml/tinyxml.h"
#include "string_splitter.h"
using namespace std;
#ifndef _WIN32
#include <unistd.h> 
#endif

#define  ENABLE_LOG		1
#define  START_FLAG		(0x7F)
log_writer local_log;

void CDCManager::InitInstance()
{  
	m_bRunning = FALSE;
	m_pFrontServer = new epollserver;
	this->ReadXML();
	m_pEpollServerCallback = new net_event_callback;
	memset(m_pEpollServerCallback, 0, sizeof(net_event_callback));
	m_pEpollServerCallback->on_data_recv = _http_authorize_on_data_recv;
	m_pEpollServerCallback->on_data_can_read = OnDataRead;
}

CDCManager::~CDCManager(void)
{  
	DELETE_PTR(m_pFrontServer);
	DELETE_PTR(m_pEpollServerCallback);

	join_all_thread();
}

void CDCManager::ReadXML()
{
#if 0
	char szTmp[256]; 
	int nVal = 0;
	const char * config_xml_filename = "./etc/dcc.xml";
	TiXmlDocument cfgDoc(config_xml_filename);

	if (!cfgDoc.LoadFile())
	{
		ERROR_EXIT("load config file[%s] failed", config_xml_filename);
	}

	TiXmlElement* pNode = cfgDoc.RootElement()->FirstChildElement("authorHttp");

	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNode, "ip", szTmp);
	m_strAuthorIP = szTmp;
	XML_GET_SUBNODE_INT_ATTRIBUTE(pNode, "port", nVal);
	m_nAuthorPort = nVal;
	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNode, "url", szTmp);
	m_strAuthorURL = szTmp;
	
	pNode = cfgDoc.RootElement()->FirstChildElement("loginInfo");
	TiXmlElement* pNodeSub = pNode->FirstChildElement("HQ");
	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNodeSub, "IP", szTmp);
	m_tServerHQ.strIP = szTmp;
	XML_GET_SUBNODE_INT_ATTRIBUTE(pNodeSub, "port", nVal);
	m_tServerHQ.nPort = nVal;

	pNodeSub = pNode->FirstChildElement("DownLoad");
	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNodeSub, "IP", szTmp);
	m_tServerDownload.strIP = szTmp;
	XML_GET_SUBNODE_INT_ATTRIBUTE(pNodeSub, "port", nVal);
	m_tServerDownload.nPort = nVal;




	pNodeSub = pNode->FirstChildElement("upload");
	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNodeSub, "IP", szTmp);
	m_tServerUpload.strIP = szTmp;
	XML_GET_SUBNODE_INT_ATTRIBUTE(pNodeSub, "port", nVal);
	m_tServerUpload.nPort = nVal;
	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNodeSub, "user", szTmp);
	m_tServerUpload.strUser = szTmp;
	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNodeSub, "pwd", szTmp);
	m_tServerUpload.strPWD = szTmp;
	

	pNodeSub = pNode->FirstChildElement("trade_ip");
	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNodeSub, "IP", szTmp);
	m_tServerTrade.strIP = szTmp;
	XML_GET_SUBNODE_INT_ATTRIBUTE(pNodeSub, "port", nVal);
	m_tServerTrade.nPort = nVal;
	XML_GET_SUBNODE_TEXT_ATTRIBUTE(pNodeSub, "key", szTmp);
	m_tServerTrade.strKey = szTmp;
#endif

}

BOOL CDCManager::ReadyConnect()
{
	BOOL bRet = this->InitRMQ();
	if(FALSE == bRet)
	{
		return FALSE;
	}
	bRet = this->ListenFS();

	
	return bRet;
}

void CDCManager::Start()
{
	if(m_bRunning)
	{
		return;
	}
	m_bRunning = TRUE; 
	int nThreadIndex = 0;
	for(nThreadIndex = 0; nThreadIndex < 2; nThreadIndex++)
	{
		safe_start_thread(&CDCManager::HanldeAllPacketThread, (void *)nThreadIndex, nThreadIndex);
	}
	safe_start_thread(&CDCManager::HanlleHeartBeatThread, NULL, nThreadIndex++);
	safe_start_thread(&CDCManager::HanlleAllSendThread, NULL, nThreadIndex++);
	safe_start_thread(&CDCManager::HandleLogoutThread, NULL, nThreadIndex++);
	
 
}

void CDCManager::Stop()
{
	if(FALSE == m_bRunning)
	{
		return;
	}
	m_bRunning = FALSE; 

	CRMQManager::GetInstance()->StarConsumer();
	this->ClearAllRecvDataPacket();
	this->ClearAllSendPacket();
	
}

void CDCManager::SendOnePacket(DWORD dwCmdID, BYTE* pBuffer, int nBufferLen, DWORD dwAppendID)
{
	int nTotalLen = nBufferLen + sizeof(tagPacketHead);
	char * pszBuffer = new char[nTotalLen];
	tagPacket * pOnePacket = new tagPacket;
	pOnePacket->tPackeHead.dwCmd = dwCmdID; 
	pOnePacket->tPackeHead.dwAppendData = dwAppendID;
	pOnePacket->tPackeHead.dwTotalLen = nTotalLen;
	if(nBufferLen > 0)
	{ 
		memcpy(&pOnePacket->szBody, pBuffer, nBufferLen);
	}
	this->SendOnePacket(pOnePacket);
}

static DWORD dwSeqIDAllocator = 1000;
void CDCManager::SendOnePacket(tagPacket * pOnePacket)
{
	this->PushPacketIntoQueque(pOnePacket, m_lstSendPacket, true );
}

void CDCManager::PushPacketIntoQueque(tagPacket * pOnePacket, ListDataPacket& lstPacket, bool bAddTag)
{
	if(NULL == pOnePacket)
	{
		return;
	} 
	if(bAddTag)
	{
#if _WIN32
		CTime cTm = CTime::GetCurrentTime();
		DWORD dwTm = cTm.GetTime();
#else
		//DWORD dwTm = time(0);
		DWORD dwTm = GetTickCount();
#endif

		pOnePacket->tPackeHead.dwTimeStamp = dwTm;
		if(0 == pOnePacket->tPackeHead.dwSeqID)
		{
			pOnePacket->tPackeHead.dwSeqID = dwSeqIDAllocator ++;
		}
	}
	lstPacket.push(pOnePacket);
	WRITE_FORMAT_LOG("push packet[%d] from[%d %d]", pOnePacket->tPackeHead.dwSeqID, pOnePacket->tPackeHead.dwFromSocket, pOnePacket->tPackeHead.dwTotalLen);
}

void CDCManager::PushOnePacket(tagPacket * pOnePacket)
{
	this->PushPacketIntoQueque(pOnePacket, m_lstRecvPacket, true ); 
}

 

BOOL CDCManager::InitRMQ()
{ 
	CRMQManager::GetInstance()->InitRMQ();
	CRMQManager::GetInstance()->StarConsumer();
	return TRUE;
}

BOOL CDCManager::ConnectCC()
{
	return TRUE;  
}


BOOL CDCManager::ListenFS()
{ 
	BOOL bRet = m_pFrontServer->listen_at("localhost", 4097, m_pEpollServerCallback, em_authorWeb);
	return bRet;
}


void CDCManager::ClearAllRecvDataPacket()
{
	while(FALSE == m_lstRecvPacket.IsEmpty())
	{
		tagPacket * pOnePacket = m_lstRecvPacket.pop();
		delete pOnePacket;
	}
}

void CDCManager::ClearAllSendPacket()
{
	while(FALSE == m_lstSendPacket.IsEmpty())
	{
		tagPacket * pOnePacket = m_lstSendPacket.pop(); 
		delete pOnePacket;
	}
} 
 

void CDCManager::HanldeAllPacketThread(void* param)
{
	printf("handle all thread is running.....\n"); 
	int  nThreaIndex = long(param);
	WRITE_FORMAT_LOG("HanldeAllPacketThread[%d] start ", nThreaIndex);
	static int nHandlePacket = 0;
	int nOneThreadPacket = 0;
	while(m_bRunning)
	{  
		tagPacket * pOnePacket = NULL;
		bool bRet = m_lstRecvPacket.popEx(pOnePacket); 
		if(false == bRet || NULL == pOnePacket)
		{
			{
#ifdef _WIN32
				Sleep(20);
#else
				usleep(5000);
#endif 

			}
			continue;
		}
		//handle one packet
		switch(pOnePacket->tPackeHead.dwCmd)
		{
		case WX_CMD_AUTHOR_REQ:
			OnMessageHttpAuthorReq(pOnePacket);
			break;
		case WX_CMD_LOGIN_REQ:
			OnMessageLoginReq(pOnePacket);
			break;
		case WX_CMD_HEART:
			SendHeartBeatToClient(pOnePacket);
			break;
		default:
			break;
		}
		nHandlePacket ++;
		nOneThreadPacket ++;
		WRITE_FORMAT_LOG("HanldeAllPacketThread[%d]packet[%d len:%d] handle:%d total:%d remain:%d  ", 
			nThreaIndex, pOnePacket->tPackeHead.dwSeqID, pOnePacket->tPackeHead.dwTotalLen, nOneThreadPacket, nHandlePacket,  m_lstRecvPacket.size());
		delete pOnePacket;
	}
}


void CDCManager::RemoveDeadSocket()
{
	std::vector<int > vecDeadSocket;
	int nTimeout = 10 * 1000;
	m_pFrontServer->GetTimeoutSocket(nTimeout, vecDeadSocket);
	for(int ii = 0; ii < vecDeadSocket.size(); ii ++)
	{
		m_pFrontServer->CloseSocketByID(vecDeadSocket[ii], 4);
	}
}

void CDCManager::HanlleHeartBeatThread(void* param)
{ 
	while(m_bRunning)
	{
#ifdef _WIN32
		Sleep(1000);
#else
		usleep(1000 * 1000);
#endif 
	}
}

void CDCManager::SendHeartBeatToClient(tagPacket *pOnePacket)
{ 
	int nPacketLen = sizeof(tagPacketHead) + 1;
	char *pszBuffer = new char[nPacketLen];
	ZeroMemory(pszBuffer, nPacketLen);
	tagPacket * pOnePacketCopy = (tagPacket *)pszBuffer;
	tagPacketHead & tPacketHead = pOnePacketCopy->tPackeHead;
	tPacketHead.dwTotalLen = nPacketLen;
	tPacketHead.byStartFlag = START_FLAG;
	tPacketHead.dwFromSocket = pOnePacket->tPackeHead.dwFromSocket;
	tPacketHead.dwCmd = pOnePacket->tPackeHead.dwCmd;
	tPacketHead.dwSeqID = pOnePacket->tPackeHead.dwSeqID;
	//memcpy(&pOnePacket->szBody, strJson.c_str(), strJson.length());
	this->PushPacketIntoQueque(pOnePacketCopy, m_lstSendPacket, true );
}


void CDCManager::HanlleAllSendThread(void* param)
{ 

	
	const int MAX_BUFFER_LEN = 1024 * 1024;
	static char bySendBuffer[MAX_BUFFER_LEN]; 
	tagPacket * pOnePacket = NULL;
	while(m_bRunning)
	{  
			tagPacket * pOnePacket = m_lstSendPacket.pop();
			if(NULL == pOnePacket)
			{ 
#ifdef _WIN32
					Sleep(20);
#else
					usleep(5 * 1000); 
#endif 
				continue;
			}
			//handle one packet
			//send by socket
			tagPacketHead& tOnePacketHead = pOnePacket->tPackeHead;
			
#if ENABLE_LOG
			char szLog[512];
			sprintf(szLog, "[socket send] ID:%d SEQ:%d, Body Len:%d ToSocket:%d\r\n", 
				pOnePacket->tPackeHead.dwCmd, pOnePacket->tPackeHead.dwSeqID,
				pOnePacket->tPackeHead.dwTotalLen, pOnePacket->tPackeHead.dwFromSocket);
			//::TraceDebugInfo(szLog);
			WRITE_LOG(szLog);
#endif
			int nHeadLen = sizeof(tOnePacketHead);
			int nBodyLen = pOnePacket->tPackeHead.dwTotalLen; 
			tOnePacketHead.dwTotalLen = ::htonl(tOnePacketHead.dwTotalLen);
			tOnePacketHead.wTotalPacket = ::htons(tOnePacketHead.wTotalPacket);
			tOnePacketHead.wPacketIndex = ::htons(tOnePacketHead.wPacketIndex);
			tOnePacketHead.dwSeqID = ::htonl(tOnePacketHead.dwSeqID);
			tOnePacketHead.dwCmd = ::htonl(tOnePacketHead.dwCmd);
			tOnePacketHead.dwTimeStamp = ::htonl(tOnePacketHead.dwTimeStamp);
			tOnePacketHead.dwAppendData = ::htonl(tOnePacketHead.dwAppendData);
			bool bRet = false;
			bRet = ::socket_sendbuffer(pOnePacket->tPackeHead.dwFromSocket, &tOnePacketHead, nBodyLen);
			if(false == bRet)
			{

			}
		printf("send result=[%d]\n",bRet);

		if(tOnePacketHead.dwTotalLen > 0 )
		{
			delete pOnePacket; 
		}
		
       gettimeofday( &end, NULL );
	   double timeuse =  (1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec)/1000.0;
	   printf("timeUser=%.f\n",timeuse);
	}
}


//////////////////////////////////////////////////////////////////////////

void CDCManager::HandleLogoutThread(void *param)
{
	printf("logout thread is running \n");
	while( m_bRunning )
	{

		int nSocket = 0;
		nSocket = epollserver::m_qLogout.pop();
		if ( 0 == nSocket)
		{ 
#ifdef _WIN32
			Sleep(20);
#else
			usleep(5 * 1000); 
#endif 
			continue;
		}
		
		CRMQManager::GetInstance()->PublishLogout(nSocket);

	}
}
//////////////////////////////////////////////////////////////////////////
void CDCManager::OnMessageHttpAuthorReq(tagPacket * pOnePacket)
{
#if 0
	if(NULL == pOnePacket || 0 == pOnePacket->szBody)
	{
		return;
	}
	int fd = pOnePacket->tPackeHead.dwFromSocket;
	int nSocketID = pOnePacket->tPackeHead.dwAppendData;
	WRITE_FORMAT_LOG("packet [id:%d [%d %d] len:%d begin", pOnePacket->tPackeHead.dwSeqID, fd, nSocketID, pOnePacket->tPackeHead.dwTotalLen);
	//WRITE_LOG(pOnePacket->pszBody);
	DWORD dwTmBegin = ::GetTickCount();
	HttpRequestProcess cHttpProcesser;
	int nBodyLen = pOnePacket->tPackeHead.dwTotalLen - sizeof(tagPacketHead);
	cHttpProcesser.process(&pOnePacket->szBody, nBodyLen -1);
	string strURL = cHttpProcesser.getURL();
	string strArgs = cHttpProcesser.getArgs();
	if(strArgs.empty() || strURL.empty())
	{
		WRITE_FORMAT_LOG("socket fd [%d %d %d]recv invalid packet  ", fd, nSocketID, pOnePacket->tPackeHead.dwSeqID);
		return;
	}
	WRITE_FORMAT_LOG("packet id:[%d %d %d] URL:%s OK", pOnePacket->tPackeHead.dwSeqID, fd, nSocketID, strURL.c_str());
	const int MAX_RESPONSEBUFFER_SIZE = 8 * 1024;
	CBinaryBuffer cResponseBuffer(MAX_RESPONSEBUFFER_SIZE, 1024);
	if(strURL.find("postman/api_detail.html") != -1)
	{
		GetLoginResponse(strURL, strArgs,cResponseBuffer);
	}
	else if(strURL.find("postman/api_permissions.html") != -1)
	{
		GetUserTokenResponse(strURL, strArgs, cResponseBuffer);
	} 
	fd = m_pFrontServer->GetSocketById(nSocketID);
	bool bRet = ::socket_sendbuffer(fd, cResponseBuffer.GetData(),cResponseBuffer.GetLength());
	//if(false == bRet)
	{
		m_pFrontServer->CloseSocketByID(nSocketID, 0);
	}
	DWORD dwTmEnd = ::GetTickCount(); 
	WRITE_FORMAT_LOG("OnMessageHttpAuthorReq [%s %d] waste time:%d, all handle time:%d len:%d result:%d", strURL.c_str(),pOnePacket->tPackeHead.dwSeqID,
					(dwTmEnd - dwTmBegin), (dwTmEnd - pOnePacket->tPackeHead.dwTimeStamp),cResponseBuffer.GetLength(), bRet);
#endif
}


void CDCManager::GetLoginResponse(string & strURL, string& strArgs, CBinaryBuffer& cSendBuffer)
{	
#if 0
	CURLArgsSplitter cURLSplitter;
	cURLSplitter.parser(strArgs);
	//string strUserName = cURLSplitter.getArgValByKey(string("token"));
	char szTemp[256];
	sprintf(szTemp, "%s;%d;%s;%s;", m_tServerUpload.strIP.c_str(), m_tServerUpload.nPort,
		m_tServerUpload.strUser.c_str(), m_tServerUpload.strPWD.c_str());

	Json::Value root;  
	root["HQIP"] = Json::Value(m_tServerHQ.strIP.c_str()); 
	root["HQPort"] = Json::Value(m_tServerHQ.nPort);  
	root["DownLoadIP"] = Json::Value(m_tServerDownload.strIP.c_str()); 
	root["DownLoadPort"] = Json::Value(m_tServerDownload.nPort); 
	root["uploadinfo"] = Json::Value(szTemp);

	root["trade_ip"] = Json::Value(m_tServerTrade.strIP.c_str()); 
	root["trade_port"] = Json::Value(m_tServerTrade.nPort);
	root["trade_key"] = Json::Value(m_tServerTrade.strKey.c_str()); 
	Json::FastWriter fast_writer;
	string strJson = fast_writer.write(root); 

	string strResponse;
	sprintf(szTemp,"Content-Length: %d\r\n", strJson.length());
	strResponse.append("HTTP/1.1 200 OK\r\n");
	strResponse.append(szTemp);
	strResponse.append("Content-Type: application/json;charset=utf-8\r\n");
	strResponse.append("Connection: close\r\n\r\n");
	//strResponse.append("\r\n\r\n");
	strResponse.append(strJson);
	cSendBuffer.Write(strResponse);
#endif

}

void CDCManager::GetUserTokenResponse(string & strURL, string& strArgs, CBinaryBuffer& cSendBuffer)
{
	CURLArgsSplitter cURLSplitter;
	cURLSplitter.parser(strArgs);
	string strKey= "token";
	string strUserName = cURLSplitter.getArgValByKey(strKey);
	 
	string strURLHead = m_strAuthorURL;
	string strFullURL = strURLHead.append("?").append(strArgs);

	DWORD dwTm1 = ::GetTickCount(); 
#if 0
#if 1

	MWebClient cWebClient; 
	cWebClient.setServerInfo(m_strAuthorIP, m_nAuthorPort);
	cWebClient.doGet(strFullURL.c_str(), cSendBuffer);
	
#else
	MWebClientEx::GetInstance()->intdoGet(strFullURL, strResponse);
#endif
#endif
	DWORD dwTm2 = ::GetTickCount();
	//WRITE_FORMAT_LOG("url:%s response:%s  webclient waste time:%d ",strFullURL.c_str(), cSendBuffer.GetData(), (dwTm2 - dwTm1));
 

}

void CDCManager::Test_Publish()
{
	Json::Value root;
	root["cmd"] = Json::Value(123);
	root["seq"] = Json::Value(456);
	root["from"] = Json::Value(789);
	root["usr"] = Json::Value("user[aa]");
	root["pwd"] = Json::Value("pwd[bb]");
	Json::FastWriter fast_writer;
	string strJson = fast_writer.write(root);
	CRMQManager::GetInstance()->Publish(strJson, true);
}


void CDCManager::OnMessageLoginReq(tagPacket * pOnePacket)
{
	 gettimeofday( &start, NULL );
	Json::Reader jsReader;
	Json::Value  jsAns;	
	if (!jsReader.parse(&pOnePacket->szBody, jsAns))
	{
		return ;
	}

	string strUser =  jsAns["usr"].asString().c_str();
	string strPWD =  jsAns["pwd"].asString().c_str();
	Json::Value root;  
	root["cmd"] = Json::Value( WX_CMD_LOGIN_REQ); 
	root["seq"] = Json::Value( pOnePacket->tPackeHead.dwSeqID); 
	root["from"] = Json::Value(pOnePacket->tPackeHead.dwFromSocket);
	root["usr"] = Json::Value(strUser.c_str()); 
	root["pwd"] = Json::Value(strPWD.c_str());   
	Json::FastWriter fast_writer;
	string strJson = fast_writer.write(root); 
	CRMQManager::GetInstance()->Publish(strJson, true);

}

void CDCManager::OnLoginRespons(string& strJson)
{
	Json::Reader jsReader;
	Json::Value  jsAns;	
	if (!jsReader.parse(strJson, jsAns))
	{
		return ;
	}
	string strUser =  jsAns["usr"].asString().c_str();
	int nCmd =  jsAns["cmd"].asInt();
	int nSeq  =  jsAns["seq"].asInt();
	int fd = jsAns["from"].asInt(); 
	int nPacketLen = sizeof(tagPacketHead) + strJson.length() + 1;
	char *pszBuffer = new char[nPacketLen];
	ZeroMemory(pszBuffer, nPacketLen);
	tagPacket * pOnePacket = (tagPacket *)pszBuffer;
	tagPacketHead & tPacketHead = pOnePacket->tPackeHead;
	tPacketHead.dwTotalLen = nPacketLen;
	tPacketHead.byStartFlag = START_FLAG;
	tPacketHead.dwFromSocket = fd;
	tPacketHead.dwCmd = nCmd;
	tPacketHead.dwSeqID = nSeq;
	memcpy(&pOnePacket->szBody, strJson.c_str(), strJson.length());
	this->SendOnePacket(pOnePacket);
}
//==============各个业务实现================================================== 
int _http_authorize_on_data_recv(int s, char* data, size_t len, DWORD pAppendData)
{
	int nTotalLen = len + sizeof(tagPacketHead) + 1;
	char * szBuffer = new char[nTotalLen];
	memcpy(szBuffer, data, len);
	tagPacket * pOnePacket = (tagPacket *)szBuffer;
	int nBodyLen = len + 1;
	pOnePacket->tPackeHead.dwTotalLen = nTotalLen;
	pOnePacket->tPackeHead.dwFromSocket = s;
	pOnePacket->tPackeHead.dwAppendData = pAppendData;
	pOnePacket->tPackeHead.dwCmd = WX_CMD_AUTHOR_REQ; 
	CDCManager::GetInstance()->PushOnePacket(pOnePacket);
	return true;
}




int OnDataRead(int s)
{
	int nPacketLen = 0;
	bool bRet = ::socket_readn(s, &nPacketLen, sizeof(int));
	if(false == bRet)
	{
		return ES_CLOSE_SOCK;
	}
	nPacketLen = ::ntohl(nPacketLen);
	char * pszBuffer = new char[nPacketLen];
	ZeroMemory(pszBuffer, nPacketLen);
	bRet = ::socket_readn(s, pszBuffer + sizeof(int), nPacketLen - sizeof(int) );
	if(false == bRet)
	{
		delete [] pszBuffer;
		return ES_CLOSE_SOCK;
	}
	tagPacket * pOnePacket = (tagPacket*)pszBuffer;
	tagPacketHead& tPacketHead = pOnePacket->tPackeHead;
	tPacketHead.dwTotalLen = nPacketLen;
	tPacketHead.wTotalPacket = ::ntohs(tPacketHead.wTotalPacket);
	tPacketHead.wPacketIndex = ::ntohs(tPacketHead.wPacketIndex);
	tPacketHead.dwSeqID = ::ntohl(tPacketHead.dwSeqID);
	tPacketHead.dwCmd = ::ntohl(tPacketHead.dwCmd);
	tPacketHead.dwTimeStamp = ::ntohl(tPacketHead.dwTimeStamp);
	tPacketHead.dwAppendData = ::ntohl(tPacketHead.dwAppendData);
	tPacketHead.dwFromSocket = s;
	if(tPacketHead.byStartFlag != START_FLAG)
	{
		delete [] pszBuffer;
		return ES_CLOSE_SOCK;
	}
	CDCManager::GetInstance()->PushOnePacket(pOnePacket);
	return ES_OK;
}
