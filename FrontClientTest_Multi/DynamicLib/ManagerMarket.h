#pragma once
#include "stdafx.h"
#include "safequeue.h"
#include "singletonbase.h"
#include "SafeThread.h"
#include "json/json.h"

#define GWTRADELIB_API _declspec(dllexport)

class GWTRADELIB_API CManagerMarket :	public new_singletonbase<CManagerMarket>,
										public SafeThreadBase<CManagerMarket,4>
{
	IMPLEMENT_NEW_SINGLETON_MODE(CManagerMarket)
public:
	//CManagerMarket(void);
	virtual ~CManagerMarket(void);
private:
	BOOL m_bRunning;

	struct tagServerInfo
	{
		CString strIP;
		int nPort;
	};
	tagServerInfo m_tTSServer;
	CSafeQueue<Market*> m_cPacketList;
	CUIRacketCtrl * m_pPacketCtrl;	//用户请求处理器
	CMultPacketCtrl * m_pMultPacktCtrl; //分包处理器
	
	HWND	m_hNotifyWnd;
	HWND	m_hDispatchWnd;
	FILE*	m_pLog;
private:
	void InitInstance();
 	UINT HandleAllRecvPacketThread();
 	UINT HandleAllRequestThread();
 	void ClearAllPacket();
public:
	BOOL ConnetToTS(const char* strIP, int nPort,int iTotal);
	void Start();
	void Stop();
	BOOL Login(const char* strName,const char* strPwd);
public:
	void SetNotifyWnd(HWND hNotifyWnd);
	void SetDispatchWnd(HWND hDispatchWnd);
	void PushOnePacket(Market * pOnePacket);
	BOOL ParserJsonResponse(string& strJson, DWORD dwCmd);
	BOOL ParserHead(tagRspMarketHead& tRspHead, Json::Value&  jsAns);
	BOOL PostTSMessage(DWORD dwMessage, WPARAM wPara = 0, LPARAM lPara = 0, BOOL bSend = TRUE);
	void FormatLog(int nLevel, const char* chDomain, char* chText, ...);
};
