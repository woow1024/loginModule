#include "StdAfx.h"
#include "CodeListMgr.h"
#include "FuncLib.h"
#include "GlobalSetting.h"
#include "Markup.h"
#include "GWMessage.h"
#include <algorithm>
#include <gwconst.h>
#include <toolclass.h>

const TCHAR SHORTKEY_CONFIG_FILENAME[]		= _T("shortkey.xml");

const TCHAR NODE_SHOTRKEY[]					= _T("shortkey");

const TCHAR ATTRIBUTE_KEY[]					= _T("key");
const TCHAR ATTRIBUTE_CMD[]					= _T("cmd");
const TCHAR ATTRIBUTE_MARKET[]				= _T("market");
const TCHAR ATTRIBUTE_CODE[]				= _T("code");

CodeListMgr* GetCodeListMgr()
{
	return CodeListMgr::GetInstance();
}

struct WizzardObjCmp
{
	BOOL operator ()  (GW_WIZZARD_OBJ* pObjA, GW_WIZZARD_OBJ* pObjB) 
	{
		if (pObjA->mainpriority == pObjB->mainpriority)
		{
			if (pObjA->subpriority == pObjB->subpriority)		// 两个匹配程度都一样，比字符长度
			{
				if (pObjA->subpriority == PRIORITY_KEY)
				{
					return pObjA->key.GetLength() < pObjB->key.GetLength();
				}
				else
				{
					return pObjA->name.GetLength() < pObjB->name.GetLength();
				}
			}

			return pObjA->subpriority < pObjB->subpriority;
		}

		return pObjA->mainpriority < pObjB->mainpriority;
	}
};

CodeListMgr::~CodeListMgr(void)
{
	for_each(m_lstShortcutObj.begin(), m_lstShortcutObj.end(), ObjPtrClean());
	m_lstShortcutObj.clear();

	for_each(m_lstSymbolObj.begin(), m_lstSymbolObj.end(), ObjPtrClean());
	m_lstSymbolObj.clear();

	for_each(m_lstMarket.begin(), m_lstMarket.end(), ObjPtrClean());
	m_lstMarket.clear();
}

void CodeListMgr::UpdateSymbol(SymbolObj* pSymbolObj)
{
	ASSERT(pSymbolObj != NULL);

	SymbolObj* pExistSymbolObj = FindSymbolObj(pSymbolObj->GetMarket(), pSymbolObj->GetCode());

	if (pExistSymbolObj == NULL)		// 不存在，走新增
	{
		AddSymbol(pSymbolObj);
	}
	else
	{
		pExistSymbolObj->SetRight(0);
		pExistSymbolObj->dbLastClose	= pSymbolObj->dbLastClose;
		pExistSymbolObj->bLastCloseInit = pSymbolObj->bLastCloseInit; 
		pExistSymbolObj->dbLastSettlement = pSymbolObj->dbLastSettlement;
		memcpy(pExistSymbolObj->nKVerArr, pSymbolObj->nKVerArr, sizeof(pExistSymbolObj->nKVerArr));
		VECTOR_MERGE(pExistSymbolObj->vecTradeTime, pSymbolObj->vecTradeTime, TRUE);
		VECTOR_MERGE(pExistSymbolObj->vecTradeTimeFull, pSymbolObj->vecTradeTimeFull, TRUE);
		WriteDebugMessage("更新商品[%s]信息成功", pSymbolObj->GetObject());
		delete pSymbolObj;			// 别忘了删除
	}
}

void CodeListMgr::AddSymbol(SymbolObj* pSymbolObj)
{
	ASSERT(pSymbolObj != NULL);
	
	if (pSymbolObj)
	{
		MarketInfo* pMarket = FindMarket(pSymbolObj->GetMarket());

		if (pMarket == NULL)
		{
			WriteLogMessage(LogFull, _T("接收到未知市场代码[%s]"), pSymbolObj->GetObject());
			delete pSymbolObj;
			return;
		}

		pSymbolObj->pMarketIfo	= pMarket;

		pMarket->vecSymbols.push_back(pSymbolObj);

		GW_WIZZARD_OBJ *obj    = new GW_WIZZARD_OBJ;
		obj->type			   = GW_COMMODITY;

		obj->exdata.pSymbolObj = pSymbolObj;

		obj->key = pSymbolObj->GetCode();
		obj->name = pSymbolObj->GetName();
		obj->shortkey = pSymbolObj->GetKeyWords();

		m_lstSymbolObj.push_back(obj);									// 加入到全局股票对象
		WriteDebugMessage("新增商品[%s]信息成功", pSymbolObj->GetObject());
	}
}

void CodeListMgr::GetMarketSymbols(const CString& strMarket, SYMBOLVEC& vecSymbol, BOOL bOnlyTopLevelSymbol)
{
	MarketInfo* pMarket = FindMarket(strMarket);

	if (pMarket == NULL)
	{
		WriteDebugMessage("找不到市场[%s]", strMarket);
		return;
	}

	if (bOnlyTopLevelSymbol)
	{
		//remove_copy_if(pMarket->vecSymbols.begin(), pMarket->vecSymbols.end(), vecSymbol.begin(), mem_fun(&SymbolObj::HasParent));
		remove_copy_if(pMarket->vecSymbols.begin(), pMarket->vecSymbols.end(), back_inserter(vecSymbol), mem_fun(&SymbolObj::HasParent)); 
	}
	else
	{
		vecSymbol = pMarket->vecSymbols;
	}
	
}

void CodeListMgr::GetAllMarketNames(CSTRINGVEC& vecName)
{
	for (MARKETLSTITR itr = m_lstMarket.begin(); itr != m_lstMarket.end(); ++itr)
	{
		if (!(*itr)->bOmit)
		{
#if defined(VERSION_TAG_TT) 
			if ((*itr)->strMarketName == "TPME" )  //广东贵金属现货延期
			continue;
#endif
			vecName.push_back((*itr)->strMarketName);
		}
	}

}

void CodeListMgr::GetAllMarket(MRKETVEC& vecMarket)
{
	for (MARKETLSTITR itr = m_lstMarket.begin(); itr != m_lstMarket.end(); ++itr)
	{
		if (!(*itr)->bOmit)
		{
			vecMarket.push_back(*itr);
		}
	}
}


SymbolObj* CodeListMgr::FindSymbolObj(const CString& strMarket, const CString &strCode)
{
	MarketInfo* pMarket = FindMarket(strMarket);

	if (pMarket == NULL)
	{
		return NULL;
	}

	for (SYMBOLVECITR itr2 = pMarket->vecSymbols.begin(); itr2 != pMarket->vecSymbols.end(); ++itr2)
	{
		if ((*itr2)->strCode == strCode)
		{
			return *itr2;
		}
	}

	return NULL;
}

MarketInfo* CodeListMgr::FindMarket(const CString& strMarket)
{
	if(m_mapMarket.empty())
	{
		return NULL;
	}
	MARKETMAPITR itr = m_mapMarket.find(strMarket);

	if (itr == m_mapMarket.end())
	{
		return NULL;
	}

	return itr->second;
}

void CodeListMgr::AddMarket(MarketInfo* pMarket)
{
	if (!CheckMarketValid(pMarket))
	{
		return;
	}

	MARKETMAPITR itr = m_mapMarket.find(pMarket->strMarketName);

	if (itr == m_mapMarket.end())
	{
		WriteDebugMessage("初始化市场[%s]，市场当前交易日期[%d]", pMarket->strMarketName, pMarket->nTradeDate);

		m_mapMarket.insert(MARKETMAP::value_type(pMarket->strMarketName, pMarket));
		m_lstMarket.push_back(pMarket);
	}
	else
	{
		// 更新市场时间
			itr->second->nLastTradeDate = itr->second->nTradeDate;//保存上一个交易日信息
			itr->second->nTradeDate = pMarket->nTradeDate;
			if(pMarket->vecTradeTime.size() > 0)
			{
				VECTOR_MERGE(itr->second->vecTradeTime, pMarket->vecTradeTime, TRUE);
			}
			if(pMarket->vecTradeTimeFull.size() > 0)
			{
				VECTOR_MERGE(itr->second->vecTradeTimeFull, pMarket->vecTradeTimeFull, TRUE);
			} 

			for (SYMBOLVECITR itr2 = itr->second->vecSymbols.begin(); itr2 != itr->second->vecSymbols.end(); ++itr2)
			{
				SymbolObj* pSymbolObj = *itr2;
				pSymbolObj->snapshotData.dbVolume.SetValue((__int64)0, pSymbolObj->snapshotData.dbVolume.GetDigit());
				pSymbolObj->snapshotData.dbAmount.SetValue((__int64)0, pSymbolObj->snapshotData.dbAmount.GetDigit());
				pSymbolObj->snapshotData.dbVolumeNow.SetValue((__int64)0, pSymbolObj->snapshotData.dbVolumeNow.GetDigit());
				pSymbolObj->snapshotData.dbRealVolumeNow.SetValue((__int64)0, pSymbolObj->snapshotData.dbRealVolumeNow.GetDigit());
				pSymbolObj->snapshotData.dbAmountNow.SetValue((__int64)0, pSymbolObj->snapshotData.dbAmountNow.GetDigit());
			}

		WriteDebugMessage("更新市场[%s]信息完毕，市场当前交易日期[%d]", itr->second->strMarketName, itr->second->nTradeDate);

		delete pMarket;			// 回收资源
	}	

}

BOOL CodeListMgr::LoadLocalConfig()
{
	CreateSelfStockMarket();		// 创建自选股市场
	LoadSelfStockFromFile();		// 加载本地自选股配置

	return LoadShortcutFromFile(GetSysShortcutFileName()) && LoadShortcutFromFile(GetUserShortcutFileName());
}

const CString CodeListMgr::GetSysShortcutFileName()
{
	CString strDir = GlobalSetting::GetInstance()->GetGFSubDir(GDT_CFG);
	GW_PathFile::EnsureDirExist(strDir);
	return strDir + SHORTKEY_CONFIG_FILENAME;
}

const CString CodeListMgr::GetUserShortcutFileName()
{
	CString strDir = GlobalSetting::GetInstance()->GetGFSubDir(GDT_USERDATA);
	GW_PathFile::EnsureDirExist(strDir);

	return strDir + SHORTKEY_CONFIG_FILENAME;
}

BOOL CodeListMgr::LoadShortcutFromFile(const CString& strFileName)
{
	CMarkup xmlDoc;

	if (!xmlDoc.Load(strFileName))
	{
		WriteLogMessage(LogWarning, _T("加载配置文件[%s]失败,%s"), strFileName, xmlDoc.GetError());
		return FALSE;
	}

	xmlDoc.ResetMainPos();


	while (xmlDoc.FindChildElem(NODE_SHOTRKEY))
	{
		GW_WIZZARD_OBJ *pWizzardObj = new GW_WIZZARD_OBJ;

		try
		{
			pWizzardObj->type		= GW_COMMAND;

			pWizzardObj->key = xmlDoc.GetChildAttrib(ATTRIBUTE_KEY);
			pWizzardObj->name = xmlDoc.GetChildAttrib(_T("name"));

			pWizzardObj->key = pWizzardObj->key.MakeUpper();
			pWizzardObj->name = pWizzardObj->name.MakeUpper();

			// 读取快捷键配置
			ASSERT (!xmlDoc.GetChildAttrib(ATTRIBUTE_CMD).IsEmpty());
			pWizzardObj->exdata.pShortcutObj = new ShortcutObj;
			pWizzardObj->exdata.pShortcutObj->command = xmlDoc.GetChildAttrib(ATTRIBUTE_CMD);

			m_lstShortcutObj.push_back(pWizzardObj);
		}
		catch(...)
		{
			delete pWizzardObj;
			WriteDebugMessage(xmlDoc.GetError());
		}
	}

#ifdef _DEBUG
	GW_WIZZARD_OBJ *pWizzardObj = new GW_WIZZARD_OBJ;

	pWizzardObj->type		= GW_COMMAND;

	pWizzardObj->key = _T("LAYOUT1");
	pWizzardObj->name = _T("可分割框架");

	pWizzardObj->exdata.pShortcutObj = new ShortcutObj;
	pWizzardObj->exdata.pShortcutObj->command = _T("loadview?view=SplitterFrame;layout=layout1");

	m_lstShortcutObj.push_back(pWizzardObj);
#endif
#if defined(VERSION_TAG_YG) || defined(_DEBUG) 
	{
	GW_WIZZARD_OBJ *pWizzardObj = new GW_WIZZARD_OBJ;

	pWizzardObj->type		= GW_COMMAND;

	pWizzardObj->key = _T("SYSINFO");
	pWizzardObj->name = _T("系统信息");

	pWizzardObj->exdata.pShortcutObj = new ShortcutObj;
	pWizzardObj->exdata.pShortcutObj->command = _T("funccall?funcname=ShowSystemInfo");

	m_lstShortcutObj.push_back(pWizzardObj);
	}
#endif
	return TRUE;
}

void CodeListMgr::KeyworkFilter(const CString& strText, WIZZARDOBJLST& lstMatch, DWORD dwMask)
{
	if (dwMask & WIZZARD_SHOW_SHORKEY)
	{
		for (WIZZARDOBJLSTITR itr = m_lstShortcutObj.begin(); itr != m_lstShortcutObj.end(); ++itr)
		{
			GW_WIZZARD_OBJ *pObj = *itr;

			UINT nMainPriority = (UINT)pObj->key.Find(strText);

			pObj->mainpriority = nMainPriority;
			pObj->subpriority  = PRIORITY_KEY;

			if (nMainPriority == 0)		// key以欲查找对象开头，直接满足条件
			{
				lstMatch.push_back(*itr);
				continue;
			}

			nMainPriority = (UINT)pObj->name.Find(strText);

			if (nMainPriority < pObj->mainpriority)
			{
				pObj->mainpriority = nMainPriority;
				pObj->subpriority  = PRIORITY_NAME;
			}

			if (pObj->mainpriority != (UINT)-1)
			{
				lstMatch.push_back(*itr);
			}
		}
	}

	if (dwMask & WIZZARD_SHOW_SYMBOL)
	{
		for (WIZZARDOBJLSTITR itr = m_lstSymbolObj.begin(); itr != m_lstSymbolObj.end(); ++itr)
		{
			GW_WIZZARD_OBJ *pObj = *itr;
			
			if (pObj->exdata.pSymbolObj && pObj->exdata.pSymbolObj->pMarketIfo && pObj->exdata.pSymbolObj->pMarketIfo->bOmit)
			{
				continue;
			}

			UINT nMainPriority = (UINT)pObj->key.Find(strText);

			pObj->mainpriority = nMainPriority;
			pObj->subpriority  = PRIORITY_KEY;

			if (nMainPriority == 0)		// key以欲查找对象开头，直接满足条件
			{
				lstMatch.push_back(*itr);
				continue;
			}

			nMainPriority = (UINT)pObj->shortkey.Find(strText);

			if (nMainPriority < pObj->mainpriority)
			{
				pObj->mainpriority = nMainPriority;
				pObj->subpriority  = PRIORITY_NAME;
			}

			if (pObj->mainpriority != (UINT)-1)
			{
				lstMatch.push_back(*itr);
			}
		}
	}

	lstMatch.sort(WizzardObjCmp());
}

BOOL CodeListMgr::LoadSelfStockFromFile()
{
	CString strFileName = GetSelfStockFileName();

	if (!GW_PathFile::IsPathFileExist(strFileName))
		strFileName =  GlobalSetting::GetInstance()->GetGFSubDir(GDT_CFG) + _T("selfstock.xml");

	SYMBOLVEC& vecSelfStock = GetMarketSymbolVec(MARKET_SELFSTOCK);
	vecSelfStock.reserve(50);

	CMarkup xmlDoc;

	if (!xmlDoc.Load(strFileName))
	{
		WriteLogMessage(LogWarning, _T("加载自选股文件[%s]失败,%s"), strFileName, xmlDoc.GetError());
		return FALSE;
	}

	while (xmlDoc.FindChildElem(_T("symbol")))
	{
		try
		{
			CString strMarket = xmlDoc.GetChildAttrib(ATTRIBUTE_MARKET);
			CString strCode	  = xmlDoc.GetChildAttrib(ATTRIBUTE_CODE);

			if(strMarket == MARKET_TPME)
				continue;

			SymbolObj* pSymbolObj = FindSymbolObj(strMarket, strCode);
			
			if (!pSymbolObj)
			{
				WriteDebugMessage("未找到自选股[%s.%s]", strMarket, strCode);
				continue;
			}

			pSymbolObj->bSelfSotck = TRUE;
			vecSelfStock.push_back(pSymbolObj);
		}
		catch(...)
		{
			WriteDebugMessage(xmlDoc.GetError());
		}
	}

	return TRUE;
}

void CodeListMgr::SaveSelfStockToFile()
{
	CMarkup xmlDoc;

	xmlDoc.SetDoc("<?xml version=\"1.0\" encoding=\"gb2312\" ?>\r\n");

	xmlDoc.AddElem(_T("root"));
	xmlDoc.IntoElem();

	SYMBOLVEC& vecSelfStock = GetMarketSymbolVec(MARKET_SELFSTOCK);

	for (SYMBOLVECITR itr = vecSelfStock.begin(); itr != vecSelfStock.end(); ++itr)
	{
		SymbolObj* pSymbol = *itr;

		xmlDoc.AddElem(_T("symbol"));
		xmlDoc.AddAttrib(ATTRIBUTE_MARKET, pSymbol->GetMarket());
		xmlDoc.AddAttrib(ATTRIBUTE_CODE, pSymbol->GetCode());
	}
	
	xmlDoc.Save(GetSelfStockFileName());
}

const CString CodeListMgr::GetSelfStockFileName()
{
	return GlobalSetting::GetInstance()->GetGFSubDir(GDT_USERDATA) + _T("selfstock.xml");
}

BOOL CodeListMgr::ReplaceSelfStock(SYMBOLVEC vecSymbol)
{
	SYMBOLVEC& vecSelfStock = GetMarketSymbolVec(MARKET_SELFSTOCK);
	if(vecSelfStock.size()>0)
	{
		for (vector<SymbolObj*>::iterator it = vecSelfStock.begin(); it != vecSelfStock.end();it++) 
		{
			(*it)->bSelfSotck = FALSE;
		}
	}

	vecSelfStock.clear();
	if(vecSymbol.size()>0)
	{
		for (vector<SymbolObj*>::iterator it = vecSymbol.begin(); it != vecSymbol.end();it++) 
		{
			(*it)->bSelfSotck = FALSE;
			AddSelfStock(*it);
		}
	}
	GW_Common::BroadcastMessage(GW_Common::GetActiveFrameWnd()->m_hWnd,WM_MSG_SELFSTOCK_CHANGE,NULL,NULL);
	SaveSelfStockToFile();
	return TRUE;
}

BOOL CodeListMgr::AddSelfStock(SymbolObj* pSymbolObj)
{
	if (pSymbolObj->bSelfSotck)		// 已经是自选股了
	{
		return FALSE;
	}

	pSymbolObj->bSelfSotck = TRUE;
	GetMarketSymbolVec(MARKET_SELFSTOCK).push_back(pSymbolObj);
	
	return TRUE;
}

BOOL CodeListMgr::RemoveSelfStock(SymbolObj* pSymbolObj)
{
	if (!pSymbolObj->bSelfSotck)	// 不是自选股
	{
		return FALSE;
	}

	pSymbolObj->bSelfSotck = FALSE;

	SYMBOLVEC& vecSelfStock = GetMarketSymbolVec(MARKET_SELFSTOCK);

	SYMBOLVECITR itr = find(vecSelfStock.begin(), vecSelfStock.end(), pSymbolObj);
	ASSERT(itr != vecSelfStock.end());

	if (itr != vecSelfStock.end())
	{
		vecSelfStock.erase(itr);
		return TRUE;
	}
	
	return FALSE;
}

BOOL CodeListMgr::MoveSelfStockPostion(SymbolObj* pSymbolObj, BOOL bMoveDown)
{
	if (!pSymbolObj->bSelfSotck)	// 不是自选股
	{
		return FALSE;
	}

	SYMBOLVEC& vecSelfStock = GetMarketSymbolVec(MARKET_SELFSTOCK);

	if (vecSelfStock.size() <= 1)		// 没有必要调整	
	{
		return TRUE;
	}

	SYMBOLVECITR itr = find(vecSelfStock.begin(), vecSelfStock.end(), pSymbolObj);
	ASSERT(itr != vecSelfStock.end());

	if (itr == vecSelfStock.end())
	{
		return FALSE;
	}

	UINT nIndex = distance(vecSelfStock.begin(), itr);

	if (bMoveDown)	// 向后交换
	{	
		if (itr == (vecSelfStock.end() -1))		// 没有必要调整
		{
			return TRUE;
		}

		vecSelfStock[nIndex] = vecSelfStock[nIndex + 1];
		vecSelfStock[nIndex + 1] = pSymbolObj;
	}
	else	// 向前交换
	{
		if (itr == vecSelfStock.begin())		// 没有必要调整
		{
			return TRUE;
		}

		vecSelfStock[nIndex] = vecSelfStock[nIndex - 1];
		vecSelfStock[nIndex - 1] = pSymbolObj;
	}

	return TRUE;
}

void CodeListMgr::CreateSelfStockMarket()
{
	MarketInfo* pMarket = new MarketInfo;
	pMarket->bVirtual	= TRUE;

	pMarket->strMarketName = MARKET_SELFSTOCK;
	pMarket->strCName	= "自选品种";
	pMarket->bOmit		= FALSE;

	CTime tm = CTime::GetCurrentTime();
	pMarket->nTradeDate = tm.GetYear() * 10000 + tm.GetMonth() * 100 + tm.GetDay();

	AddMarket(pMarket);
}

template <class T>SymbolObj* FindNextSymbolImpl(SymbolObj* pSymbolObj, T begin, T end)
{
	T t = find(begin, end, pSymbolObj);

	if (t == end)
	{
		return NULL;
	}

	++t;

	if (t == end)
	{
		t = begin;
	}

	return *t;
};

SymbolObj* CodeListMgr::GetNextSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket)
{
	BOOL bUseSelfVec = FALSE;

	if (pSymbolObj->bSelfSotck && strMarket == MARKET_SELFSTOCK)	// 自选股
	{
		bUseSelfVec = TRUE;;
	}

	SYMBOLVEC &vec = m_vecActiveSymbols.empty() ? (bUseSelfVec ? GetMarketSymbolVec(MARKET_SELFSTOCK) : pSymbolObj->pMarketIfo->vecSymbols) : m_vecActiveSymbols;

	SymbolObj* pNext = FindNextSymbolImpl(pSymbolObj, vec.begin(), vec.end());

	if (pNext == NULL && !m_vecActiveSymbols.empty())
	{
		pNext = FindNextSymbolImpl(pSymbolObj, pSymbolObj->pMarketIfo->vecSymbols.begin(), pSymbolObj->pMarketIfo->vecSymbols.end());

	}

	return pNext ? pNext : pSymbolObj;
}

SymbolObj* CodeListMgr::GetPreviousSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket)
{
	BOOL bUseSelfVec = FALSE;

	if (pSymbolObj->bSelfSotck && strMarket == MARKET_SELFSTOCK)	// 自选股
	{
		bUseSelfVec = TRUE;;
	}

	SYMBOLVEC &vec = m_vecActiveSymbols.empty() ? (bUseSelfVec ? GetMarketSymbolVec(MARKET_SELFSTOCK) : pSymbolObj->pMarketIfo->vecSymbols) : m_vecActiveSymbols;

	SymbolObj* pNext = FindNextSymbolImpl(pSymbolObj, vec.rbegin(), vec.rend());

	if (pNext == NULL && !m_vecActiveSymbols.empty())
	{
		pNext = FindNextSymbolImpl(pSymbolObj, pSymbolObj->pMarketIfo->vecSymbols.rbegin(), pSymbolObj->pMarketIfo->vecSymbols.rend());

	}

	return pNext ? pNext : pSymbolObj;
}

void CodeListMgr::DoMarketAdjust()
{
	CString strMarket = GetGloablSetting()->ToString(GW_FIRSTMARKETKEY);

	if (strMarket.IsEmpty())
	{
		return;
	}

	MarketInfo* pMarket = FindMarket(strMarket);
	ASSERT(pMarket != NULL);

	if (pMarket == NULL)
	{
		return;
	}

	MARKETLSTITR itr = find(m_lstMarket.begin(), m_lstMarket.end(), pMarket);
	ASSERT(itr != m_lstMarket.end());

	if (itr == m_lstMarket.begin() || itr == m_lstMarket.end())			// 在头部或者没有不需要实施交换
	{
		return;
	}

	m_lstMarket.erase(itr);
	m_lstMarket.push_front(pMarket);
}

void CodeListMgr::GetMarketSymbolLoopVector(SYMBOLVEC& vecMarketSymbol, const CString& strMarket, SymbolObj* pStartSymbolObj, UINT nMaxCount, BOOL bLoop)
{
	if (strMarket.IsEmpty())
	{
		return;
	}

	SYMBOLVEC& vecSymbol = GetMarketSymbolVec(strMarket);
	
	if (vecSymbol.empty())
	{
		return;
	}

	if (pStartSymbolObj == NULL)		// 没有指定商品，从该市场第一个开始
	{
		vecMarketSymbol.insert(vecMarketSymbol.begin(), vecSymbol.begin(), (vecSymbol.size() > nMaxCount) ? vecSymbol.begin() + nMaxCount : vecSymbol.end());	
		return;
	}
	else	// 有指定的商品，从指定商品开始拷贝
	{
		SYMBOLVECITR itr = find(vecSymbol.begin(), vecSymbol.end(), pStartSymbolObj);
		ASSERT(itr != vecSymbol.end());
		
		if (itr == vecSymbol.end())
		{
			vecMarketSymbol.insert(vecMarketSymbol.begin(), vecSymbol.begin(), (vecSymbol.size() > nMaxCount) ? vecSymbol.begin() + nMaxCount : vecSymbol.end());	
			return;
		}

		size_t nTotal = vecSymbol.size();
		size_t nPos	  = distance(vecSymbol.begin(), itr);
		size_t nCopyCount = 0;

		if (nTotal - nPos >= nMaxCount)		// 数量多多，随便拷贝
		{
			vecMarketSymbol.insert(vecMarketSymbol.begin(), itr, itr + nMaxCount);	
		}
		else	// 数量不够	
		{
			vecMarketSymbol.insert(vecMarketSymbol.begin(), itr, vecSymbol.end());	
			
			if (bLoop)		// 需要从头循环一下
			{
				nCopyCount = nTotal - nPos;
				nTotal -= nCopyCount;
				nMaxCount -= nCopyCount;

				if (nTotal >= nMaxCount)	
				{
					vecMarketSymbol.insert(vecMarketSymbol.end(), vecSymbol.begin(), vecSymbol.begin() + nMaxCount);	
				}
				else
				{
					vecMarketSymbol.insert(vecMarketSymbol.end(), vecSymbol.begin(), itr);	
				}
			}
		}
	}
}

SYMBOLVEC& CodeListMgr::GetMarketSymbolVec(const CString& strMarketName)
{
	MarketInfo* pMarket = FindMarket(strMarketName);

	if (pMarket)
	{
		return pMarket->vecSymbols;
	}

	static SYMBOLVEC vec;
	return vec;
}

void CodeListMgr::SetActiveSymbolObj(SymbolObj* pSymbolObj)
{
	m_pActiveSymbol = pSymbolObj;
}

void CodeListMgr::SetActiveMarket(MarketInfo* pMarket)
{
	m_pActiveMarket = pMarket;

	m_vecActiveSymbols.clear();
}

void CodeListMgr::SetActiveMarket(const CString& strMarket)
{
	SetActiveMarket(FindMarket(strMarket));
}

MarketInfo* CodeListMgr::GetActiveMarket() 
{
	if (m_pActiveMarket == NULL)
	{
		m_pActiveMarket = m_lstMarket.front();
	}

	return m_pActiveMarket;
}

SymbolObj* CodeListMgr::GetActiveSymbolObj() 
{
	if (m_pActiveSymbol == NULL)
	{
		if (m_pActiveMarket)
		{
			m_pActiveSymbol = m_pActiveMarket->vecSymbols.front();
		}
	}

	return m_pActiveSymbol;
}

BOOL CodeListMgr::CheckMarketValid(MarketInfo* pMarket)
{
	ASSERT (pMarket != NULL);

	if (pMarket == NULL)
	{
		return FALSE;
	}

	for (CSTRINGVECITR itr = m_vecOmitMarket.begin(); itr != m_vecOmitMarket.end(); ++itr)
	{
		if (pMarket->strMarketName.CompareNoCase(*itr) == 0)
		{
			pMarket->bOmit = TRUE;
			WriteLogMessage(LogFull, _T("市场[%s]被标记为已忽略"), pMarket->strMarketName);
		}
	}

	return TRUE;
}

void CodeListMgr::InitInstance()
{
	const CString strOmitMarkets = GetGloablSetting()->ToString(GW_OMITMARKETKEY);

	if (!strOmitMarkets.IsEmpty())
	{
		stringsplitter sp(_T("|"));
		sp.split(strOmitMarkets);

		m_vecOmitMarket.swap(sp.getdata());
	}
}

void CodeListMgr::MakeRelationship()
{
	for (MARKETLSTITR itr = m_lstMarket.begin(); itr != m_lstMarket.end(); ++itr)
	{
		MarketInfo* pMarket = *itr;

		if (pMarket->bOmit)
		{
			continue;
		}

		SymbolObj *pLastParent = NULL;

		for (SYMBOLVECITR itr2 = pMarket->vecSymbols.begin(); itr2 != pMarket->vecSymbols.end(); ++itr2)
		{
 			SymbolObj* pSymbol = *itr2;
 
			if (!pSymbol->HasParent())
 			{
 				continue;
			}
			
			stringsplitter sp(_T(";"));
			sp.split(pSymbol->strParent);

			for (CSTRINGVECITR itr3 = sp.begin(); itr3 != sp.end(); ++itr3)
			{
				pLastParent = GW_HQ::FindSymbolObjEx(*itr3);

				if (pLastParent == NULL)
				{
					WriteDebugMessage("%s 无法找到父节点 %s", pSymbol->GetObject(), *itr3); 
					pSymbol->strParent.Empty();
					continue;
				}

				pLastParent->vecChildren.push_back(pSymbol->GetObject());
#ifdef _DEBUG
				WriteDebugMessage("%s 匹配到父节点 %s", pSymbol->GetObject(), *itr3); 
#endif
			}
		}

	}


}


void CodeListMgr::MakeSymbolRight(GWSYMBOLRIGHTVEC& vecSymbolRight)
{
	int nSymbolNum = vecSymbolRight.size();
	for(int ii = 0; ii < nSymbolNum; ii ++)
	{
		GWSymbolRight & tOneItem = vecSymbolRight[ii]; 
		SymbolObj* pSymbol = this->FindSymbolObj(tOneItem.szMarket, tOneItem.szSymbol);
		if(NULL == pSymbol)
		{
			ASSERT(0);
			continue;
		}
		pSymbol->SetRight(tOneItem.dwRight);
	}
}

void CodeListMgr::SetActiveSymbolsVec(SYMBOLVEC& vecSymbols)
{
	m_vecActiveSymbols = vecSymbols;
}

