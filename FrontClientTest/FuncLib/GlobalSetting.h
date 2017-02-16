#pragma once
#include "gwtypes.h"
#include <toolclass.h>
#include "GWParamParser.h"

#undef GW_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define GW_EXT_CLASS _declspec(dllexport)
#else
#define GW_EXT_CLASS _declspec(dllimport)
#endif
 
typedef enum GW_DIR_TYPE 
{ 
	GDT_CFG	     = 0,		// 配置文件目录
	GDT_USERDATA = 1,		// 用户文件
	GDT_UPDATE   = 2,		// 更新文件
	GDT_DATA	 = 3,		// 程序数据
	GDT_RES		 = 4,		// 资源目录
	GDT_LOG		 = 5,		// 系统日志
	GDT_BAKUP	 = 6,		// 备份目录
	GDT_PLUGIN	 = 7,		// 插件目录
	GDT_TACFG	 = 8,		// 技术分析使用配置目录
	GDT_TMP		 = 9,		// 临时目录
	GDT_SCRIPT	 = 10,		// 脚本文件目录
} GF_DIR_TYPE;

DECLARE_MAP(CString, CString, CSTRING)


enum EM_NewType
{
//	em_Scroll_RSS = 461,	//滚动资讯
	em_News_Common = 527,	//公共新闻
	em_News_TT = 528,	//TT新闻
	em_News_TD = 529,	//TD新闻
	em_News_YG = 530,	//YG新闻
	em_News_ZY = 9999, //ZY新闻
	em_FastNews = 988,	//快讯
	em_StatusBar_DX = 9998, //大象版本鲜花
	em_StatusBar_Common = 461, //滚动资讯

	em_Today_Headline = 585,	//今日必读
	em_Central_Bank_Dynamic = 586, //央行动态
	em_Goverment_Voice = 587, //重要言论

	em_News_Other_Left = 757,			// 兴业银行左侧新闻资讯
	em_News_Other_Right = 758,			// 兴业银行右侧新闻资讯
	em_News_Adv = 10000,	//广告新闻
	em_Baijia_News = 10001,	//百家新闻
	em_BaiJia_Adv_TT = 768,	//首页广告   TT
	em_BaiJia_Adv_SY = 809,	//首页广告   SY
	em_News_Oill_Headline = 806,	//原油要闻
	em_News_Oill_Dynamic = 807,  //油市动态
	em_News_Oill_Market_Dynamic = 808, //交易所动态
	//金策版新闻
	em_News_TD_Left=527,		//797,
	em_News_TD_Right=529,		//798,

	em_Baijia_YG = 9997,	//百家新闻
};

#if defined(VERSION_TAG_DX)
#define em_Scroll_RSS   em_StatusBar_DX
#else
#define em_Scroll_RSS   em_StatusBar_Common
#endif

class GW_EXT_CLASS GlobalSetting
{
public:
	~GlobalSetting(void);
	static GlobalSetting* GetInstance();

	int SetRunWithWinStart(BOOL bWithWinStart = TRUE);
	const CString GetMainExeName();								// 获取执行exe名称

	const CString GetMainDir();									// 获取程序运行目录
	const CString GetGFSubDir(GF_DIR_TYPE type);				// 获取指定类型的目录

	const CString GetDealFileName(SymbolObj* pSymbol, long lDate);			// 获取逐笔落地文件名称
	const CString GetMinuteFileName(SymbolObj* pSymbol, long lDate);		// 获取走势图落地文件名称
	const CString GetKLineFileName(SymbolObj* pSymbol, GWKLINE_TYPE nType);	// 获取K线落地文件名称

	const CString GetMinMineFileName( SymbolObj* pSymbol, long lDate);						// 当日信息地雷落地文件名称
	const CString GetHisMineFileName(SymbolObj* pSymbol);									// 历史信息地雷落地文件名称
 
	const CString GetMinServerTAFileName( SymbolObj* pSymbol, long lDate);						// 服务器指标分时落地文件名称
	const CString GetHisServerTAFileName(SymbolObj* pSymbol);									// 历史服务器指标落地文件名称
	const CString GetNewsFileName(SymbolObj* pSymbol, long lDate, int nNewsType);				//服务器 新闻落地文件名称
	const CString GeSLSPFileName(SymbolObj* pSymbol, long lDate, int nPeriod);				//服务器 SLSP 文件名称
	const CString GeQKSignalFileName(SymbolObj* pSymbol, int nPeriodType, int nSignalKind);				//服务器 新闻落地文件名称
	const CString GeBYJZFileName(SymbolObj* pSymbol, int nPeriodType, int nSignalKind);	
	const CString GetSystemConfigFileName();					// 获取系统配置文件名称
	const CString GetUserConfigFileName();						// 获取用户配置文件名称
	const CString GetResSubDir(CString& strSubDir);	//资源目录下的子目录

	void AddKey(const CString& strKey, const CString& strValue);
	void AddKey(const CString& strKey, int nValue);
	void AddKey(const CString& strKey, INT64 nValue);
	void AddKey(const CString& strKey, double dbValue);

	const CString ToString(LPCTSTR lpKey, const CString& strDefault = _T(""));
	BOOL ToBool(LPCTSTR lpKey, BOOL bDefault = FALSE);
	int ToInt(LPCTSTR lpKey, int nDefault = 0);
	INT64 ToInt64(LPCTSTR lpKey, INT64 nDefault = 0);
	double ToDouble(LPCTSTR lpKey, double dbDefault = 0.0);

	const CString operator[](LPCTSTR lpKey);
	static DWORD GetMaxNewsCountFromType(int nNewsType);
	static BOOL IsNeedSaveByType(int nNewsType);
	void RemoveLocalDataFile(const GWClearDataCommand & tClearCmd);
	void RemvoeLocalDataFile(SymbolObj * pSymbol, BOOL bHisKOrMin);
	void RemvoeLocalDataFile(MarketInfo * pMarket, BOOL bHisKOrMin);
	//本地数据版本号
	int GetVerDataNum();
	GWItemVer * FindVerDataByType(int nType);
	int IsTypeExist(int nType);
	void AddTypeInVerData(const GWItemVer& tItemVer);

	void SetUIMode(const CString& strMode);
	void SetUIMode(int nIndex);
	int  GetCurrentUIModeIndex();
	COLORREF GetCurSkinColorByID(UINT uKeyID, COLORREF clrDefault = 0);
	COLORREF GetUIColorImpl(UINT uKeyID, LPCTSTR lpKey, COLORREF clrDefault);
	COLORREF GetColorFromString(const CString& strColor);
	void GetAllSkinName(CSTRINGVEC & skinVec);

	void SetDXRoomInfo(const CString& strRoomID, const CString& strTeacherName);
	void GetDXRoomInfo(OUT CString& strRoomID, OUT CString& strTeacherName);
	BOOL IsDXRoomEmpty();
private:
	void RefreshUICfg();
private:
	GlobalSetting(void);

	void LoadLocalConfig();										// 加载本地配置
	
	void SaveSettings();
	void ReadSettings();
	
	void ReadVerFile();		//读取文件版本信息
	BOOL ReadSkinFile();	//读取配色方案
	void FreeAllSkin();		//释放所有皮肤内存
public:
	void SaveVerFile();		//读取文件版本信息
private:
	struct tagSkinCell
	{
		DWORD dwID;		//ID
		COLORREF clColor;	//配置颜色
		BOOL bInited;		//是否初始化
		tagSkinCell()
		{
			ZeroMemory(this, sizeof(tagSkinCell));
		}
	};
	struct tagSkinSerial	
	{
		DWORD dwID;		//ID
		TCHAR szSerialName[32];	//皮肤名称
		TCHAR szResPath[64];	//资源图片路径
		int nMaxCellNum;	//最大颜色数
		int nRealCellNum;	//实际颜色数
		tagSkinCell * pSkinCell;	//细胞颜色
		tagSkinSerial()
		{
			ZeroMemory(this, sizeof(tagSkinSerial));
		}
		~tagSkinSerial()
		{
			if(pSkinCell)
			{
				delete [] pSkinCell;
				pSkinCell = NULL;
			}
		}
	};
	DECLARE_VECTOR(tagSkinSerial*, SKINSERIAL);
	SKINSERIALVEC m_cSkinVec;		//皮肤盒子
	int m_nCurrentSkinIndex;	//当前皮肤索引

	CSTRINGMAP	m_mapSetting;	
	GWITEMVERVEC m_vecCommonVer;
	CString		m_strResDir;
	CString		m_strUIMode;

	DECLARE_MAP(UINT, COLORREF, UICOLOR);
	UICOLORMAP	m_mapUIColor;
};

GW_EXT_CLASS GlobalSetting* GetGloablSetting();

#define POPUPWND_ATTR_TITLE			"name"
#define POPUPWND_ATTR_URL			"url"
#define POPUPWND_ATTR_WIDTH			"lwidth"
#define POPUPWND_ATTR_HIGH			"lhigh"


typedef struct tagPopupAttr
{
	CString m_strName;
	CString m_strURL;
	CSize	  m_size;
	tagPopupAttr()
	{
		m_strName = "";
		m_strURL = "";
		m_size.cx = 0;
		m_size.cy = 0;
	}
}POPUPATTR;
class GW_EXT_CLASS CMHPopupWndAttr
{
public:

	static POPUPATTR  GetAttr(LPCTSTR lpName);

private:
	static BOOL LoadAttr();
private:
	static vector<POPUPATTR>  m_vecPopupAttr;
};

class GW_EXT_CLASS EnvironSetting : public static_singletonbase<EnvironSetting>
{
	IMPLEMENT_STATIC_SINGLETON_MODE(EnvironSetting)
public:
	~EnvironSetting() {}

	void InitInstance();

	BOOL	LoadFromProcessEnvString(const CString& strCommadLine);
	const CString GetValue(LPCTSTR lpKey);
	const CString operator[](LPCTSTR lpKey);
	BOOL ToBool(LPCTSTR lpKey, BOOL bDefault = FALSE);

 	BOOL IsActive() const { return m_bActive; }
private:
	GWParamParser m_parser;
	BOOL m_bActive;
};


GW_EXT_CLASS EnvironSetting* GetEnvSetting();