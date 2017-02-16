#include "StdAfx.h"
#include "GlobalSetting.h"
#include <io.h>

#include "Markup.h"

#include <json/json.h>
#include "FuncLib.h" 

const char DRIVE_IDENTIFICATION = _T(':');					// 盘符标识符
const char PATH_SPLIT_CHAR_W	= _T('\\');					// window风格路径分割符
const char PATH_SPLIT_CHAR_U	= _T('/');					// LINUX风格路径分割符

const char SUB_DIR_DEALDATA[]   = "deal\\";					// 逐笔文件目录
const char SUB_DIR_MINUTEDATA[] = "minute\\";				// 走势图文件目录
const char SUB_DIR_KLINEDATA[] =  "kline\\";				// K线文件目录
const char SUB_DIR_MINEDATA[] = "mine\\";					// 信息地雷目录
const char SUB_DIR_SERVERTADATA[] = "ServerTA\\";					// 信息地雷目录
const char SUB_DIR_BIGNEWSDATA[] = "News\\";					// 信息地雷目录

#define IS_PATH_SPLIT_CHAR(ch)	(((ch) == PATH_SPLIT_CHAR_W) || ((ch) == PATH_SPLIT_CHAR_U))			// 字符是否路径分割符

// 系统各级目录，注意以\\或/结尾
const TCHAR GF_DIR_TYPE_STRING[][16] = { 
										_T("cfg\\"), _T("userdata\\"), _T("update\\"), _T("data\\"), _T("res\\"), 
										_T("log\\"), _T("bakup\\"), _T("plugin\\"), _T("config\\"), _T("tmp\\"),
										_T("script\\")
										};

using namespace GW_PathFile;
;
GlobalSetting* GetGloablSetting()
{
	return GlobalSetting::GetInstance();
}

GlobalSetting::GlobalSetting(void)
{
	LoadLocalConfig();

	ReadSettings();
	ReadVerFile();
	//SetUIMode(_T("default"));
	ReadSkinFile();
	m_nCurrentSkinIndex = 0;
	m_strResDir = GetMainDir() + GF_DIR_TYPE_STRING[GDT_RES];
}


GlobalSetting::~GlobalSetting(void)
{
	FreeAllSkin();
	SaveSettings();
}

GlobalSetting* GlobalSetting::GetInstance()
{
	static GlobalSetting global;
	return &global;
}

int GlobalSetting::SetRunWithWinStart(BOOL bWithWinStart /*= TRUE*/)
{
	int nRet(0);
#ifndef VERSION_TAG_YG
	CString strKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	CString strItem = _T("GWStation");
	CString strItemValue = GetMainDir() + _T("GWStation.exe");
	HKEY hKey(NULL);
	BOOL bFindKey = (BOOL)(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKey, NULL, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS);

	DWORD state(REG_CREATED_NEW_KEY);
	if (bWithWinStart)  // 随windows自动启动
	{
		if (bFindKey == FALSE)
		{
			long lValue = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, strKey, 0, NULL, 0, 0, NULL, &hKey, &state);
			if (lValue == ERROR_SUCCESS)
				bFindKey = (BOOL)(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKey, NULL, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS);
			else
			{
				WriteLogMessage(LogError,  _T("自启动Run创建失败[%d]"), lValue);
				nRet = -1; //  创建注册表项失败
			}
		}

		if (bFindKey)
		{
			long lValue = ::RegSetValueEx(hKey, strItem, 0, REG_SZ, (BYTE *)((LPCTSTR)strItemValue), strItemValue.GetLength()) ;
			nRet = (lValue == ERROR_SUCCESS) ? 0 : -1;
			if (!nRet)
				WriteLogMessage(LogError,  _T("开机自启动[%d]"), lValue);
		}	
	}
	else				// 不随windows自动启动
	{
		if (bFindKey)
		{
			long lValue =::RegDeleteValue(hKey, strItem) ;
			nRet = (lValue == ERROR_SUCCESS) ? 0 : -1;
			if (!nRet)
				WriteLogMessage(LogError,  _T("取消自启动[%d]"), lValue);
		}
	}
	if (bFindKey)
		::RegCloseKey(hKey);
#endif
	return nRet;
}

const CString GlobalSetting::GetMainDir()
{
	return GetParentPath(GetMainExeName());
}

const CString GlobalSetting::GetGFSubDir(GF_DIR_TYPE type)
{
	if (type == GDT_RES)
	{  
		if(m_strResDir.IsEmpty())
		{
			m_strResDir = GetMainDir() + GF_DIR_TYPE_STRING[GDT_RES];
		}
		return m_strResDir;
	}
	
	CString strDir = GetMainDir() + GF_DIR_TYPE_STRING[type];
	EnsureDirExist(strDir);

	return strDir;
}

const CString GlobalSetting::GetMainExeName()
{
	CString strFileName;

	GetModuleFileName(NULL, strFileName.GetBuffer(1024), 1024);
	strFileName.ReleaseBuffer(); 

	return strFileName;
}

const CString GlobalSetting::GetSystemConfigFileName()
{
	return GetGFSubDir(GDT_CFG) + "loginfo.ini";
}

const CString GlobalSetting::GetDealFileName(SymbolObj* pSymbol, long lDate)
{
	if (lDate == 0)		// 没有指定日期，默认为商品所在市场的当前日期
	{
		ASSERT(pSymbol->pMarketIfo != NULL);
		lDate = pSymbol->pMarketIfo->nTradeDate;
	}

	CString strDir;

	strDir.Format(
				_T("%s%s%d"),
				GetGFSubDir(GDT_DATA),
				SUB_DIR_DEALDATA,
				lDate
				);

	EnsureDirExist(strDir);

	CString strFileName;
	strFileName.Format(_T("%s\\%s.deal"), strDir, pSymbol->GetObject());

	return strFileName;
}

const CString GlobalSetting::GetMinuteFileName(SymbolObj* pSymbol, long lDate)
{
	if (lDate == 0)		// 没有指定日期，默认为商品所在市场的当前日期
	{
		ASSERT(pSymbol->pMarketIfo != NULL);
		lDate = pSymbol->pMarketIfo->nTradeDate;
	}

	CString strDir;

	strDir.Format(
				_T("%s%s%d"),
				GetGFSubDir(GDT_DATA),
				SUB_DIR_MINUTEDATA,
				lDate
				);

	EnsureDirExist(strDir);

	CString strFileName;
	strFileName.Format(_T("%s\\%s.min"), strDir, pSymbol->GetObject());

	return strFileName;
}

const CString GlobalSetting::GetKLineFileName(SymbolObj* pSymbol, GWKLINE_TYPE nType)
{
	ASSERT (nType == KT_1MIN || nType == KT_DAY || nType == KT_15MIN);

	const CString KLINE_FILE_EXT_NAME[] = { "min", "3min", "5min",  "15min", "30min",
											"60min","120min", "180min","240min", _T("unkown"), 
											_T("unkown"),_T("unkown"),_T("unkown"), _T("unkown"), _T("unkown"),
											_T("unkown"), _T("unkown"), _T("unkown"), _T("unkown"), _T("unkown"),
											"Day", "week", "month", "6month", "year" };
	CString strDir;

	strDir.Format(
		_T("%s%s%s"),
		GetGFSubDir(GDT_DATA),
		SUB_DIR_KLINEDATA,
		KLINE_FILE_EXT_NAME[nType]
		);

	EnsureDirExist(strDir);

	CString strFileName;
	strFileName.Format(_T("%s\\%s.%s"), strDir, pSymbol->GetObject(), KLINE_FILE_EXT_NAME[nType]);

	return strFileName;
}
const CString GlobalSetting::GetMinMineFileName(SymbolObj* pSymbol, long lDate)
{
 	if (lDate == 0)		// 没有指定日期，默认为商品所在市场的当前日期
 	{
 		ASSERT(pSymbol->pMarketIfo != NULL);
 		lDate = pSymbol->pMarketIfo->nTradeDate;
 	}
 
 	CString strDir;
 	strDir.Format(
 		_T("%s%s%s%d"),
 		GetGFSubDir(GDT_DATA),
 		SUB_DIR_MINEDATA,
 		_T("day\\"),
 		lDate
 	);
 
 	EnsureDirExist(strDir);
 
 	CString strFileName;
 	strFileName.Format(_T("%s\\%s.bm"), strDir, pSymbol->GetObject());

	return strFileName;
}

const CString GlobalSetting::GetHisMineFileName(SymbolObj* pSymbol)
{
 	CString strDir;
 
 	strDir.Format(
 		_T("%s%s%s"),
 		GetGFSubDir(GDT_DATA),
 		SUB_DIR_MINEDATA,
 		_T("history")
 	);
 
 	EnsureDirExist(strDir);
 
 	CString strFileName;
 	strFileName.Format(_T("%s\\%s.mh"), strDir, pSymbol->GetObject());

	return strFileName;
}


const CString GlobalSetting::GetMinServerTAFileName( SymbolObj* pSymbol, long lDate)
{
	if (lDate == 0)		// 没有指定日期，默认为商品所在市场的当前日期
	{
		ASSERT(pSymbol->pMarketIfo != NULL);
		lDate = pSymbol->pMarketIfo->nTradeDate;
	}

	CString strDir;
	strDir.Format(
		_T("%s%s%s%d"),
		GetGFSubDir(GDT_DATA),
		SUB_DIR_SERVERTADATA,
		_T("day\\"),
		lDate
		);

	EnsureDirExist(strDir);

	CString strFileName;
	//ylx 已经用过，不能继续用
	strFileName.Format(_T("%s\\%s.yle2"), strDir, pSymbol->GetObject());

	return strFileName;

}
const CString GlobalSetting::GetHisServerTAFileName(SymbolObj* pSymbol)
{
	CString strDir;

	strDir.Format(
		_T("%s%s%s"),
		GetGFSubDir(GDT_DATA),
		SUB_DIR_SERVERTADATA,
		_T("history")
		);

	EnsureDirExist(strDir);

	CString strFileName;
	strFileName.Format(_T("%s\\%s.yle2"), strDir, pSymbol->GetObject());

	return strFileName;

}

 const CString GlobalSetting::GetNewsFileName(SymbolObj* pSymbol, long lDate, int nNewsType)
 {
	 if (lDate == 0)		// 没有指定日期，默认为商品所在市场的当前日期
	 {
		 if(pSymbol)
		 {
			 ASSERT(pSymbol->pMarketIfo != NULL);
			 lDate = pSymbol->pMarketIfo->nTradeDate;
		 }
	 }

	 CString strDir;
	 strDir.Format(
		 _T("%s%s%s%d"),
		 GetGFSubDir(GDT_DATA),
		 SUB_DIR_BIGNEWSDATA,
		 _T("day\\"),
		 lDate
		 );

	 EnsureDirExist(strDir);

	 CString strFileName;
	 CString strName(_T("common"));
	 if(pSymbol)
	 {
		 strName = pSymbol->GetObject();
	 }
	 strFileName.Format(_T("%s\\%s_%d.bn"), strDir,(LPCTSTR)strName, nNewsType);

	 return strFileName;
 
 }

const CString GlobalSetting::GeSLSPFileName(SymbolObj* pSymbol, long lDate, int nPeriod)
{

	if (lDate == 0)		// 没有指定日期，默认为商品所在市场的当前日期
	{
		if(pSymbol)
		{
			ASSERT(pSymbol->pMarketIfo != NULL);
			lDate = pSymbol->pMarketIfo->nTradeDate;
		}
	}
	CString strDir;
	strDir.Format(
		_T("%s%s%s%d"),
		GetGFSubDir(GDT_DATA),
		SUB_DIR_MINEDATA,
		_T("day\\"),
		lDate
		);
	EnsureDirExist(strDir);
	CString strFileName;
	strFileName.Format(_T("%s\\%s_%d.sp2"), strDir, pSymbol->GetObject(), nPeriod);
	 return strFileName;
}

const CString GlobalSetting::GeQKSignalFileName(SymbolObj* pSymbol, int nPeriodType, int nSignalKind)
{
	if(NULL == pSymbol)
	{
		ASSERT(0);
		return _T("");
	}
	CString strDir;
	strDir.Format(
		_T("%s%s%s%d"),
		GetGFSubDir(GDT_DATA),
		SUB_DIR_SERVERTADATA,
		_T("history\\"),
		nPeriodType
		);
	EnsureDirExist(strDir);
	CString strFileName;
	strFileName.Format(_T("%s\\%s_%d.qk"), strDir, pSymbol->GetObject(), nSignalKind);
	return strFileName;
}

const CString GlobalSetting::GeBYJZFileName(SymbolObj* pSymbol, int nPeriodType, int nSignalKind)
{
	nSignalKind = 0;
	if(NULL == pSymbol)
	{
		ASSERT(0);
		return _T("");
	}
	CString strDir;
	strDir.Format(
		_T("%s%s%s%d"),
		GetGFSubDir(GDT_DATA),
		SUB_DIR_SERVERTADATA,
		_T("history\\"),
		nPeriodType
		);
	EnsureDirExist(strDir);
	CString strFileName;
	strFileName.Format(_T("%s\\%s_%d.byjz"), strDir, pSymbol->GetObject(), nSignalKind);
	return strFileName;
}

const CString GlobalSetting::GetUserConfigFileName()
{
	return GetGFSubDir(GDT_USERDATA) + _T("selfset.gw");
}

const  CString GlobalSetting::GetResSubDir(CString& strSubDir)
{
	return (this->GetGFSubDir(GDT_RES) + strSubDir);
 }

void GlobalSetting::AddKey(const CString& strKey, const CString& strValue)
{
	CSTRINGMAPITR itr = m_mapSetting.find(strKey);

	if (itr != m_mapSetting.end())
	{
		itr->second = strValue;
	}
	else
	{
		m_mapSetting.insert(itr, CSTRINGMAP::value_type(strKey, strValue));
	}
}

void GlobalSetting::AddKey(const CString& strKey, int nValue)
{
	CString strValue;
	strValue.Format(_T("%d"), nValue);

	AddKey(strKey, strValue);
}

void GlobalSetting::AddKey( const CString& strKey, INT64 nValue )
{
	CString strValue;
	strValue.Format(_T("%I64d"), nValue);

	AddKey(strKey, strValue);
}

void GlobalSetting::AddKey(const CString& strKey, double dbValue)
{
	CString strValue;
	strValue.Format(_T("%f"), dbValue);

	AddKey(strKey, strValue);
}

const CString GlobalSetting::ToString(LPCTSTR lpKey, const CString& strDefault)
{
	CSTRINGMAPITR itr = m_mapSetting.find(lpKey);

	if (itr != m_mapSetting.end())
	{
		return itr->second;
	}

	return strDefault;
}

BOOL GlobalSetting::ToBool(LPCTSTR lpKey, BOOL bDefault)
{
	const CString& strValue = ToString(lpKey);

	if (strValue.IsEmpty())
	{
		return bDefault;
	}

	if (strValue == "0" || strValue.CompareNoCase("false") == 0)
	{
		return FALSE;
	}
	else if (strValue == "1" || strValue.CompareNoCase("true") == 0)
	{
		return TRUE;
	}

	return bDefault;
}

int GlobalSetting::ToInt(LPCTSTR lpKey, int nDefault)
{
	const CString& strValue = ToString(lpKey);

	if (strValue.IsEmpty())
	{
		return nDefault;
	}

	return atoi(strValue);
}


INT64 GlobalSetting::ToInt64( LPCTSTR lpKey, INT64 nDefault /*= 0*/ )
{	
	const CString& strValue = ToString(lpKey);

	if (strValue.IsEmpty())
	{
		return nDefault;
	}

	return _atoi64(strValue);
}


double GlobalSetting::ToDouble(LPCTSTR lpKey, double dbDefault)
{
	const CString& strValue = ToString(lpKey);

	if (strValue.IsEmpty())
	{
		return dbDefault;
	}

	return atof(strValue);
}

const CString GlobalSetting::operator[](LPCTSTR lpKey)
{
	return ToString(lpKey);
}

void GlobalSetting::LoadLocalConfig()
{
	CString strFileName = GetGFSubDir(GDT_CFG) +_T("system.ini");

	CString strMarket = GW_Cfg::GetRegKeyString(strFileName, _T("market"), _T("firstmarket"), MARKET_TPME);
	AddKey(GW_FIRSTMARKETKEY, strMarket);

	CString strAllMarket = GW_Cfg::GetRegKeyString(strFileName, _T("market"), _T("hqmainmarkets"), _T("TPME|SGE|INAU"));
	AddKey(GW_HQ_ALLMARKETS, strAllMarket);

	strMarket =  GW_Cfg::GetRegKeyString(strFileName, _T("market"), _T("omit"));
	AddKey(GW_OMITMARKETKEY, strMarket);
}

struct tagSettingItem
{
	TCHAR szKey[32];	//key
	TCHAR szDefaultVal[32];	//默认值
};
#define DX_ROOMID				_T("DXRoomID")
#define DX_TEACHERNAME			_T("DXTeacheName")
const tagSettingItem szSaveKeyArr[] =
{
	{_T("BottomFuncView"), _T("1")},
	{_T("RadarHisExpand"),_T("0")},
	{_T("EconomicDataID"),_T("0")},
	{_T("FluctuationLine"),_T("1")},
	{_T("NewsShowStatus"),_T("1")},
	{DX_ROOMID , _T("")},
	{DX_TEACHERNAME, _T("")}
};

void GlobalSetting::SaveSettings()
{
	int nKeyNum = sizeof(szSaveKeyArr) / sizeof(szSaveKeyArr[0]);
	CString strCfgFile = this->GetGFSubDir(GDT_USERDATA) + _T("selfset.gw");
	for(int ii = 0; ii < nKeyNum; ii ++)
	{
		CString strVal = this->ToString(szSaveKeyArr[ii].szKey);
		GW_Cfg ::WriteRegKeyString(strCfgFile, _T("GlobalSetting"), szSaveKeyArr[ii].szKey, strVal);
	}
}


void GlobalSetting::ReadSettings()
{
	int nKeyNum = sizeof(szSaveKeyArr) / sizeof(szSaveKeyArr[0]);
	CString strCfgFile = this->GetGFSubDir(GDT_USERDATA) + _T("selfset.gw");
	for(int ii = 0; ii < nKeyNum; ii ++)
	{
		CString strVal = GW_Cfg::GetRegKeyString(strCfgFile, _T("GlobalSetting"), szSaveKeyArr[ii].szKey, szSaveKeyArr[ii].szDefaultVal);
		this->AddKey(szSaveKeyArr[ii].szKey, strVal);
	}

}


void GlobalSetting::ReadVerFile()
{
	CString strVerFile = this->GetGFSubDir(GDT_USERDATA) + _T("VersionInfo.dat");
	if(FALSE == ::PathFileExists((LPCTSTR)strVerFile))
	{
		return;
	}
	try
	{
		m_vecCommonVer.clear();
		CFile cStdFile;
		cStdFile.Open((LPCTSTR)strVerFile, CFile::modeRead);
		int nNum = 0;
		cStdFile.Read(&nNum, sizeof(int));
		if(nNum <= 0)
		{
			cStdFile.Close();
			return;
		}
		m_vecCommonVer.resize(nNum);
		cStdFile.Read(&m_vecCommonVer[0], sizeof(GWItemVer) * nNum);
		cStdFile.Close();
	}
	catch (CMemoryException* )
	{
		
	}
	catch (CFileException* )
	{
	}
	catch (CException* )
	{
	}
}

void GlobalSetting::SaveVerFile()
{
	CString strVerFile = this->GetGFSubDir(GDT_USERDATA) + _T("VersionInfo.dat");
	try
	{
		CFile cStdFile;
		cStdFile.Open((LPCTSTR)strVerFile, CFile::modeReadWrite | CFile::modeCreate);
		int nNum = m_vecCommonVer.size();
		cStdFile.Write(&nNum, sizeof(int)); 
		cStdFile.Write(&m_vecCommonVer[0], sizeof(GWItemVer) * nNum);
		cStdFile.Close();
	}
	catch (CMemoryException* )
	{

	}
	catch (CFileException* )
	{
	}
	catch (CException* )
	{
	}
}

void GlobalSetting::RemvoeLocalDataFile(SymbolObj * pSymbol, BOOL bHisKOrMin)
{
	if(NULL == pSymbol)
	{
		return;
	}
	CString strFile;
	if(bHisKOrMin)
	{
		strFile = this->GetKLineFileName(pSymbol, KT_1MIN);
		GW_PathFile::DeleteFile(strFile);
		strFile = this->GetKLineFileName(pSymbol, KT_15MIN);
		GW_PathFile::DeleteFile(strFile);
	}
	else
	{//日线
		strFile = this->GetKLineFileName(pSymbol, KT_DAY);
		GW_PathFile::DeleteFile(strFile);		
	}
}

void GlobalSetting::RemvoeLocalDataFile(MarketInfo * pMarket, BOOL bHisKOrMin)
{
	if(NULL == pMarket)
	{
		return;
	}
	SYMBOLVEC vecAllSymbol;
	GW_HQ::GetAllMarketSymbols(vecAllSymbol);
	int nSymbolNum = vecAllSymbol.size();
	for(int ii = 0 ; ii < nSymbolNum; ii ++)
	{
		RemvoeLocalDataFile(vecAllSymbol[ii], bHisKOrMin);
	}
}

void GlobalSetting::RemoveLocalDataFile(const GWClearDataCommand & tClearCmd)
{
	int nCmdType = tClearCmd.nCmdType;
	CString strFile;
#if 0
	if(em_KVerDay == nCmdType)
	{//日线
		if(tClearCmd.pSymbol)
		{//清除特定品种
			RemvoeLocalDataFile(tClearCmd.pSymbol, FALSE);
		}
		else if(tClearCmd.pMarket)
		{//清除指定市场
			RemvoeLocalDataFile(tClearCmd.pMarket, FALSE);
		}
	}
	else if(em_KVer1Min == nCmdType )
	{//分钟线
		if(tClearCmd.pSymbol)
		{//清除特定品种
			RemvoeLocalDataFile(tClearCmd.pSymbol, TRUE);
		}
		else if(tClearCmd.pMarket)
		{//清除指定市场
			RemvoeLocalDataFile(tClearCmd.pMarket, TRUE);
		}
	}
#else
	if(tClearCmd.nCmdType < em_VerEnd )
	{//跟品种相关，只需要更新品种对于版本号即可，因为重新请求数据，会与本地文件比较版本号
		int kk = 0;
	}
#endif
	else
	{//品种无关的本地数据
		strFile = GetNewsFileName(NULL, 0, tClearCmd.nCmdType);
		GW_PathFile::DeleteFile(strFile);
	} 

}

int GlobalSetting::GetVerDataNum()
{
	int nNum = m_vecCommonVer.size();
	return nNum;
}

GWItemVer * GlobalSetting::FindVerDataByType(int nType)
{
	int nNum = m_vecCommonVer.size();
	for(int ii = 0; ii < nNum; ii ++)
	{
		if(nType == m_vecCommonVer[ii].nType)
		{
			return &m_vecCommonVer[ii];
		}
	}
	return NULL;
}

int GlobalSetting::IsTypeExist(int nType)
{
	int nNum = m_vecCommonVer.size();
	for(int ii = 0; ii < nNum; ii ++)
	{
		if(nType == m_vecCommonVer[ii].nType)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void GlobalSetting::AddTypeInVerData(const GWItemVer& tItemVer)
{
	int nNum = m_vecCommonVer.size();
	for(int ii = 0; ii < nNum; ii ++)
	{
		if(tItemVer.nType == m_vecCommonVer[ii].nType)
		{//update
			m_vecCommonVer[ii] = tItemVer;
			return ;
		}
	}
	//add
	m_vecCommonVer.push_back(tItemVer);

}



DWORD GlobalSetting::GetMaxNewsCountFromType(int nNewsType)
{
	DWORD dwDefaultCount = 50;	//默认为50
	switch (nNewsType)
	{
	case em_Scroll_RSS:	//滚动资讯
		dwDefaultCount = 10;
		break;
	case em_FastNews:	//快讯
		dwDefaultCount = 2000;
		break;
	case em_Today_Headline:
	case em_Central_Bank_Dynamic:
	case em_Goverment_Voice:
	case em_BaiJia_Adv_TT:
	case em_BaiJia_Adv_SY:
	case em_News_Common:
		dwDefaultCount = 200;
		break;
	case em_Baijia_News:
		dwDefaultCount = 500;
		break;
	default:
		break;
	}
	return dwDefaultCount;
}
BOOL GlobalSetting::IsNeedSaveByType(int nNewsType)
{ 
	if(em_Scroll_RSS == nNewsType ||
		em_News_Adv == nNewsType)
	{
		return FALSE;
	}
	return TRUE;
}


void GlobalSetting::SetUIMode(const CString& strMode)
{
#if 0
	CString strFileName = GetGFSubDir(GDT_CFG) + _T("system.ini");
	ASSERT(IsPathFileExist(strFileName));

	CString strRes = GW_Cfg::GetRegKeyString(strFileName, _T("UI"), strMode);
	
	if (strRes.IsEmpty())
	{
		strRes = _T("res\\");
	} 

	if (strRes.Right(1) != _T("\\") && strRes.Right(1) != _T("/") )
	{
		strRes += _T("\\");
	}

	m_strResDir = GetMainDir() + strRes;
	ASSERT(IsPathFileExist(m_strResDir));

	m_strUIMode = strMode;
	
	RefreshUICfg();
#else
	int nSkinNum = m_cSkinVec.size();
	for(int ii =0; ii < nSkinNum; ii ++)
	{
		if(strMode == m_cSkinVec[ii]->szSerialName)
		{
			m_nCurrentSkinIndex = ii;
			m_strResDir = GetMainDir() + m_cSkinVec[m_nCurrentSkinIndex]->szResPath ;
			return;
		}
	}
#endif
}

void GlobalSetting::RefreshUICfg()
{
	UICOLORMAP mapColor;
	mapColor.swap(m_mapUIColor);
}

COLORREF GlobalSetting::GetUIColorImpl(UINT uKeyID, LPCTSTR lpKey, COLORREF clrDefault)
{
	ASSERT(0);
	return 0;
#if 0
	UICOLORMAPITR itr = m_mapUIColor.find(uKeyID);

	if (itr != m_mapUIColor.end())
	{
		return itr->second;
	}

	static CString strColorFile = GetGFSubDir(GDT_CFG) + _T("uicolor.ini");
	ASSERT(IsPathFileExist(strColorFile));

	CString strColor = GW_Cfg::GetRegKeyString(strColorFile, m_strUIMode, lpKey);

	COLORREF clr = clrDefault;

	if (!strColor.IsEmpty())
	{
		stringsplitter sp(_T(",;"));

		sp.split(strColor);

		if (sp.size() == 1)
		{
			clr = (COLORREF)atoi(sp[0]);
		}
		else if (sp.size() == 3)
		{
			clr = RGB(atoi(sp[0]), atoi(sp[1]),atoi(sp[2]));
		}
	}

	m_mapUIColor.insert(itr, UICOLORMAP::value_type(uKeyID, clr));

	return clr;
#endif
}


void GlobalSetting::SetUIMode(int nIndex)
{
	int nSkinNum = m_cSkinVec.size();
	if(nIndex < 0 || nIndex > nSkinNum)
	{
		return;
	}
	m_nCurrentSkinIndex = nIndex;
	m_strResDir = GetMainDir() + m_cSkinVec[m_nCurrentSkinIndex]->szResPath ;
}

int  GlobalSetting::GetCurrentUIModeIndex()
{
	return m_nCurrentSkinIndex;
}

COLORREF GlobalSetting::GetCurSkinColorByID(UINT uKeyID, COLORREF clrDefault)
{
	int nSkinNum = m_cSkinVec.size();
	if(m_nCurrentSkinIndex < 0 || m_nCurrentSkinIndex >= nSkinNum)
	{
		return clrDefault;
	}
	if(uKeyID >= m_cSkinVec[m_nCurrentSkinIndex]->nMaxCellNum)
	{
		return clrDefault;
	}
	if(FALSE == m_cSkinVec[m_nCurrentSkinIndex]->pSkinCell[uKeyID].bInited)
	{
		return clrDefault;
	}
	return m_cSkinVec[m_nCurrentSkinIndex]->pSkinCell[uKeyID].clColor;
}


COLORREF GlobalSetting::GetColorFromString(const CString& strColor)
{
	COLORREF clColor = 0;
	if(strColor.IsEmpty())
	{
		return clColor;
	}
	if(strColor[0] == '#')
	{//such #FF00AA		16进制
		clColor = _tcstol((LPCTSTR)strColor, NULL, 16);
	}
	else
	{//RGB 色系
		stringsplitter sp(_T(",;"));
		sp.split(strColor);
		if (sp.size() == 1)
		{
			clColor = (COLORREF)_ttol(sp[0]);
		}
		else if (sp.size() == 3)
		{
			clColor = RGB(_ttol(sp[0]), _ttol(sp[1]),_ttol(sp[2]));
		}
	}
	return clColor;
}

BOOL GlobalSetting::ReadSkinFile()
{
	CString strFileName = GlobalSetting::GetInstance()->GetGFSubDir(GDT_CFG);
	strFileName += _T("SkinBox.xml");

	CMarkup xmlDoc;

	if (!xmlDoc.Load(strFileName))
	{
		WriteLogMessage(LogError, _T("加载皮肤[%s]失败,%s"), strFileName, xmlDoc.GetError());
		return FALSE;
	}

	xmlDoc.ResetMainPos();
	if(FALSE == xmlDoc.FindElem(_T("root"))) 
	{
		return FALSE;
	}
	CString strText; 
	strText=xmlDoc.GetAttrib(_T("maxcolornum"));
	int nMaxColorNum = _ttol((LPCTSTR)strText);
	if(nMaxColorNum <= 0)
	{
		return FALSE;
	}
	xmlDoc.IntoElem();
	while(xmlDoc.FindElem(_T("SkinSeria")))
	{
		tagSkinSerial * pSkin = new tagSkinSerial;
		pSkin->pSkinCell = new tagSkinCell[nMaxColorNum];
		pSkin->nMaxCellNum = nMaxColorNum;
		strText = xmlDoc.GetAttrib(_T("name"));
		_tcsncpy(pSkin->szSerialName, (LPCTSTR)strText, 31);//皮肤名称
		strText = xmlDoc.GetAttrib(_T("resPath"));
		if(strText.IsEmpty())
		{
			strText=_T("Res");
		}
		_stprintf(pSkin->szResPath, _T("%s\\"), (LPCTSTR)strText);//图片资源路径
		xmlDoc.IntoElem();
		while(xmlDoc.FindElem(_T("WndSkin")))
		{
			strText = xmlDoc.GetAttrib(_T("name"));
			xmlDoc.IntoElem();
			while(xmlDoc.FindElem(_T("Cell")))
			{
				strText = xmlDoc.GetAttrib(_T("id"));
				int nID = _ttol(strText);
				if(nID >= nMaxColorNum)
				{
					ASSERT(0);
					continue;
				}
				strText = xmlDoc.GetAttrib(_T("color"));
				COLORREF clColor = GetColorFromString(strText);
				pSkin->pSkinCell[nID].clColor = clColor;
				pSkin->pSkinCell[nID].dwID = nID;
				pSkin->pSkinCell[nID].bInited = TRUE;
			}
			xmlDoc.OutOfElem();
		}
		m_cSkinVec.push_back(pSkin);
		xmlDoc.OutOfElem();
	}
	xmlDoc.OutOfElem();
	return TRUE;
}

void  GlobalSetting::FreeAllSkin()
{
	int nSkinNum = m_cSkinVec.size();
	for(int ii = 0; ii < nSkinNum; ii ++)
	{
		tagSkinSerial * pSkin = m_cSkinVec[ii];
		delete pSkin;
		pSkin = NULL;
	}
	m_cSkinVec.clear();
}

void GlobalSetting::GetAllSkinName(CSTRINGVEC & skinVec)
{
	int nSkinNum = m_cSkinVec.size();
	for(int ii = 0; ii < nSkinNum; ii ++)
	{
		CString strSkin = m_cSkinVec[ii]->szSerialName;
		skinVec.push_back(strSkin);
	}
}

 
void GlobalSetting::SetDXRoomInfo(const CString& strRoomID, const CString& strTeacherName)
{
	this->AddKey(DX_ROOMID, strRoomID);
	this->AddKey(DX_TEACHERNAME, strTeacherName);
}

void GlobalSetting::GetDXRoomInfo(OUT CString& strRoomID, OUT CString& strTeacherName)
{
	strRoomID = this->ToString(DX_ROOMID,_T(""));
	strTeacherName = this->ToString(DX_TEACHERNAME,_T(""));
}

BOOL GlobalSetting::IsDXRoomEmpty()
{
	CString strRoomID;
	CString strTeacherName;
	this->GetDXRoomInfo(strRoomID , strTeacherName);
	if(strRoomID.IsEmpty())
	{
		return TRUE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
EnvironSetting* GetEnvSetting()
{
	return EnvironSetting::GetInstance();
}

BOOL EnvironSetting::LoadFromProcessEnvString(const CString& strCommadLine)
{
	if (strCommadLine.IsEmpty() || strCommadLine.Left(6).CompareNoCase(GW_CMDLINE_SIGN) != 0)
	{
		return FALSE;
	}

	m_bActive = TRUE;

	m_parser.Parse(strCommadLine);

	return TRUE;
}

const CString EnvironSetting::operator[](LPCTSTR lpKey)
{
	return GetValue(lpKey);
}

BOOL EnvironSetting::ToBool(LPCTSTR lpKey, BOOL bDefault)
{
	const CString strValue = GetValue(lpKey);

	if (strValue.IsEmpty())
	{
		return bDefault;
	}

	if (strValue == "0" || strValue.CompareNoCase("false") == 0)
	{
		return FALSE;
	}
	else if (strValue == "1" || strValue.CompareNoCase("true") == 0)
	{
		return TRUE;
	}

	return bDefault;
}

const CString EnvironSetting::GetValue(LPCTSTR lpKey)
{
	return m_parser.ValueAsString(lpKey);
}

void EnvironSetting::InitInstance()
{
	m_bActive = FALSE;

#if _DEBUG
#if 0
	m_bActive = TRUE;
	m_parser.AddKey(_T("webserver"), _T("test."));
	m_parser.AddKey(_T("hqserver"), _T("stapp.gwstation.baidao.com"));	
#endif
#endif
}

/************************************************************************/
/* 弹出对话框属性                                                                */
/************************************************************************/
vector<POPUPATTR>  CMHPopupWndAttr::m_vecPopupAttr;
BOOL CMHPopupWndAttr::LoadAttr()
{
	m_vecPopupAttr.clear();

	CString strFileName = GlobalSetting::GetInstance()->GetGFSubDir(GDT_CFG);
	strFileName += "pupwebwnd.xml";

	CMarkup xmlDoc;

	if (!xmlDoc.Load(strFileName))
	{
		WriteLogMessage(LogError, _T("加载配置文件[%s]失败,%s"), strFileName, xmlDoc.GetError());
		return FALSE;
	}

	xmlDoc.ResetMainPos();

	while(xmlDoc.FindChildElem(_T("field")))
	{
		POPUPATTR popattr;
		popattr.m_strName = xmlDoc.GetChildAttrib(POPUPWND_ATTR_TITLE);
		popattr.m_strURL = xmlDoc.GetChildAttrib(POPUPWND_ATTR_URL);
		popattr.m_size.cx = xmlDoc.GetChildAttrib(POPUPWND_ATTR_WIDTH, 0);
		popattr.m_size.cy = xmlDoc.GetChildAttrib(POPUPWND_ATTR_HIGH, 0);
		m_vecPopupAttr.push_back(popattr);

	}

	return TRUE;
}

POPUPATTR CMHPopupWndAttr::GetAttr( LPCTSTR lpName )
{
	//if (m_vecPopupAttr.size() <= 0)
	LoadAttr();

	POPUPATTR popattr;
	vector<POPUPATTR>::iterator it;
	for (it = m_vecPopupAttr.begin(); it != m_vecPopupAttr.end(); it++)
	{
		if ((*it).m_strName == lpName)
		{
			popattr = (*it);
			break;
		}
	}
	return popattr;
}
