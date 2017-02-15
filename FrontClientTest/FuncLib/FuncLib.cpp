// FuncLib.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FuncLib.h"
#include "FuncLibEx.h"
#include "GlobalSetting.h"
#include <json/json.h>
#include "CodeListMgr.h"
#include <GWMessage.h>
#include "GWUserInfo.h"
#include "PluginManager.h"
//#include "LuaWrap.h"
#include <mmsystem.h>
#include "AuthorizeDefine.h"
#pragma comment(lib, "winmm.lib") 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace GW_Version
{
	GWBOOL IsVisitor()
	{
		return GetUserInfo()->IsVisitor();
	}

	GWBOOL IsRegisteredUser()
	{
		return GetUserInfo()->IsRegisteredUser();
	}

	GWBOOL IsActiveUser()
	{
		return GetUserInfo()->IsActiveUser();
	}

	GWBOOL IsVip()
	{
		return GetUserInfo()->IsVIP();
	}

	GW_FUNCLIB_API GWBOOL IsInnerStaff()
	{
		return GetUserInfo()->IsInnerStaff();
	}

}

namespace GW_Common
{
	HWND GetTopLevelParent(HWND hWnd)
	{
		ASSERT(hWnd != NULL);

		if (hWnd == NULL)
		{
			return NULL;
		}

		HWND hParent = hWnd;
		
		do
		{
			hWnd = hParent;
			hParent = ::GetParent(hWnd);
			
		} while (hParent != NULL);

		ASSERT(hWnd != NULL);

		return hWnd;
	}

	CWnd * g_ActiveFrameWnd = NULL;

	GWVOID SetActiveFrameWnd(CWnd* pFrameWnd)
	{
		ASSERT(pFrameWnd != NULL);

		g_ActiveFrameWnd = pFrameWnd;
	}

	CWnd* GetActiveFrameWnd()
	{
		return g_ActiveFrameWnd;
	}

	SymbolObj* GetActiveSymbolObj()
	{
		ASSERT(g_ActiveFrameWnd != NULL);

		if (g_ActiveFrameWnd == NULL)
		{
			return NULL;
		}

		SymbolObj *pSymbolObj = (SymbolObj *)g_ActiveFrameWnd->SendMessage(MSG_QUERYMAINSYMBOLOBJ, 0, 0);
		return pSymbolObj;
	}

	const CString CreateGUID(BOOL bFullMode)
	{
		GUID guid;
		CoCreateGuid(&guid);

		WCHAR * pGUIDStr;
		StringFromCLSID(guid, &pGUIDStr);

		CString strGUID(pGUIDStr);

		CoTaskMemFree(pGUIDStr);

		if (!bFullMode)
		{
			strGUID.Trim(_T("{}"));
		}

		return strGUID;
	}

	GWBOOL PutStringToClipboard(const CString& strText)
	{
		if (!OpenClipboard(NULL)) 
		{
			return FALSE ; 
		}

		EmptyClipboard(); 

		// Allocate a global memory object for the text. 
		int nSize = strText.GetLength() + 1;

		HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, nSize * sizeof(TCHAR)); 

		if (hglbCopy == NULL) 
		{ 
			CloseClipboard(); 
			return FALSE; 
		} 

		LPSTR lpstrCopy = (LPSTR)GlobalLock(hglbCopy);

		ZeroMemory(lpstrCopy, nSize);
		memcpy_s(lpstrCopy, nSize, strText, nSize);
		GlobalUnlock(hglbCopy); 

#ifdef _UNICODE
		SetClipboardData(CF_UNICODETEXT, hglbCopy); 
#else
		SetClipboardData(CF_TEXT, hglbCopy); 
#endif

		CloseClipboard();

		return TRUE;
	}

	GWBOOL QuickOpenUrl(LPCTSTR lpApp, LPCTSTR lpSection)
	{
		CString strUrl = GW_Cfg::GetUrlFromCfg(lpApp, lpSection);

		if (!strUrl.IsEmpty())
		{
			ShellExecute(NULL, "open", strUrl, NULL, NULL, SW_SHOWNORMAL);

			return TRUE;
		}	

		return FALSE;
	}


	CView*	CreateViewByName(const CString& strViewName, CWnd * pParent, BOOL bInit, SymbolObj * pSymbolObj, const CString& strParam )
	{ 
		CView * pView = SAFE_GETVIEW(GetPluginManager()->CreateViewByName(strViewName));

		if (pView == NULL)
		{
			return NULL;
		}

		DWORD dwStyle = WS_CHILD | WS_CLIPCHILDREN;
		BOOL  bRet;

		bRet = pView->CreateEx(0,
			NULL,
			NULL,
			dwStyle,
			CRect(0,0,0,0),
			pParent,
			GetPluginManager()->GetNextID(),
			NULL);
		pView->OnInitialUpdate();
		if(bInit)
		{
			IGWViewPlugin *pPlugin = GWGetInterface(pView, IGWViewPlugin);
			ASSERT( pPlugin != NULL);

			if (pPlugin) 
			{
				pPlugin->ViewInitialize(pSymbolObj, strParam, 0);
			}
		}
		return pView;
	}

	GWBOOL IsDoubleEqual(double dfVal1, double dfVal2)
	{
		double dfDiff = dfVal1 - dfVal2;
		if(dfDiff > DOUBLE_ZERO || dfDiff < - DOUBLE_ZERO)
		{
			return FALSE;
		}
		return TRUE;
	}

	GWBOOL CallScriptFunction(const char*filename, const char* func)
	{
		WriteDebugMessage(_T("暂时不支持脚本调用,文件[%s],函数[%s]"), filename, func);
		return FALSE;
// 		if (!GW_PathFile::IsPathFileExist(filename))		// 没有脚本文件
// 		{
// 			return FALSE;
// 		}
// 
// 		LuaWrap l;
// 
// 		if (!l.LoadFile(filename))
// 		{
// 			WriteLogMessage(LogError, l.GetLastErrorStr());
// 		}
// 
// 		return l.Excute(func);
	}

	GW_FUNCLIB_API CString GetViewType(BYTE byViewType)
	{
		const TCHAR VIEW_TYPE[][16] = { _T("fst"), _T("kxt"), _T("zbt"), _T("f10"), _T("dzq"), _T("fjb"), _T("hq"), _T("dpz"), _T("zy"), _T("login"), _T("toolbar") };
		if(byViewType < em_View_Max)
		{
			return VIEW_TYPE[byViewType];
		}
		return _T("");
	} 
	
	GW_FUNCLIB_API GWVOID ReportUserAction(BYTE byFromViewType, const CString& strKey,DWORD dwID,  BYTE byActionType, BYTE byChangeType)
	{
		CString & strViewType = GetViewType(byFromViewType);
		ReportUserAction(strKey, dwID, byActionType, byChangeType, _T(""), strViewType, _T(""));		
	}

	GW_FUNCLIB_API GWVOID ReportUserAction2(BYTE byToViewType, const CString& strKey,DWORD dwID,  BYTE byActionType, BYTE byChangeType)
	{
		CString & strViewType = GetViewType(byToViewType);
		ReportUserAction(strKey, dwID, byActionType, byChangeType, _T(""), _T(""), strViewType);		
	}


	GW_FUNCLIB_API GWVOID ReportUserAction(const CString& strKey,  DWORD dwID, BYTE byActionType, BYTE byChangeType,
											const CString& strSymbol, const CString& strFromViewClass,const CString & strToViewClass)
	{ 
		if(NULL == g_ActiveFrameWnd)
		{
			return;
		}

		//const TCHAR VIEW_NAME[][16] = { _T("fst"), _T("kxt"), _T("zbt"), _T("f10"), _T("dzq"), _T("fjb"), _T("hq"), _T("dpz") };
		//分时图  K线图  逐笔图  F10 多周期同列 分价表 行情首页 多品种同列
		DirectCommand *pCommand = new DirectCommand;
		pCommand->strCmd.Format( _T("userAction?key=%s;id=%d;actiontype=%d;changetype=%d;symbol=%s;from=%s;to=%s"), 
								(LPCTSTR)strKey,dwID, byActionType, byChangeType, (LPCTSTR)strSymbol, (LPCTSTR)strFromViewClass, (LPCTSTR)strToViewClass); 
		g_ActiveFrameWnd->PostMessage(MSG_DIRECT_COMMAND, (WPARAM)pCommand, 0);
	}

	struct MSGEX : public MSG
	{
		BOOL bPostMessage;
	};

	BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM lParam)
	{
		MSGEX* pMsg = (MSGEX*)lParam;
		ASSERT(pMsg != NULL);

		if (pMsg == NULL)
		{
			return FALSE;
		}

		if (pMsg->bPostMessage)
		{
			::PostMessage(hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
		}
		else
		{
			::SendMessage(hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
		}

		return TRUE;
	}

	GW_FUNCLIB_API void BroadcastMessage(HWND hParent, UINT msg, WPARAM wp, LPARAM lp, BOOL bPostMessage)
	{
		MSGEX* pMsg = new MSGEX;
		pMsg->message = msg;
		pMsg->wParam = wp;
		pMsg->lParam = lp;
		pMsg->bPostMessage = bPostMessage;

		EnumChildWindows(hParent, EnumThreadWndProc, (LPARAM)pMsg);

		delete pMsg;
	}

}

namespace GW_Json
{
	using namespace Json;

	GWVOID PrintJsonString(Json::Value& jsValue)
	{
		Json::StyledWriter jsDebugWriter;
		string strText = jsDebugWriter.write(jsValue);
		WriteDebugMessage(strText.c_str());
	}

	CString StringFromJsonValue(Json::Value& jsValue)
	{
		Json::FastWriter jsWriter;
		string strText = jsWriter.write(jsValue);
		return strText.c_str();
	}

}

namespace GW_Time
{
	GWLONG TimeFromTimeT(time_t t, TIME_FORMAT fmt)
	{
		CTime tm(t);

		long iRet = 0;

		ASSERT(fmt != TIME_FULL);  // 不支持这个，超出范围

		if (fmt == TIME_FULL)
		{
			return 0;
		}

		switch(fmt)
		{
		case TIME_YMD:
			{
				iRet = tm.GetYear() * 10000 + tm.GetMonth() * 100 + tm.GetDay();
			}
			break;
		case TIME_MD:
			{
				iRet = tm.GetMonth() * 100 + tm.GetDay();
			}
			break;
		case TIME_HMS:
			{
				iRet = tm.GetHour() * 10000 + tm.GetMinute() * 100 + tm.GetSecond();
			}
			break;
		case TIME_HM:
			{
				iRet = tm.GetMinute() * 100 + tm.GetSecond();
			}
			break;
			// 	case TIME_FULL:
			// 		{
			// 			iRet = tm.GetYear() * 10000000000 + tm.GetMonth() * 100000000 + tm.GetDay() * 1000000 + tm.GetHour() * 10000 + tm.GetMinute() * 100 + tm.GetSecond();
			// 		}
			// 		break;
		}

		return iRet;
	}

	time_t TimeTFromDate(long nDate)
	{
		if(0 >= nDate)
		{
			return 0;
		}
		long nYear  = nDate / 10000;
		long nMonth = (nDate - nYear * 10000) / 100;
		long nDay   = nDate % 100;

		CTime tm(nYear, nMonth, nDay, 0 , 0, 0);

		return tm.GetTime();
	}

	GW_DLLEXPORT CString StringFromTimeT(time_t t, TIME_FORMAT fmt, const CString& strDateSp, const CString& strTimeSp, const CString& strDateTimeSp)
	{
		CString strTime;

		CTime tm(t);

		switch(fmt)
		{
		case TIME_YMD:
			{
				strTime.Format(_T("%04d%s%02d%s%02d"), tm.GetYear(), strDateSp, tm.GetMonth(), strDateSp, tm.GetDay());
			}
			break;
		case TIME_MD:
			{
				strTime.Format(_T("%02d%s%02d"), tm.GetMonth(), strDateSp, tm.GetDay());
			}
			break;
		case TIME_HMS:
			{
				strTime.Format(_T("%02d%s%02d%s%02d"), tm.GetHour(), strTimeSp, tm.GetMinute(), strTimeSp, tm.GetSecond());
			}
			break;
		case TIME_HM:
			{
				strTime.Format(_T("%02d%s%02d"), tm.GetMinute(), strTimeSp, tm.GetSecond());
			}
			break;
		case TIME_FULL:
			{
				strTime.Format(
								_T("%04d%s%02d%s%02d%s%02d%s%02d%s%02d"),
								tm.GetYear(), 
								strDateSp, 
								tm.GetMonth(), 
								strDateSp, 
								tm.GetDay(),
								strDateTimeSp,
								tm.GetHour(), 
								strTimeSp, 
								tm.GetMinute(), 
								strTimeSp, 
								tm.GetSecond()
								);
			}
			break;
		}

		return strTime;
	}

	// 根据交易日生成字符时间串
	GW_DLLEXPORT CString StringFromDate(long nDate, TIME_FORMAT fmt, const CString& strDateSp, const CString& strTimeSp, const CString& strDateTimeSp)
	{
		return StringFromTimeT(nDate == 0 ? 0 : TimeTFromDate(nDate), fmt, strDateSp, strTimeSp, strDateTimeSp);
	}
}

namespace GW_HQ
{
	GWVOID AddMarketToMarketMap(MarketInfo* pMarket)
	{
		GetCodeListMgr()->AddMarket(pMarket);
	}

	MarketInfo* FindMarket(const CString& strMarket)
	{
		return GetCodeListMgr()->FindMarket(strMarket);
	}
	
	GWVOID AddSymbolToCodeList(SymbolObj* pSymbolObj)
	{
		GetCodeListMgr()->AddSymbol(pSymbolObj);
	}
	
	GWVOID GetAllMarketNames(CSTRINGVEC& vecName)
	{
		GetCodeListMgr()->GetAllMarketNames(vecName);
	}

	GWVOID GetAllMarket(MRKETVEC& vecMarket)	
	{
		GetCodeListMgr()->GetAllMarket(vecMarket);
	}

	GWVOID GetMarketSymbols(const CString& strMarket, SYMBOLVEC& vecSymbol, BOOL bOnlyTopLevelSymbol)
	{
		GetCodeListMgr()->GetMarketSymbols(strMarket, vecSymbol, bOnlyTopLevelSymbol);
	}

	GW_FUNCLIB_API GWVOID GetMarketSymbols(SymbolObj& pParentSymbol, SYMBOLVEC& vecChildrenSymbol)
	{
		if (!pParentSymbol.HasChildren())
		{
			return;
		}

		for (CSTRINGVECITR itr = pParentSymbol.vecChildren.begin(); itr != pParentSymbol.vecChildren.end(); ++itr)
		{
			SymbolObj* pSymbol = FindSymbolObjEx(*itr);

			if (pSymbol == NULL)
			{
				WriteDebugMessage(_T("[%s]无法找到子节点[%s]"), pSymbol->GetObject(), *itr);
				continue;
			}

			vecChildrenSymbol.push_back(pSymbol);
		}
	}

	GWVOID GetAllMarketSymbols(SYMBOLVEC& vecAllSymbol)		
	{
		vecAllSymbol.clear();
		MRKETVEC  vecMarket;
		GetAllMarket(vecMarket);
		int nMarketNum = vecMarket.size();
		for(int ii = 0; ii < nMarketNum; ii ++)
		{
			MarketInfo* pMarketInfo = vecMarket[ii];
			if(NULL == pMarketInfo)
			{
				continue;;
			}
			SYMBOLVEC vecSymbol;
			GetMarketSymbols(pMarketInfo->strMarketName,  vecSymbol);
			int nSymbolNum = vecSymbol.size();
			for(int jj = 0; jj < nSymbolNum; jj ++)
			{
				if(NULL == vecSymbol[jj])
				{
					continue;;
				}
				vecAllSymbol.push_back(vecSymbol[jj]);
			}
		}
	}

	SymbolObj* FindSymbolFromAllMarket(const CString &strCode)	
	{
		MRKETVEC  vecMarket;
		GetAllMarket(vecMarket);
		int nMarketNum = vecMarket.size();
		for(int ii = 0; ii < nMarketNum; ii ++)
		{
			MarketInfo* pMarketInfo = vecMarket[ii];
			if(NULL == pMarketInfo)
			{
				continue;;
			}
			SYMBOLVEC vecSymbol;
			GetMarketSymbols(pMarketInfo->strMarketName,  vecSymbol);
			int nSymbolNum = vecSymbol.size();
			for(int jj = 0; jj < nSymbolNum; jj ++)
			{
				if(NULL == vecSymbol[jj])
				{
					continue;;
				}
				if(vecSymbol[jj]->strCode == strCode)
				{
					return vecSymbol[jj];
				}
			}
		}

		return NULL;
	}

	SymbolObj* FindSymbolObj(const CString& strMarket, const CString &strCode)	
	{
		return GetCodeListMgr()->FindSymbolObj(strMarket, strCode);
	}

	SymbolObj* FindSymbolObjEx(const CString& strObj)
	{
		if (strObj.IsEmpty())
		{
			return NULL;
		}

		stringsplitter sp(".");
		sp.split(strObj);

		if (sp.size() == 1)
		{
			return FindSymbolFromAllMarket(sp[0]);
		}
		else if (sp.size() == 2)
		{
			return FindSymbolObj(sp[0], sp[1]);
		}

		return NULL; 
	}

	SymbolObj*	GetNextSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket)				
	{
		return GetCodeListMgr()->GetNextSymbolObj(pSymbolObj, strMarket);
	}

	SymbolObj*	GetPreviousSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket)			
	{
		return GetCodeListMgr()->GetPreviousSymbolObj(pSymbolObj, strMarket);
	}

	GWVOID GetMarketSymbolLoopVector(SYMBOLVEC& vecMarketSymbol, const CString& strMarket, SymbolObj* pStartSymbolObj, UINT nMaxCount, BOOL bLoop)
	{
		GetCodeListMgr()->GetMarketSymbolLoopVector(vecMarketSymbol, strMarket, pStartSymbolObj, nMaxCount, bLoop);
	}
	GWBOOL IsVolMarket(SymbolObj* pSymbolObj)
	{
		if(NULL == pSymbolObj)
		{
			return FALSE;
		}
		return IsVolMarket(pSymbolObj->pMarketIfo);
	}
	GWBOOL IsVolMarket(MarketInfo* pMarket)
	{
		if(NULL == pMarket)
		{
			return FALSE;
		}
		if(pMarket->nTradeGearNum > 1)
		{
			return TRUE;
		}
		return FALSE;
	} 

	GW_FUNCLIB_API void SetActiveSymbols(SYMBOLVEC& vecSymbols)
	{
		GetCodeListMgr()->SetActiveSymbolsVec(vecSymbols);
	}

	GW_FUNCLIB_API CString GetTJXName(int nKType)
	{
		CString strTJXName;
		if (nKType == KT_60MIN)
			strTJXName = TJX60;
		else if (nKType == KT_180MIN)
			strTJXName = TJX180;
		else if (nKType == KT_240MIN)
			strTJXName = TJX240;

		return strTJXName;
	}

	GW_FUNCLIB_API CString GetQKXName(int nKType, int nQKX)
	{
		CString strTJXName;
		if(1 == nQKX || 0 == nQKX)
		{
			if (nKType == KT_30MIN)
				strTJXName = QKX30;
			else if (nKType == KT_60MIN)
				strTJXName = QKX60;
			else if (nKType == KT_120MIN)
				strTJXName = QKX120;
			else if (nKType == KT_180MIN)
				strTJXName = QKX180;
			else if (nKType == KT_240MIN)
				strTJXName = QKX240;
			else if (nKType == KT_DAY)
				strTJXName = QKX1DAY;
		}
		else if(tagQKSignal::em_QK_Map == nQKX)
		{
			if (nKType == KT_DAY)
				strTJXName = QKT1DAY;
		}
		else if(tagQKSignal::em_BYJZ == nQKX)
		{
			if (nKType == KT_60MIN)
				strTJXName = BYJZ60;
			else if (nKType == KT_120MIN)
				strTJXName = BYJZ120;
		}
		else
		{
			ASSERT(0);
		}

		return strTJXName;
	}
	
	GW_FUNCLIB_API int PeriodMin2KType(int nPeriodMin)
	{
		int nKType = KT_DAY;
		switch (nPeriodMin)
		{
		case 60:
			nKType = KT_60MIN;
			break;
		case 120:
			nKType = KT_120MIN;
			break;
		case 180:
			nKType = KT_180MIN;
			break;
		case 1440:
			nKType = KT_DAY;
			break;
		default: 
			ASSERT(0);
			break;
		} 
		return nKType;
	}

	GW_FUNCLIB_API int KType2PeriodMin(int nKType)
	{
		int nPeriodMin = 1440;
		switch(nKType)
		{
		case KT_60MIN:
			nPeriodMin = 60;
			break;
		case KT_120MIN:
			nPeriodMin = 120;
			break;
		case KT_180MIN:
			nPeriodMin = 80;
			break;
		case KT_DAY:
			nPeriodMin = 1440;
			break;
		default:
			ASSERT(0);
			break;
		}
		return nPeriodMin;
	}

	GW_FUNCLIB_API CString GetTJXNameEx(int nPeriodMin)
	{
		int nKType = PeriodMin2KType(nPeriodMin);

		return GetTJXName(nKType);
	}

	GW_FUNCLIB_API CString GetQKXNameEx(int nPeriodMin, int nQKX)
	{
		int nKType = PeriodMin2KType(nPeriodMin);
		return GetQKXName(nKType, nQKX);

	}
	GW_FUNCLIB_API CString GetBYJZName(int nPeriodMin)
	{
		CString strBYJZ = BYJZ60;
		switch(nPeriodMin)
		{
		case 120:
			strBYJZ = BYJZ120;
			break;
		default:
			break;
		}
		return strBYJZ;

	}
}

namespace GW_Msg
{
	// 发送MSG_DIRECT_COMMAND
	GW_DLLEXPORT GWRESULT SendDirectMessage(const CString& strMsgContent, CWnd* pSenderWnd, CWnd * pTargetWnd, BOOL bPost)
	{
		DirectCommand *pCommand = new DirectCommand;
		pCommand->strCmd = strMsgContent;

		if (pTargetWnd == NULL)
		{
			pTargetWnd = GW_Common::GetActiveFrameWnd();
		}

		ASSERT(pTargetWnd != NULL);

		GWRESULT lRet = 0;

		if (bPost)
		{
			lRet = pTargetWnd->PostMessage(MSG_DIRECT_COMMAND, (WPARAM)pCommand, LPARAM(pSenderWnd));	
		}
		else
		{
			lRet = pTargetWnd->SendMessage(MSG_DIRECT_COMMAND, (WPARAM)pCommand, LPARAM(pSenderWnd));	

		}

		return lRet;
	}


	
}

namespace GW_Multimedia
{
	BOOL PlaySoundEx(LPCTSTR pszSound,BOOL bBeep)
	{
		CString strTipSoundFile = GlobalSetting::GetInstance()->GetGFSubDir(GDT_RES) + pszSound;
		if(GW_PathFile::IsPathFileExist((LPCTSTR)strTipSoundFile))
		{
			::PlaySound((LPCTSTR)strTipSoundFile, NULL, SND_FILENAME | SND_ASYNC);
		} 
		else
		{//循环播放系统声音 
			if (bBeep)
				::MessageBeep(MB_ICONASTERISK);
		}

		return TRUE;
	}

}

GW_FUNCLIB_API void GW_UI::SetUIMode(const CString& strMode)
{
	GetGloablSetting()->SetUIMode(strMode);
}

GW_FUNCLIB_API COLORREF GW_UI::GetUIColorImpl(UINT uKeyID,  COLORREF clrDefault)
{
	COLORREF clColor = GetGloablSetting()->GetCurSkinColorByID(uKeyID,  clrDefault);
	return clColor;
}
