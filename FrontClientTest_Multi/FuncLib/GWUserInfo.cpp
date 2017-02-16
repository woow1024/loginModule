#include "StdAfx.h"
#include "GWUserInfo.h"
#include "FuncLib.h"
#include <algorithm>
#include "json/json.h"
#include "GlobalSetting.h"
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

#include <json/json.h>
#include <BuildInfo.h>

GWUserInfo* GetUserInfo()
{
	return GWUserInfo::GetInstance();
}


CString GetClientType()
{
	GWUserInfo * pUser = GetUserInfo();
	if(pUser)
	{
		return pUser->GetClientType();
	}
	return _T("");
}

void GWUserInfo::InitInstance()
{
	m_nAccountLevel = REGISTEREDUSER;
	m_bOnLineState	= FALSE;
	m_dwTAPrivilege = 0;
	m_dwSLSPPrivilege = 0;
	m_nIMMessageCount = 0;
	m_nIMState = em_Line_Off;
#ifdef VERSION_TAG_TT
#ifdef _DEBUG
	m_nTmpLevel	= -1;
#else
	m_nTmpLevel	= 0;
#endif
#endif
}	



GWUserInfo::~GWUserInfo(void)
{
	m_mapAccredit.clear();
}

void GWUserInfo::CollectLocalHardwareInfo()
{
	// 分辨率
	m_cbHDInfo.resolution.cx = (USHORT)GetSystemMetrics(SM_CXSCREEN);
	m_cbHDInfo.resolution.cy = (USHORT)GetSystemMetrics(SM_CYSCREEN);

	// 机器名
	gethostname(m_cbHDInfo.hostname, sizeof(m_cbHDInfo.hostname));

	// 默认浏览器
	long dwSize = 1024;
	TCHAR szBuffer[1024] = { 0 };
  
	HRESULT hResult = RegQueryValue(HKEY_CLASSES_ROOT, _T("http\\shell\\open\\command"), szBuffer, &dwSize);

	if (hResult == ERROR_SUCCESS)
	{
		try
		{
			char szName[64];
			_splitpath(szBuffer, NULL, NULL, szName, NULL);
			sprintf_s(m_cbHDInfo.brower, sizeof(m_cbHDInfo.brower), "%s.exe", szName);
		}
		catch(...)
		{
			
		}
	}

	// 操作系统
	ZeroMemory(&m_cbHDInfo.osversion, sizeof(m_cbHDInfo.osversion));
	m_cbHDInfo.osversion.dwOSVersionInfoSize = sizeof(m_cbHDInfo.osversion);
	GetVersionEx(LPOSVERSIONINFOA(&m_cbHDInfo.osversion));

	// 系统
	ZeroMemory(&m_cbHDInfo.sysinfo, sizeof(m_cbHDInfo.sysinfo));
	GetSystemInfo(&m_cbHDInfo.sysinfo);

	// 适配器信息
	CollectAdapterInfo();
}

void GWUserInfo::CollectAdapterInfo()
{
	ULONG uSize = sizeof(IP_ADAPTER_INFO);

	PIP_ADAPTER_INFO pAdapterInfo = (PIP_ADAPTER_INFO)malloc(uSize);

	DWORD dwRet = GetAdaptersInfo(pAdapterInfo, &uSize);
	
	if (dwRet == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (PIP_ADAPTER_INFO)malloc(uSize);
	}
	
	dwRet = GetAdaptersInfo(pAdapterInfo, &uSize);

	if (dwRet != ERROR_SUCCESS)
	{
		ASSERT (FALSE);
		free(pAdapterInfo);
		return;
	}

	PIP_ADAPTER_INFO pNextAdapter = pAdapterInfo;

	while (pNextAdapter)
	{
		LocalAdapterInfo adapter;

		adapter.ip = pNextAdapter->IpAddressList.IpAddress.String;

		for (UINT n = 0; n < min(pNextAdapter->AddressLength, MAX_ADAPTER_ADDRESS_LENGTH); ++n)
		{
			CString strTemp;
			strTemp.Format(_T("%02X"), pNextAdapter->Address[n]);
		
			adapter.mac += strTemp;
		}

		m_cbHDInfo.adapters.push_back(adapter);
		pNextAdapter = pNextAdapter->Next;
	}
	
	free(pAdapterInfo);

	sort(m_cbHDInfo.adapters.begin(), m_cbHDInfo.adapters.end());
}

BOOL GWUserInfo::IsLocalMacAddr(const CString& strMac)
{
	CString strBuffer = strMac;
	strBuffer.MakeUpper();
	strBuffer.Replace("-", "");

	for (ADAPTERVECITR itr = m_cbHDInfo.adapters.begin(); itr != m_cbHDInfo.adapters.end(); ++itr)
	{
		if (itr->mac == strBuffer)
		{
			return TRUE;
		}
	}

	return FALSE;
}

CString GWUserInfo::GetLocalMachineID()
{
	CString strBuffer;

	for (ADAPTERVECITR itr = m_cbHDInfo.adapters.begin(); itr != m_cbHDInfo.adapters.end(); ++itr)
	{
		strBuffer += itr->mac;
	}

	strBuffer += m_cbHDInfo.hostname;
	
	if (strBuffer.IsEmpty())
	{
		WriteDebugMessage("无法生成有效的用户本地ID");
		return GW_Common::CreateGUID();
	}

	CString strID;

	GW_Encrypt::MD5Encrypt(strBuffer, strID, strBuffer.GetLength());

	return strID;
}

CString GWUserInfo::GetExtInfoString()
{
	Json::Value jsValue;
	
	jsValue["localid"] = (LPCTSTR)GetLocalMachineID();
	jsValue["client_type"] = VERSION_ACCOUNTTYPE;

	if (IsVisitor())
	{
		jsValue["visitor"] = 1;
	}
	
	return GW_Json::StringFromJsonValue(jsValue);
}

BOOL GWUserInfo::ParseTradeString(const string& strJson)
{
	m_vecSymbolRight.clear();
	try
	{
		Json::Value jsTrade;
		Json::Reader jsReader;

		if (!jsReader.parse(strJson, jsTrade))
		{
			WriteLogMessage(LogError, "无法解析模拟交易配置[%s]", strJson.c_str());
			return FALSE;
		}
		CString strNickName = jsTrade["nickname"].asString().c_str();
		if (IsVisitor() && strNickName.IsEmpty())
		{
			strNickName = _T("游客");
		}
		m_svrArray[Login_Server].SetNickName(strNickName);
		m_dwTAPrivilege = jsTrade[_T("s_type")].asInt();
		m_dwSLSPPrivilege = jsTrade[_T("tc")].asInt();

		AddAccredit(_T("tj"),jsTrade[_T("tj")].asInt());
		UINT ii = 0;
		Json::Value jsRs = jsTrade["right"];
		UINT nSize = jsRs.size();
		for(ii = 0; ii < nSize; ii ++)
		{//品种权限列表
			GWSymbolRight tOneItem;
			Json::Value jsObj = jsRs[ii];
 
			 CString strSymbol = jsObj[_T("sym")].asString().c_str();
			 if(strSymbol.IsEmpty())
			 {//如果symbol为空，则不丢弃
				 ASSERT(0);
				 continue;;
			 }
			 stringsplitter sp(_T("."));
			 sp.split(strSymbol);
			 if(sp.size() < 2)
			 {
				 ASSERT(0);
				 continue;
			 }
			 _tcsncpy(tOneItem.szMarket, (LPCTSTR)sp[0], sizeof(tOneItem.szMarket) - 1);
			_tcsncpy(tOneItem.szSymbol, (LPCTSTR)sp[1], sizeof(tOneItem.szSymbol) - 1);
			tOneItem.dwRight = jsObj[_T("slsp")].asInt(); 
			m_vecSymbolRight.push_back(tOneItem);
		}
		SetServerInfo(Trade_Server, jsTrade["trade_ip"].asString().c_str(), (USHORT)atol(jsTrade["trade_port"].asString().c_str()), jsTrade["trade_key"].asString().c_str());
		m_svrArray[Trade_Server].SetServerInfoEx(jsTrade["trade_uid"].asString().c_str(), _T(""));
		
		if (!IsVisitor())
		{
			//GWACCOUNTLEVEL nType = (GWACCOUNTLEVEL)jsTrade["user_type"].asInt();
			int nUType = jsTrade["user_type"].asInt();				// VIP以上的权限 暂时都转为VIP权限
#if 0
			if (nUType > VIPUSER)
			{
				nUType = VIPUSER;
			}
#endif

			GWACCOUNTLEVEL nType = (GWACCOUNTLEVEL)nUType;

			if (nType == VISITOR)		// 应该是没有传回来
			{
				nType = VIPUSER;

			}
			//m_nAccountLevel = nType;
			int nSubType = jsTrade["sublevel"].asInt();				// VIP以上的权限 暂时都转为VIP权限
			m_nAccountLevel =  (GWACCOUNTLEVEL)(nType * ACCOUNT_LEVEL_UNIT + nSubType);
		}
		SetServerInfo(Upload_Server, jsTrade["uploadinfo"].asString().c_str());
		
#ifndef _GWSTATION_RELEASE_VERSION_BUILDING
		WriteLogMessage(LogNormal, _T("文件上传服务器[%s:%d]"), GetServerInfo(Update_Server).GetServerIP(), GetServerInfo(Update_Server).GetPort());
#endif
		//获取备用服务器列表
		TCHAR szKey[32];
		ZeroMemory(szKey, sizeof(szKey));
		for( ii = 0; ii < MAX_SERVER - HQ_Server1 -1; ii ++)
		{
			_stprintf(szKey, _T("hq_ip%d"), ii +2);
			CString strBakServer = jsTrade[szKey].asString().c_str();
			if(strBakServer.IsEmpty())
			{
				break;
			}
			_stprintf(szKey, _T("hq_port%d"), ii +2);
			int nBakPort = jsTrade[szKey].asInt();
			_stprintf(szKey, _T("hq_key%d"), ii +2);
			CString strBakKey = jsTrade[szKey].asString().c_str();
			GWServerType emServerType = (GWServerType)(HQ_Server1 + ii);
			SetServerInfo(emServerType, (LPCTSTR)strBakServer, nBakPort, (LPCTSTR)strBakKey);
		}
		m_strDefaultVerID = jsTrade["right_auth"].asString().c_str();
		m_strSLSP = jsTrade["seat_no"].asString().c_str();
#ifdef VERSION_TAG_TT
		m_nTmpLevel	= jsTrade["level"].asInt();	
#endif

		m_bInnerStaff = jsTrade["employee"].asInt();

		if (m_bInnerStaff)
		{
			WriteDebugMessage("内部员工");
		}
		if(FALSE == jsTrade["client_type"].isNull())
		{
			m_strClientType =  jsTrade["client_type"].asCString();
		}
		return TRUE;
	}
	catch(std::runtime_error e)
	{
		WriteLogMessage(LogError, "解析模拟交易配置发生错误[%s]", e.what());
		return FALSE;
	}
	catch(...)
	{
		WriteLogMessage(LogError, "无法解析模拟交易配置");
		return FALSE;
	}
}

void GWUserInfo::SetServerInfo(GWServerType nType, LPCTSTR lpServerIP, USHORT uPort, LPCTSTR lpKey)
{
	ASSERT(nType < MAX_SERVER);

	if (nType >= MAX_SERVER)
	{
		return;
	}

	m_svrArray[nType].SetServerInfo(lpServerIP, uPort, lpKey);
}

void GWUserInfo::SetServerInfo(GWServerType nType, LPCTSTR lpServerInfo)
{
	stringsplitter sp(";");
	sp.split(lpServerInfo);

	ASSERT(sp.size() == 2 || sp.size() == 4);

	if (sp.size() != 2 && sp.size() != 4)
	{
		return;
	}

	SetServerInfo(nType, sp[0], (USHORT)atol(sp[1]));

	if (sp.size() == 4)
	{
		GetServerInfo(nType).SetServerInfoEx(sp[2], sp[3]);
	}
}


DWORD GWUserInfo::GetTAPrivilege()
{
#if 0
#ifdef _DEBUG
	return TRUE;
#endif
#endif
#if !(defined (VERSION_TAG_YG) || defined(VERSION_TAG_SY) || defined(VERSION_TAG_BSY))
	return m_dwTAPrivilege;
#else
	return FALSE;
#endif
}

BOOL GWUserInfo::IsHasIM()
{// TT  YG 版本具有IM权限
	return FALSE; //屏蔽 IM
#ifndef VERSION_TAG_YG 
	#ifndef VERSION_TAG_TT
		return FALSE;
	#endif
#endif
	if(m_nAccountLevel >= ACTIVEUSER && m_nAccountLevel < VIPUSER)
	{//激活用户才有 IM 权限
		return TRUE;
	}
	return FALSE;
}	

void GWUserInfo::SetIMMessageCount(int nMessageCount)
{
	m_nIMMessageCount = nMessageCount;
}

int  GWUserInfo::GetIMMessageCount()
{
	return m_nIMMessageCount;
}

void GWUserInfo::SetIMState(int nIMState)
{
	m_nIMState = nIMState;
}

int  GWUserInfo::GetIMState()
{
	return m_nIMState;
}


void GWUserInfo::AddOneAttionExpert(GWExpertInfo & tExpert)//增加一个关注列表
{	

	int nExpertNum = m_vectorExpert.size();
	GWEXPERTINFOVECITR itBegin = m_vectorExpert.begin();
	GWEXPERTINFOVECITR itEnd = m_vectorExpert.end();
	for(GWEXPERTINFOVECITR itr = itBegin; itr != itEnd; itr ++)
	{//先移除原有的,避免重复
		if(*itr == tExpert)
		{//移除
			m_vectorExpert.erase(itr);
			break;
		}
	}
	if(GWExpertInfo::em_Focus_Off == tExpert.nStatus)
	{//取消关注
	}
	else 
	{//增加关注
		m_vectorExpert.push_back(tExpert);
	}
}

GWExpertInfo * GWUserInfo::GetExpertByID(DWORD dwID, DWORD dwType )
{	
	int nExpertNum = m_vectorExpert.size();
	for(int ii = 0; ii < nExpertNum; ii ++)
	{
		if(m_vectorExpert[ii].dwExpertID == dwID)
		{
			if(-1 == dwType )
			{
				return &m_vectorExpert[ii];
			}
			else
			{
				if(m_vectorExpert[ii].dwType == dwType)				
				{
					return &m_vectorExpert[ii];
				}
			}
		}
	}
	return NULL;
}

BOOL GWUserInfo::IsExpertExist(DWORD dwID, DWORD dwType )
{
	GWExpertInfo * pExpert  = GetExpertByID(dwID, dwType );
	if(NULL == pExpert)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL GWUserInfo::IsAttention()
{
	if(m_vectorExpert.size() > 0)
		return TRUE;
	else
		return FALSE;
}

void GWUserInfo::ClearExpertList()
{
	m_vectorExpert.clear();
}

GWSYMBOLRIGHTVEC &  GWUserInfo::GetSymoblRightVec()
{
	return m_vecSymbolRight;
}

BOOL GWUserInfo::IsNewsShow(tagInfoBombItem  tBombItem)
{
	BOOL bFlag = FALSE;
	int nNewsShowStatus =  GetGloablSetting()->ToInt("NewsShowStatus", 1);
	if(nNewsShowStatus==0)
		return FALSE;

	int nMode =  GetMainAccountLevel();
#ifdef VERSION_TAG_TT
	switch(tBombItem.nTargetUser)
	{
	case 0: //全部用户
		break;
	case 1: //直播室
		if( !GWUserInfo::GetInstance()->IsExpertExist(tBombItem.dwImageID,1))
			return FALSE;
		break;
	case 2: //未激活
		if(nMode >= ACTIVEUSER)
			return FALSE;
		break;
	case 3: //激活
		if(nMode < ACTIVEUSER)
			return FALSE;
		break;
	}

	if(!(nNewsShowStatus == 1))//显示全部
	{
		if((nNewsShowStatus&VBS_STATE_TOP) == VBS_STATE_TOP)  //显示热门
		{
			if( GWUserInfo::GetInstance()->IsExpertExist(tBombItem.dwImageID,1))
				bFlag = TRUE;

			if((nNewsShowStatus&VBS_STATE_ATTENTION) == VBS_STATE_ATTENTION)  //显示已关注
			{
				if( GWUserInfo::GetInstance()->IsExpertExist(tBombItem.dwImageID,0))
					bFlag = TRUE;
			}
		}
		else
		{
			if((nNewsShowStatus&VBS_STATE_ATTENTION) == VBS_STATE_ATTENTION)  //显示已关注
			{
				if(GWUserInfo::GetInstance()->IsExpertExist(tBombItem.dwImageID,0))
					bFlag = TRUE;
			}

		}
	}
	else
		bFlag = TRUE;
#else
	switch(nNewsShowStatus)
	{
	case 1:
		bFlag = TRUE;
	 	break;
	case 2:
		if(GWUserInfo::GetInstance()->IsExpertExist(tBombItem.dwImageID,0))
			bFlag = TRUE;
		break;
	case 3:
		if(!GWUserInfo::GetInstance()->IsExpertExist(tBombItem.dwImageID,0))
			bFlag = TRUE;
		break;
	}
#endif

	return bFlag;
}

void GWUserInfo::AddAccredit(CString strKey,int bValue)
{
	m_mapAccredit[strKey] = bValue;
}

int GWUserInfo::GetAccredit(CString strKey,int nDefault /*= 0*/)
{
	CSTRINGINTMAPITR it = m_mapAccredit.find(strKey);
	if (it == m_mapAccredit.end())
	{
		return nDefault;
	}
	else
	{
		return m_mapAccredit[strKey];
	}
}

BOOL GWUserInfo::HasAccredit(CString strKey)
{
//#ifdef VERSION_TAG_YG
//	CSTRINGINTMAPITR it = m_mapAccredit.find(strKey);
//	if (it == m_mapAccredit.end())
//		return FALSE;
//	else
//	{
//		int nValue = m_mapAccredit[strKey];
//		return nValue > 0 ? TRUE : FALSE;
//	}
//#else
//	return FALSE;
//#endif
	if (strKey.CompareNoCase(_T("tj")) == 0)
		return HasTJAccredit();
	
	return FALSE;
}

BOOL GWUserInfo::HasTJAccredit()
{
	CString strKey = _T("tj");
#if defined (VERSION_TAG_YG) || defined(VERSION_TAG_ZY) || defined(VERSION_TAG_TT) || defined(VERSION_TAG_BSY) || defined(VERSION_TAG_SY)
	if (GWUserInfo::GetInstance()->GetMainAccountLevel() >= ACTIVEUSER || GWUserInfo::GetInstance()->IsInnerStaff())
	{
		return TRUE;
	}
	else
	{
		CSTRINGINTMAPITR it = m_mapAccredit.find(strKey);
		if (it == m_mapAccredit.end())
			return FALSE;
		else
		{
			int nValue = m_mapAccredit[strKey];
			return nValue > 0 ? TRUE : FALSE;
		}
	}
#else
	return FALSE;
#endif
}

GWTaiJiSignal * GWUserInfo::GetTaijiSignal(SymbolObj * pSymbol, int nPeriodType)
{
	int nSignalNum = m_vecTaijiSignal.size();
	for(int ii = 0; ii < nSignalNum; ii ++)
	{
		GWTaiJiSignal & tTaijiSignal = m_vecTaijiSignal[ii];
		if(tTaijiSignal.pSymbol == pSymbol && tTaijiSignal.nKPeriode == nPeriodType )
		{
			return &m_vecTaijiSignal[ii];
		}
	}
	return NULL;
}

BOOL GWUserInfo::AddTaijiSignal(GWTaiJiSignal & tTaijiSignal)
{
	if(NULL == tTaijiSignal.pSymbol)
	{
		return FALSE;
	}
	GWTaiJiSignal *	 pTaijiSignal = GetTaijiSignal(tTaijiSignal.pSymbol, tTaijiSignal.nKPeriode);
	if(NULL == pTaijiSignal)
	{
		m_vecTaijiSignal.push_back(tTaijiSignal);
	}
	else
	{
		*pTaijiSignal = tTaijiSignal;
	}
	return TRUE;
}

CString GWUserInfo::GetSLSPID()
{
	return m_strSLSP;
}

CString GWUserInfo::GetClientType()
{
	return m_strClientType;
}

GWTAIJISIGNALVEC GWUserInfo::GetTaijiSignals(SymbolObj * pSymbol)
{
	GWTAIJISIGNALVEC signals;
	if (pSymbol != NULL)
	{
		int nSignalNum = m_vecTaijiSignal.size();
		for(int ii = 0; ii < nSignalNum; ii ++)
		{
			GWTaiJiSignal & tTaijiSignal = m_vecTaijiSignal[ii];
			if(tTaijiSignal.pSymbol == pSymbol)
			{
				signals.push_back(m_vecTaijiSignal[ii]);
			}
		}
	}
	return signals;
}

BOOL GWUserInfo::IsLocalUser() const
{
	return m_nAccountType == VERSION_ACCOUNTTYPE;
}

void GWUserInfo::SetAccountInfoEx(LPCTSTR lpUID, GWACCOUNTTYPE nType)
{
	m_strUID = lpUID;

	if (IsVisitor())		// 游客默认为本地版本用户
	{
		m_nAccountType = (GWACCOUNTTYPE)VERSION_ACCOUNTTYPE;
	}
	else
	{
		m_nAccountType = nType;
	}
}



GWQKSignal* GWUserInfo::GetQKSignal(SymbolObj* pSymbol, int nPeriod, int nSignalKind)
{
	int nQKSignalNum = m_vecQKSignal.size();
	for(int ii = 0; ii < nQKSignalNum; ii ++)
	{
		GWQKSignal& tQKSignal = m_vecQKSignal[ii];
		if(tQKSignal.pSymbol == pSymbol && 
			tQKSignal.nPeriod == nPeriod &&
			tQKSignal.nSignalKind == nSignalKind)
		{
			return & tQKSignal;
		}
	}
	return NULL;
}

void GWUserInfo::AddQKSignal(GWQKSignal & tQKSignal)
{
	GWQKSignal* pQKSignal = this->GetQKSignal(tQKSignal.pSymbol, tQKSignal.nPeriod, tQKSignal.nSignalKind);
	if(NULL == pQKSignal)
	{
		m_vecQKSignal.push_back(tQKSignal);
	}
	else
	{
		*pQKSignal = tQKSignal;
	}
}

void GWUserInfo::AddQKSignal(GWQKSIGNALVEC & vecQKSignal)
{
	int nQKNum = vecQKSignal.size();
	ASSERT( 1 == nQKNum );
	for(int ii = 0; ii < nQKNum; ii ++)
	{
		AddQKSignal(vecQKSignal[ii]);
	}
}

int  GWUserInfo::GetQKState(SymbolObj* pSymbol, int nPeriod, int nSignalKind, time_t dwActiveTime)
{
	GWQKSignal* pQKSignal = this->GetQKSignal(pSymbol, nPeriod, nSignalKind);
	if(NULL == pQKSignal)
	{
		return GWQKSignal::em_Trend_Zero;
	}
#if 0
	if(nSignalKind ==GWQKSignal::em_QK_Map &&  pSymbol->pMarketIfo->nTradeDate != pQKSignal->dwTradeDay)
	{//交易日更换，则不明确 乾坤线不带交易日
		return GWQKSignal::em_Trend_Zero;
	}
#endif
	if(pQKSignal->dwExpireTime != pQKSignal->dwLastOccurTime)
	{//延续信号
		return GWQKSignal::em_Trend_Zero;
	}
	int nTimeSpan = nPeriod * 60;
	time_t nDif  = pQKSignal->dwExpireTime - pQKSignal->dwLastOccurTime;
	if(nDif > nTimeSpan)
	{
		return GWQKSignal::em_Trend_Zero;
	}
	if(0 == dwActiveTime)
	{
		dwActiveTime = pSymbol->snapshotData.tmTime;
	}
	dwActiveTime = (dwActiveTime / 60) * 60;	//剔除 秒
	nDif = pQKSignal->dwExpireTime - dwActiveTime - nPeriod * 60;
	if(dwActiveTime > pQKSignal->dwExpireTime || nDif > 0 )
	{//时间超过 
		return GWQKSignal::em_Trend_Zero;
	}
	return pQKSignal->nBS;
}
