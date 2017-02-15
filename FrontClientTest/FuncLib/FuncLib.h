#pragma once

#include <GWDefine.h>
#include <gwtypes.h>

#undef GW_FUNCLIB_API
#ifdef GWFUNCLIB_PROJECT  
#define GW_FUNCLIB_API					GW_DLLEXPORT
#else
#define GW_FUNCLIB_API					GW_DLLIMPORT
#endif

namespace GW_Log
{
	// 写入日志
#ifdef _DEBUG
	GW_FUNCLIB_API GWVOID WriteDebugMessageImp(LPCTSTR lpFile, int nLine, GWLogLevel nLevel, LPCTSTR lpMessage, ...);
#else
	GW_FUNCLIB_API GWVOID WriteDebugMessageImp(GWLogLevel nLevel, LPCTSTR lpMessage, ...);
#endif
	// 获取当前日志名称
	GW_FUNCLIB_API const CString& GetLastLogFileName();
}

namespace GW_Monitor
{
	enum MonitorRectType { EntireMonitor = 0, WorkAreaOnly };

	// 获取当前窗体所在显示器Rect
	GW_FUNCLIB_API GWBOOL GetCurrentMonitorRect(CWnd* pWnd, CRect& rcMonitor, MonitorRectType eType = WorkAreaOnly);
}

namespace GW_PathFile
{
	// 目录或文件是否存在
	GW_FUNCLIB_API GWBOOL IsPathFileExist(const CString& strPathFile);					
	
	// 判断路径是否相对路径
	GW_FUNCLIB_API GWBOOL IsRelativePath(const CString& strPath);	

	// 判断路径是否为网络路径
	GW_FUNCLIB_API GWBOOL IsNetPath(const CString& strPath);								

	// 获取文件或路径父目录
	GW_FUNCLIB_API const CString GetParentPath(const CString& strFileName);					


	GW_FUNCLIB_API  CString FullPath2ShortPath(const CString& strFullPahtFileName);	
	// 创建目录
	GW_FUNCLIB_API GWBOOL CreateDir(const CString& strPath);

	// 确保目录存在
	GW_FUNCLIB_API GWBOOL EnsureDirExist(const CString& strPath);	

	// 简单文件查找
	GW_FUNCLIB_API GWUINT FindFiles(const CString& strDir, CSTRINGVEC& vecFile, const CString& strExtName = _T("*.*"));

	// 目录删除
	GW_FUNCLIB_API GWBOOL DeleteDir(const CString&  strDir);

	// 文件删除
	GW_FUNCLIB_API GWBOOL DeleteFile(const CString&  strFile);

	// 创建空xml文件
	GW_FUNCLIB_API GWBOOL CreateEmptyXMLFile(const CString& strFileName);
}

namespace GW_Version
{
	// 是否游客
	GW_FUNCLIB_API GWBOOL IsVisitor();

	// 是否注册用户
	GW_FUNCLIB_API GWBOOL IsRegisteredUser();

	// 是否激活
	GW_FUNCLIB_API GWBOOL IsActiveUser();

	// 是否VIP
	GW_FUNCLIB_API GWBOOL IsVip();

	// 是否内部员工
	GW_FUNCLIB_API GWBOOL IsInnerStaff();
}

#define GetUIColor(id, default_color)		GW_UI::GetUIColorImpl(id,  default_color)
namespace GW_UI
{
	// 获取指定大小中文字体
	GW_FUNCLIB_API CFont* GetChineseFont(int nSize, BOOL bBold = FALSE);

	// 获取指定大小数字字体
	GW_FUNCLIB_API CFont* GetNumberFont(int nSize, BOOL bBold = FALSE);

	// 获取指定大小英文字体
	GW_FUNCLIB_API CFont* GetEnglishFont(int nSize, BOOL bBold = FALSE);

	// 获取价格的颜色
	GW_FUNCLIB_API COLORREF GetPriceColor(double dbPreClose, double dbPirce, int nDigit = 3);

	// 获取分割线pen
	GW_FUNCLIB_API CPen* GetSplitLine(int nWidth, COLORREF clrLine= RGB(99, 98, 98), int nStyle = PS_SOLID);

	// 两点间画线,为兼容老函数，新函数请直接调用GDILib库
	GW_FUNCLIB_API void LinePoint(CDC* pDC, CPoint pt1, CPoint pt2, CPen* pPen);

	// 设置UI模式
	GW_FUNCLIB_API void SetUIMode(const CString& strMode);
	
	GW_FUNCLIB_API COLORREF GetUIColorImpl(UINT uKeyID, COLORREF clrDefault = RGB(0, 0, 0));
}

namespace GW_Common
{
	// 获取指定窗体最顶层父窗体
	GW_FUNCLIB_API HWND GetTopLevelParent(HWND hWnd);

	// 设置当前活动FrameWnd
	GW_FUNCLIB_API GWVOID SetActiveFrameWnd(CWnd* pFrameWnd);

	// 获取当前活动FrameWnd
	GW_FUNCLIB_API CWnd*	GetActiveFrameWnd();

	// 获取当前活动Symbol
	GW_FUNCLIB_API SymbolObj* GetActiveSymbolObj();

	// 根据名称创建视图
	GW_FUNCLIB_API CView*	CreateViewByName(const CString& strViewName, CWnd * pParent, BOOL bInit, SymbolObj * pSymbolObj = NULL, const CString& strParam = _T(""));
	
	// 生成一个GUID,FullMode表示是否需要最外面大括号
	GW_FUNCLIB_API const CString CreateGUID(BOOL bFullMode = TRUE);

	// 将字符放入剪贴板
	GW_FUNCLIB_API GWBOOL PutStringToClipboard(const CString& strText);

	// 使用系统默认浏览器打开配置文件中的url
	GW_FUNCLIB_API GWBOOL QuickOpenUrl(LPCTSTR lpApp, LPCTSTR lpSection);

	// 两个double是否相等
	GW_FUNCLIB_API GWBOOL IsDoubleEqual(double dfVal1, double dfVal2);

	// 调用脚本文件
	GW_FUNCLIB_API GWBOOL CallScriptFunction(const char*filename, const char* func = NULL);
	
	GW_FUNCLIB_API void BroadcastMessage(HWND hParent, UINT msg, WPARAM wp, LPARAM lp, BOOL bPostMessage = TRUE);

	enum EMViewClassType
	{
		em_View_FST = 0,	//分时图
		em_View_KXT,	//K线图
		em_View_ZBT,	//逐笔图
		em_View_F10,	//F10
		em_View_DZQ,	//多周期同列
		em_View_FJB,	//分价表
		em_View_HQ,	//行情首页
		em_View_DPZ,	//多品种同列
		em_View_SY,	//首页
		em_View_LOGIN,	//登录
		em_View_TOOLBAR,	//工具条
		em_View_Max,
	};
	GW_FUNCLIB_API CString GetViewType(BYTE byViewType); 
	GW_FUNCLIB_API GWVOID ReportUserAction(const CString& strKey, DWORD dwID = 0,  BYTE byActionType = 0, BYTE byChangeType = 0,
											const CString& strSymbol = _T(""), const CString& strFromViewClass = _T(""), 
											const CString & strToViewClass = _T(""));
	GW_FUNCLIB_API GWVOID ReportUserAction(BYTE byFromViewType, const CString& strKey,DWORD dwID = 0,  BYTE byActionType = 0, BYTE byChangeType = 0);

	GW_FUNCLIB_API GWVOID ReportUserAction2(BYTE byToViewType, const CString& strKey,DWORD dwID = 0,  BYTE byActionType = 0, BYTE byChangeType = 0);
}

namespace GW_Encrypt
{
	// MD5
	GW_FUNCLIB_API CString MD5Encrypt(const CString& strCleartext,CString &strCiphertext,long lLen);
	
	// 对字符串简单进行MD5加密
	GW_FUNCLIB_API CString SimpleMD5Key(const CString& strText);

	//BASE64 加密
	GW_FUNCLIB_API CString BASE64Encrypt(const CString &strCleartext,long lLen);

	//BASE64 解密
	GW_FUNCLIB_API CString BASE64Decode(const CString &strCiphertext,long lLen);
}

namespace Json
{
	class Value;
}

namespace GW_Json
{
	class Value;
	
	// 打印Json字符串
	GW_FUNCLIB_API GWVOID PrintJsonString(Json::Value& jsValue);
	
	// 从Json结构获取字符串 
	GW_FUNCLIB_API CString StringFromJsonValue(Json::Value& jsValue);

}

namespace GW_Time
{
	enum TIME_FORMAT
	{
		TIME_YMD = 0,		// 日期 年月日（20121214）
		TIME_MD,			// 日期 月日 （1214）
		TIME_HMS,			// 时间 时分秒（131415）
		TIME_HM,			// 时间 时分（1314）
		TIME_FULL			// 年月日时分秒（20121214131415）
	};

	// 根据time_t生成时间
	GW_FUNCLIB_API GWLONG TimeFromTimeT(time_t t, TIME_FORMAT fmt);

	// 根据日期生成time_t
	GW_FUNCLIB_API time_t TimeTFromDate(long nDate);

	// 根据time_t生成字符时间串
	GW_FUNCLIB_API CString StringFromTimeT(time_t t, TIME_FORMAT fmt, const CString& strDateSp = _T("-"), const CString& strTimeSp = _T(":"), const CString& strDateTimeSp = _T(" "));

	// 根据交易日生成字符时间串
	GW_FUNCLIB_API CString StringFromDate(long nDate, TIME_FORMAT fmt, const CString& strDateSp = _T("-"), const CString& strTimeSp = _T(":"), const CString& strDateTimeSp = _T(" "));
}

namespace GW_HQ
{
	// 增加一个市场对象
	GW_FUNCLIB_API GWVOID AddMarketToMarketMap(MarketInfo* pMarket);
	
	// 查找一个市场对象
	GW_FUNCLIB_API MarketInfo* FindMarket(const CString& strMarket);
	
	// 将对象加入码表
	GW_FUNCLIB_API GWVOID AddSymbolToCodeList(SymbolObj* pSymbolObj);
	
	// 获取所有市场
	GW_FUNCLIB_API GWVOID GetAllMarketNames(CSTRINGVEC& vecName);
	
	// 获取所有市场
	GW_FUNCLIB_API GWVOID GetAllMarket(MRKETVEC& vecMarket);	

	// 获取指定市场下所有代码
	GW_FUNCLIB_API GWVOID GetMarketSymbols(const CString& strMarket, SYMBOLVEC& vecSymbol, BOOL bOnlyTopLevelSymbol = FALSE);		
	
	// 获取所有市场下所有代码
	GW_FUNCLIB_API GWVOID GetAllMarketSymbols(SYMBOLVEC& vecAllSymbol);		
	
	// 获取symbol所有子节点
	GW_FUNCLIB_API GWVOID GetMarketSymbols(SymbolObj& pParentSymbol, SYMBOLVEC& vecChildrenSymbol);

	// 全市场查找代码
	GW_FUNCLIB_API SymbolObj* FindSymbolFromAllMarket(const CString &strCode);	

	// 查找代码
	GW_FUNCLIB_API SymbolObj* FindSymbolObj(const CString& strMarket, const CString &strCode);	

	// 查找代码
	GW_FUNCLIB_API SymbolObj* FindSymbolObjEx(const CString& strObj);

	// 获取下一只股票
	GW_FUNCLIB_API SymbolObj*	GetNextSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket);				

	// 获取上一只股票
	GW_FUNCLIB_API SymbolObj*	GetPreviousSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket);			

	// 设置当前循环的symbols
	GW_FUNCLIB_API void SetActiveSymbols(SYMBOLVEC& vecSymbols);

	// 获取选中市场的股票循环队列
	GW_FUNCLIB_API GWVOID GetMarketSymbolLoopVector(SYMBOLVEC& vecMarketSymbol, const CString& strMarket, SymbolObj* pStartSymbolObj = NULL, UINT nMaxCount = 16, BOOL bLoop = TRUE);
	GW_FUNCLIB_API GWBOOL IsVolMarket(SymbolObj* pSymbolObj);
	GW_FUNCLIB_API GWBOOL IsVolMarket(MarketInfo* pMarket);

	GW_FUNCLIB_API int PeriodMin2KType(int nPeriodMin);
	GW_FUNCLIB_API int KType2PeriodMin(int nKType);
	GW_FUNCLIB_API CString GetTJXName(int nKType);		//太极
	GW_FUNCLIB_API CString GetQKXName(int nKType, int nQKX = tagQKSignal::em_QK_Line);		//乾坤
	GW_FUNCLIB_API CString GetTJXNameEx(int nPeriodMin);		//太极
	GW_FUNCLIB_API CString GetQKXNameEx(int nPeriodMin, int nQKX);		//乾坤
	GW_FUNCLIB_API CString GetBYJZName(int nPeriodMin);//博弈矩阵
}

namespace GW_Cfg
{
	// 从配置文件Indicator.xml读取所有技术分析指标
	GW_FUNCLIB_API GWVOID ReadIndicatorXML(CIndicatorArr & CTAArr, const CString & strKey);

	// 从配置文件urlconfig.ini中读取网页信息
	GW_FUNCLIB_API CString GetUrlFromCfg(LPCTSTR lpApp, LPCTSTR lpSection);

	// 读取INI配置文件
	GW_FUNCLIB_API CString GetRegKeyString(const CString& strFile, const CString& strSection, const CString& strKey, const CString& strDefault = _T(""));
	GW_FUNCLIB_API int GetRegKeyInt(const CString& strFile, const CString& strSection, const CString& strKey, int lValDefalt = 0);

	// 写入INI配置文件
	GW_FUNCLIB_API GWBOOL WriteRegKeyString( const CString& strFile,  const CString& strSection,  const CString& strKey, const CString& strVal);
	GW_FUNCLIB_API GWBOOL WriteRegKeyInt( const CString& strFile,  const CString& strSection,  const CString& strKey, int lVal);
}

namespace GW_InfoBomb
{
	GW_FUNCLIB_API 	void ExtractDelInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecDel);//提取需要删除的信息地雷
	GW_FUNCLIB_API 	void RemoveUnusedInfoBomb(IN OUT GWINFOBOMVEC& vecOrg, IN const GWINFOBOMVEC & vecDel);	//删除过期的信息地雷

	GW_FUNCLIB_API void SortAndUniqueInfoBombArr(IN OUT GWINFOBOMVEC& vecOrg, BOOL bKMode = FALSE);					//排序与唯一化

	GW_FUNCLIB_API 	void ExtractUpdateInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecUpdate);//提前更新的信息地雷
	GW_FUNCLIB_API 	void MergeUpdateInfoBomb(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecUpdate);	//合并更新的信息地雷，保留最新的一个
	GW_FUNCLIB_API  void RemovInfoBombByType(IN OUT GWINFOBOMVEC& vecOrg, IN int nType);	//删除指定类型的信息地雷

	GW_FUNCLIB_API  void CheckUpdateInfoBombArr(IN OUT GWINFOBOMVEC& vecOrg);//检测信息地雷队列，强制更新标志
	
	GW_FUNCLIB_API 	void MergeTwoInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecNew, BOOL bNeedSortAndUnique, BOOL bKMode = FALSE);//合并2个信息地雷队列
	GW_FUNCLIB_API 	void TrimeInfoBombArr(IN GWINFOBOMVEC& vecOrg,  BOOL bNeedSortAndUnique, BOOL bKMode = FALSE);

	GW_FUNCLIB_API CString CreateInfoBomLink(const SymbolObj * pSymbol, const GWInfoBom & tGWInfoBomb, const CString& strFixLink);		// 构造信息地雷连接
}

namespace GW_Multimedia
{
	GW_FUNCLIB_API BOOL  PlaySoundEx(LPCTSTR pszSound,BOOL bBeep = TRUE);
	
}
namespace GW_BigNews
{
	GW_FUNCLIB_API 	void ExtractDelNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecDel);//提取需要删除的新闻资讯
	GW_FUNCLIB_API 	void RemoveUnusedNews(IN OUT GWBIGNEWSVEC& vecOrg, IN const GWBIGNEWSVEC & vecDel);	//删除过期的新闻资讯

	GW_FUNCLIB_API void SortAndUniqueNewsArr(IN OUT GWBIGNEWSVEC& vecOrg);					//排序与唯一化

	GW_FUNCLIB_API 	void ExtractUpdateNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecUpdate);//提前更新的新闻资讯
	GW_FUNCLIB_API 	void MergeUpdateNews(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecUpdate);	//合并更新的信息地雷新闻资讯，保留最新的一个
	GW_FUNCLIB_API  void RemovNewsByType(IN OUT GWBIGNEWSVEC& vecOrg, IN int nType);	//删除指定类型的新闻资讯
	
	GW_FUNCLIB_API 	void MergeTwoNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecNew, BOOL bNeedSortAndUnique);//合并2个信息地雷队列
	GW_FUNCLIB_API 	void TrimeNewsArr(IN GWBIGNEWSVEC& vecOrg,  BOOL bNeedSortAndUnique);
	GW_FUNCLIB_API  int  GetValidNewsNum(IN GWBIGNEWSVEC& vecOrg);
	GW_FUNCLIB_API  void SaveNewsArr(IN GWBIGNEWSVEC& vecOrg, int  nMaxValidNum);
	GW_FUNCLIB_API  void MarkUpdateNewsByTime(IN OUT GWBIGNEWSVEC& vecOrg, time_t dwLastTime, BOOL bForceRecover = FALSE);
 
}

class GW_FUNCLIB_API CStringCoder  
{  
public:  
	CStringCoder(void);  
	~CStringCoder(void);  

	static void UTF_8ToGB2312(CString &pOut, char *pText, int pLen);//utf_8转为gb2312  
	static void GB2312ToUTF_8(CString& pOut,char *pText, int pLen); //gb2312 转utf_8  
	static CString UrlGB2312(char * str);                           //urlgb2312编码  
	static CString UrlUTF8(char * str);                             //urlutf8 编码  
	static CString UrlUTF8Decode(const CString& strURL);                  //urlutf8解码  
	static CString UrlGB2312Decode(const CString& strURL);                //urlgb2312解码  

private:  
	static void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);  
	static void UTF_8ToUnicode(WCHAR* pOut,char *pText);  
	static void UnicodeToUTF_8(char* pOut,WCHAR* pText);  
	static void UnicodeToGB2312(char* pOut,WCHAR uData);  
	static char  CharToInt(char ch);  
	static char StrToBin(char *str);  

};  