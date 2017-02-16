#include "stdafx.h"
#include "NetManagerMakket.h"
#include "ManagerMarket.h"

//CManagerMarket::CManagerMarket(void)
//{
//}

CManagerMarket::~CManagerMarket(void)
{
	if (m_pLog)
	{
		fclose(m_pLog);
		m_pLog = NULL;
	}
}

void CManagerMarket::InitInstance()
{
	m_bRunning = FALSE;
	m_pLog = NULL;
	m_pLog = fopen("./MarketLog.txt", "w");
}

UINT CManagerMarket::HandleAllRecvPacketThread()
{
	while(m_bRunning)
	{
		Market * pOnePacket = m_cPacketList.pop();
		if(NULL == pOnePacket)
		{
			Sleep(10);
			continue;
		}
		pOnePacket = m_pMultPacktCtrl->MergePacket(pOnePacket);
		if(NULL == pOnePacket)
		{//如果是多包，则会合并后返回；不完整的包被存储
			continue;
		}
		//已经合并成json body
		string strJson = ::UTF8ToGB(pOnePacket->pszBody); 
		//BOOL bRet = ParserJsonResponse(strJson, pOnePacket->tPackeHead.wCmd);
		BOOL bRet = ParserJsonResponse(strJson, pOnePacket->tPackeHead.dwCmd);

		G_FREE_ONE_MEM(pOnePacket);
	}
	return 1;
}

UINT CManagerMarket::HandleAllRequestThread()
{
	return 1;
}

void CManagerMarket::ClearAllPacket()
{

}

BOOL CManagerMarket::ConnetToTS(const char* strIP, int nPort,int iTotal)
{
	return CNetManagerMakket::GetInstance()->ConnectToTS(strIP,nPort,iTotal);
}

void CManagerMarket::Start()
{
	if(!m_bRunning)
	{
		CNetManagerMakket::GetInstance()->Start();
		m_bRunning = TRUE;
		SafeStartThread(&CManagerMarket::HandleAllRecvPacketThread,0);
	}
}

void CManagerMarket::Stop()
{
	if(FALSE == m_bRunning)
	{
		return;
	}
	m_bRunning = FALSE;
	CNetManagerMakket::GetInstance()->Stop();
}
void  PostRequest(DWORD dwCmd, Json::Value &jsRequest)
{
	Json::FastWriter jsWriter;
	string strReqText = jsWriter.write(jsRequest);
	CNetManagerMakket::GetInstance()->PushOnePacket(dwCmd, strReqText);
}

BOOL CManagerMarket::Login(const char* strName,const char* strPwd)
{
	Json::Value jsRequest;
	try
	{
		jsRequest[_T("usr")] = Json::Value((LPCTSTR)strName);	// 用户名
		jsRequest["pwd"] =  Json::Value((LPCTSTR)strPwd); // 密码	
	}
	catch(std::runtime_error e)
	{
		CManagerMarket::GetInstance()->FormatLog(1,"登录错误[%s]", const_cast<char*>(e.what()));
		return FALSE;
	}
	catch(...)
	{
		CManagerMarket::GetInstance()->FormatLog(1,"请求登录发生未知错误","");
		return FALSE;
	}
	::PostRequest(GWTS_CMD_LOGIN, jsRequest);
	return TRUE;
}

void CManagerMarket::SetNotifyWnd(HWND hNotifyWnd)
{
	m_hNotifyWnd = hNotifyWnd;
}

void CManagerMarket::SetDispatchWnd(HWND hDispatchWnd)
{
	m_hDispatchWnd = hDispatchWnd;
}

void CManagerMarket::PushOnePacket(Market * pOnePacket)
{
	m_cPacketList.push(pOnePacket);
}

CString GetStringFromJsonValue(Json::Value& jsValue, LPCSTR lpKey, LPCTSTR lpDefault="")
{
	CString strValue;

	try
	{
		Json::Value tmp = jsValue[lpKey];

		if (tmp.empty())
		{
			CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		}
		else
		{
			strValue = tmp.asString().c_str();
		}
	}
	catch (...)
	{
		CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		ASSERT(0);
	}

	if (strValue.IsEmpty() && lpDefault != NULL)
	{
		strValue = lpDefault;
	}

	return strValue;
}



int GetInt64FromJsonValue(Json::Value& jsValue, LPCSTR lpKey)
{
	DWORD nValue = 0;
	CString strValue;
	CString strValue1;
	try
	{
		Json::Value tmp = jsValue[lpKey];

		if (tmp.empty())
		{
			CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		}
		else
		{

			strValue = tmp.asString().c_str();
			strValue1 = strValue.Left(10);  //转为int
			nValue = (DWORD)(atol(strValue1));
			//nValue = atoll(strValue);
		}
	}
	catch (...)
	{
		CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		ASSERT(0);
	}

	return nValue;
}



int GetIntFromJsonValue(Json::Value& jsValue, LPCSTR lpKey)
{
	int nValue = 0;

	try
	{
		Json::Value tmp = jsValue[lpKey];

		if (tmp.empty())
		{
			CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		}
		else
		{
			nValue = tmp.asInt();
		}
	}
	catch (...)
	{
		CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		ASSERT(0);
	}

	return nValue;
}

double GetDoubleFromJsonValueEX(Json::Value& jsValue, LPCSTR lpKey)
{
	double dbValue = 0.0;

	try
	{
		Json::Value tmp = jsValue[lpKey];

		if (tmp.empty())
		{
			CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		}
		else
		{
			CString strValue = GetStringFromJsonValue(jsValue, lpKey);
			dbValue = atof(strValue);
		}
	}
	catch (...)
	{
		CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		ASSERT(0);
	}

	return dbValue;
}

double GetDoubleFromJsonValue(Json::Value& jsValue, LPCSTR lpKey)
{
	double dbValue = 0.0;

	try
	{
		Json::Value tmp = jsValue[lpKey];

		if (tmp.empty())
		{
			CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		}
		else
		{
			dbValue = tmp.asDouble();
		}
	}
	catch (...)
	{
		CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		ASSERT(0);
	}

	return dbValue;
}

char GetCharFromJsonValue(Json::Value& jsValue, LPCSTR lpKey)
{
	CString strValue = GetStringFromJsonValue(jsValue, lpKey);

	char ch = '\0';

	if (!strValue.IsEmpty())
	{
		ch = strValue.GetAt(0);
	}

	return ch;
}

GWFloat GetGWFloatFromJsonValue(Json::Value& jsValue, LPCSTR lpKey, int nDigit)
{
	GWFloat gfValue;
	try
	{
		Json::Value tmp = jsValue[lpKey];

		if (tmp.empty())
		{
			CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
		}
		else
		{
			CString strValue = GetStringFromJsonValue(jsValue, lpKey);
			gfValue.SetValue(atof(strValue), nDigit);
		}
	}
	catch (...)
	{
		CManagerMarket::GetInstance()->FormatLog(1,"无法从Json字符串[%s]中获取[%s]对应的值", const_cast<char*>(jsValue.toStyledString().c_str()), lpKey);	
	}

	return gfValue;
}

#define GET_JSON_STRINGVALUE(structobj, field, jvalue)						structobj->field = GetStringFromJsonValue(jvalue, #field)
#define GET_JSON_STRINGVALUE_EX(structobj, field, jvalue, defaultvalue)		structobj->field = GetStringFromJsonValue(jvalue, #field, defaultvalue)
#define GET_JSON_GWFLOATVALUE(structobj, field, jvalue, digit)				structobj->field = GetGWFloatFromJsonValue(jvalue, #field, digit)
#define GET_JSON_INTVALUE(structobj, field, jvalue)							structobj->field = GetIntFromJsonValue(jvalue, #field)
#define GET_JSON_INT64VALUE(structobj, field, jvalue)						structobj->field = GetInt64FromJsonValue(jvalue, #field)
#define GET_JSON_DOUBLEVALUE(structobj, field, jvalue)						structobj->field = GetDoubleFromJsonValue(jvalue, #field)
#define GET_JSON_DOUBLEVALUE_EX(structobj, field, jvalue)					structobj->field = GetDoubleFromJsonValueEX(jvalue, #field)
#define GET_JSON_CHARVALUE(structobj, field, jvalue)						structobj->field = GetCharFromJsonValue(jvalue, #field)
BOOL CManagerMarket::ParserJsonResponse(string& strJson, DWORD dwCmd)
{
 	Json::Reader jsReader;
 	Json::Value  jsAns;	
 	if (!jsReader.parse(strJson, jsAns))
 	{
 		ASSERT (FALSE);
 		return FALSE;
 	}
 	if(jsAns.empty())
 	{
 		return TRUE;
 	}
 
 	tagRspMarketHead tRspHead;
 	BOOL bRet = ParserHead(tRspHead, jsAns);
 	if(FALSE == bRet)
 	{
 		//ASSERT(0);
 	}
 	Json::Value   jsBody;
 	try
 	{
 		jsBody = jsAns["body"];
 	}
 	catch (...)
 	{
 		ASSERT(0);	
 		return FALSE;	
 	}
 	BOOL bNeedNotifyMessage = FALSE;
 	//各个包处理
 	switch(dwCmd)
 	{
// 	case GWTS_CMD_LOGIN:	//登录
// 		{
// 			if(GWTS_OK == tRspHead.ret )
// 			{
// 				OnLoginOK();
// 			}
// 			bNeedNotifyMessage = TRUE;
// 		}
// 		break;
	case 1:
		break;
 	default:
 		break;
 	}
 	if(bNeedNotifyMessage)
 	{
 		this->PostTSMessage(dwCmd, tRspHead.ret, (LPARAM)(LPCTSTR)tRspHead.msg);
 	}

	return TRUE;
}

BOOL CManagerMarket::ParserHead(tagRspMarketHead& tRspHead, Json::Value& jsAns)
{
	tagRspMarketHead * pRspHead = & tRspHead;
	GET_JSON_INTVALUE(pRspHead, ret, jsAns);
	GET_JSON_STRINGVALUE(pRspHead, msg, jsAns);
	GET_JSON_STRINGVALUE(pRspHead, uname, jsAns);
	GET_JSON_STRINGVALUE(pRspHead, yg_trade_key, jsAns);
	if(tRspHead.uname.IsEmpty())
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CManagerMarket::PostTSMessage(DWORD dwMessage, WPARAM wPara /*= 0*/, LPARAM lPara /*= 0*/, BOOL bSend /*= TRUE*/)
{
	if(NULL == m_hNotifyWnd)
	{
		return FALSE;
	}
	if(FALSE == bSend)
	{
		::PostMessage(m_hNotifyWnd, dwMessage, wPara, lPara);
	}
	else
	{
		::SendMessage(m_hNotifyWnd, dwMessage, wPara, lPara);
	}
	return TRUE;
}

void CManagerMarket::FormatLog(int nLevel, const char* chDomain, char* chText, ...)
{
	char strLog[500],szParam[500];
// 	switch(nLevel)
// 	{
// 	case LOG_APP_LEVEL:
// 		sprintf(strLog, "CTraderSpi::%s\t [Info] ", chDomain);
// 		break;
// 	case LOG_DEBUG_LEVEL:
// 		sprintf(strLog, "CTraderSpi::%s\t [Debug] ", chDomain);
// 		break;
// 	case LOG_WARN_LEVEL:
// 		sprintf(strLog, "CTraderSpi::%s\t [Warn] ", chDomain);
// 		break;
// 	case LOG_ERROR_LEVEL:
// 		sprintf(strLog, "CTraderSpi::%s\t [Err] ", chDomain);
// 		break;
// 	}
	sprintf(strLog, "CManagerMarket::%s ", chDomain);
	va_list args;
	va_start(args,chText);
	_vsnprintf(szParam, 490, chText, args);
	va_end(args);
	strcat(strLog, szParam);
	if (m_pLog)
	{
		fprintf(m_pLog, "%s\n", strLog);
		fflush(m_pLog);
	}
}
