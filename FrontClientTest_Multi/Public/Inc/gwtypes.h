#pragma once

#include <GWDefine.h>

#include <math.h>
#include <vector>
#include <list>
#include <map>
using namespace std;
#define  MAX_COMMON_TEXT_LEN		255
#define  MAX_COMMON_NAME_LEN  31

enum GWLogLevel 
{ 
	LogDisabled		= 0,			// 禁用 
	LogFull			= 1,			// 完整
	LogNormal		= 2,			// 普通
	LogWarning		= 3,			// 警告
	LogError		= 4				// 错误
};

DECLARE_VECTOR(CString, CSTRING)

class GWFloat
{
public:
	GWFloat()
	{
		iValue = 0;
		nDigit = 0;
		bInit = FALSE;
	}

	GWFloat(__int64 iValue, int nDigit)
	{
		SetValue(iValue, nDigit);
	}

	GWFloat(double dbValue, int nDigit)
	{
		SetValue(dbValue, nDigit);
	}

	BOOL IsValid() const
	{
		return bInit;
	}

	void SetValue(const GWFloat& v)
	{
		bInit = v.bInit;
		iValue = v.GetInnerValue();
		nDigit = v.GetDigit();
	}

	void SetValue(__int64 iValue, int nDigit = 2)
	{
		bInit = TRUE;
		this->nDigit = nDigit;
		this->iValue = iValue;
	}

	void SetValue(double dbValue, int nDigit = 2, BOOL bRounded = TRUE)
	{
		bInit = TRUE;
		this->nDigit = nDigit;
		float fRound = 0.5;
		if(FALSE == bRounded)
		{//末尾是否四舍五入
			fRound = 0.0;
		}
		if(dbValue < - 0.00001)
		{
			iValue = (__int64)(dbValue *  pow(10.0, nDigit) - fRound);
		}
		else
		{
			iValue = (__int64)(dbValue *  pow(10.0, nDigit) + fRound);
		}
	}

	double GetValue() const
	{
		//ASSERT(IsValid());

		if (!IsValid())
		{
			return 0.0;
		}

		return iValue / pow(10.0, nDigit);
	}

	int GetIntValue() const
	{
		return (int)iValue;
	}

	__int64 GetInnerValue() const
	{
		return iValue;
	}

	const CString AsNumberString() const
	{
		if (!IsValid())
		{
			return _T("0");
		}

		return ToString();
	}

	const CString ToString() const
	{
		if (!IsValid())
		{
			return _T("-");
		}

		CString strText;
		strText.Format(_T("%.*f"), nDigit, GetValue());			
		return strText;
	}
	const CString ToStringUnit() const
	{
		if (!IsValid())
		{
			return _T("-");
		}

		CString strText;
		double dfDiff = 0.000;

#define TEN_MILLION			10000000
#define ONE_HUNDRED_MILLION			100000000
#define TEN_THOUSAND				10000
		if (GetValue() > TEN_MILLION)
		{
			strText.Format(_T("%.2f"), (GetValue() /  ONE_HUNDRED_MILLION+ dfDiff));
			strText += _T("亿");
		}
		else if (GetValue() > TEN_THOUSAND && GetValue() < TEN_MILLION )
		{
			strText.Format(_T("%.2f"), (GetValue() / TEN_THOUSAND+dfDiff));
			strText += _T("万");
		}	
		else
		{
			strText.Format(_T("%.0f"),GetValue());
		}
		return strText;
	}
	const CString ToStringPrecent() const
	{

		if (!IsValid())
		{
			return _T("-");
		}

		CString strText;
		strText.Format(_T("%.*f"), 2, GetValue() * 100);	
		return strText;
	}

	int GetDigit() const
	{
		return nDigit;
	}

	bool EqualZero() const
	{
		return GetInnerValue() == 0;
	}

	bool operator==(const GWFloat& v)
	{
		return GetInnerValue() == v.GetInnerValue();
	}

	bool operator<(GWFloat& v)
	{
		//		return GetInnerValue() < v.GetInnerValue();
		return GetValue() < v.GetValue();
	}

	bool operator>(GWFloat& v)
	{
		//		return GetInnerValue() > v.GetInnerValue();
		return GetValue() > v.GetValue();
	}

	GWFloat operator -(GWFloat& v)
	{
		if (!IsValid())
		{
			return v;
		}

		if (GetDigit() == v.GetDigit())				// 小数位数相同直接减就可以了
		{
			return GWFloat(GetInnerValue() - v.GetInnerValue(), nDigit);
		}
		else
		{
			return GWFloat(GetValue() - v.GetValue(), nDigit);
		}
	}

	GWFloat operator -(double db)
	{
		if (!IsValid())
		{
			return GWFloat(-db, 2);
		}

		GWFloat v(db, GetDigit());
		return *this - v;
	}

	GWFloat operator +(double db)
	{
		if (!IsValid())
		{
			return GWFloat(db, 2);
		}

		GWFloat v(db, GetDigit());
		return *this + v;
	}

	float ToFloat() 
	{
		return (float)GetValue();
	}

	operator double() 
	{
		return GetValue();
	}

	operator float() const
	{
		return (float)GetValue();
	}

	operator double() const
	{
		return GetValue();
	}

	GWFloat operator +(GWFloat& v)
	{
		if (!IsValid())
		{
			return v;
		}

		if (GetDigit() == v.GetDigit())				// 小数位数相同直接加就可以了
		{
			return GWFloat(GetInnerValue() + v.GetInnerValue(), nDigit);
		}
		else
		{
			return GWFloat(GetValue() + v.GetValue(), nDigit);
		}
	}

	void operator += (GWFloat& v)
	{
		*this = *this + v;
	}

	GWFloat& operator = (const GWFloat& v)
	{
		SetValue(v);
		return *this;
	}
private:
	__int64		iValue;			// 值
	int			nDigit;			// 小数点
	BOOL		bInit;			// 是否初始化
};

typedef struct tagInfoBomItem
{
	enum EMBombStat
	{
		em_Bomb_Add = 2,
		em_Bomb_Del = 3,
		em_Bomb_Update = 432,
	};
	DWORD		dwArticalID;		// 文章ID
	DWORD		dwChannelID;		//频道
	int	nType;						// 类型  

	time_t	tDeployTime;			// 发布日期
	time_t	dwTradeDate;			// 交易日 
	TCHAR	strSymbol[32];			//品种
	TCHAR		strTitle[256];			// 标题
	TCHAR		strSummary[256];	// 摘要
	TCHAR		strURL[256];		//url
	time_t	dwCreateTime;			// 创建日期
	time_t  dwUpdateDateTime;		// 更新日期
	int			nState;				// 状态
	DWORD dwRankID;					// 评级
	TCHAR		strAuthor[64];		// 作者 
	DWORD dwImageID;		//头像ID
	int		nTargetUser;		//目标用户 0：全部，1：直播室，2：未激活，3：激活
	DWORD dwReserved[3];	//预留字段
	BYTE  byReserved[256];	//预留字段

	bool operator==(const tagInfoBomItem& item)
	{
		if (dwArticalID == item.dwArticalID )
		{
			return true;
		}
		else
			return false;
	}
	tagInfoBomItem()
	{ 
		ZeroMemory(this, sizeof(tagInfoBomItem)); 
	}
} GWInfoBom;

DECLARE_VECTOR(GWInfoBom, GWINFOBOM)

struct SnapShotData
{
	time_t	tmTime;				// 快照时间
	GWFloat  dbOpen;				// 开盘
	GWFloat  dbHigh;				// 最高
	GWFloat  dbLow;				// 最低
	GWFloat  dbClose;			// 收盘
	GWFloat  dbPreClose;			// 前收
	GWFloat  dbChange;			// 涨跌
	GWFloat  dbChangeRate;		// 涨跌幅
	GWFloat	dbAmplitude;		// 振幅
	GWFloat  dbAverage;			// 均价
	GWFloat  dbEntrustDiff;		// 委差
	GWFloat  dbEntrustPercent;	// 委比

	GWFloat	dbPositions;		// 持仓量

	GWFloat  dbVolume;			// 成交量
	GWFloat  dbAmount;			// 成交额

	GWFloat  dbRealVolumeNow;		//快照实际的 现量
	GWFloat  dbVolumeNow;		// 现量（用于显示）
	GWFloat  dbAmountNow;		// 现额 

	GWFloat  dbBuyPrice[10];		// 买1 - 买5
	GWFloat  dbSellPrice[10];	// 卖1 - 卖5
	GWFloat  dbBuyVolume[10];	// 买1 - 买5	量
	GWFloat  dbSellVolume[10];	// 卖1 - 卖5	量
	GWFloat  dbBuyAmount[10];	// 买1 - 买5	额
	GWFloat  dbSellAmount[10];	// 卖1 - 卖5	额
};

struct MarketInfo;
#define  MAX_KLINE_VER_NUM			16
enum emKVerType
{
	em_KVerDay = 0,	//日线
	em_KVer1Min,	//1分钟
	em_KVer15Min,	//15分钟
	em_VerInfoBomb,	//信息地雷 
	em_VerServerTA,	//银利一号指标
	em_VerF1,		//逐笔明细
	em_VerEnd,
};
struct TRADETIME_SECTION
{
	DWORD nBegin;
	DWORD nEnd;
};
DECLARE_VECTOR(TRADETIME_SECTION, TRADETIME)			// 交易时间段
struct SymbolObj
{
	CString strMarket;				// 市场
	CString	strCode;				// 代码
	CString strName;				// 中文名
	CString	strCategory;			// 分类
	CString strKeyWords;			// 关键字
	GWFloat dbLastClose;			// 昨收盘
	GWFloat dbLastSettlement;		// 昨结算
	GWFloat dbNewSettlement;	// 新的昨结
	int	   nDigit;					// 小数位数
	CString strCurrencyType;		// 货币类型
	CString strQuoteUnit;			// 报价单位
	BOOL	bLastCloseInit;			// 是否成功初始化昨收

	MarketInfo *pMarketIfo;			// 股票所属市场信息
	BOOL	bSelfSotck;				// 是否自选股

	SnapShotData	snapshotData;	// 保持最后一笔快照
	DWORD nKVerArr[MAX_KLINE_VER_NUM];			//K线版本号
	DWORD dwPrivilege;				//权限控制
	DWORD dwAllRight;				//是否具有 SLSP权限
	//GWFloat dbDiancha;		//点差	通过买卖价程序计算
	GWFloat dbShouxufei;	//手续费	比如万分之八
	GWFloat dbBaozhengjin;	//保证金比例	比如百分之八
	GWFloat dbUnit;		//单位 1手
	
	//TT 版本扩展
	CString strGuapaiFang;		//挂牌方
	CString strTradetype;		//现货可回购
	CString strDir;		//收货（买）
	CString strApplytime; //交收宽期限（180天)
	CString strReadytime; //准备期(30天）
	CString strBrand;		//符合国标、津贵所质量标准要求的品牌
	enum EMSymbolType
	{
		em_Symbol_Type_PreciousMetals = 0, //贵金属
		em_Symbol_Type__Stock,	//股票
		em_Symbol_Type_Index,	//指数
		em_Symbol_Type_Futures,	//期货Foreign currency
		em_Symbol_Type_ForeignCurrency, //外汇
		em_Symbol_Type_Warrant,	//权证
		em_Symbol_Type_Bond,	//债券
		em_Symbol_Type_Other1,
		em_Symbol_Type_Other2,
		em_Symbol_Type_Other3,
	};
	enum EMPrivilege
	{
		em_Symbol_Privilege_TA = 0X1, //是否具有银利一号数据
		em_Symbol_Privilege_SLSP = 0X2, //是否具有SLSP数据
		em_Symbol_Privilege__Other1  = 0X4,
		em_Symbol_Privilege__Other2 = 0X8,
		em_Symbol_Privilege__Other3 = 0X10,
	};
	int nSymbolType;	//类型  0： 贵金属   1 股票 2 指数  3期货...
	TRADETIMEVEC vecTradeTime;		// 交易时间段 HHMM 格式
	TRADETIMEVEC vecTradeTimeFull;	// 当天交易整个时段，time_t 格式

	CString	strParent;				// 父节点
	CSTRINGVEC vecChildren;			//  子节点

	BOOL HasChildren() const
	{
		return !vecChildren.empty();
	}

	BOOL HasParent() const
	{
		return !strParent.IsEmpty();
	}

	BOOL HasSnapshotData()
	{
		return snapshotData.tmTime != 0;
	}

	SymbolObj()
	{
		bSelfSotck = FALSE;
		bLastCloseInit = FALSE;
		pMarketIfo = NULL;
		ZeroMemory(&snapshotData, sizeof(snapshotData));
		ZeroMemory(nKVerArr, sizeof(nKVerArr));
		nSymbolType = em_Symbol_Type_PreciousMetals;
		dwPrivilege = 0;
		dwAllRight = 0;
	}

	bool operator < (const SymbolObj& symbolObj) const
	{
		if (symbolObj.strCode == strCode)		// 代码相同
		{
			if (symbolObj.strMarket == strMarket)	// 市场相同
			{
				return strCategory < symbolObj.strCategory;
			}
			else
			{
				return strMarket < symbolObj.strMarket;
			}
		}

		return strCode < symbolObj.strCode;
	}

	bool operator == (const SymbolObj& symbolObj)
	{
		return strMarket == symbolObj.strMarket
			&& strCode == symbolObj.strCode
			&& strCategory == symbolObj.strCategory;
	}

	double GetPreClose()
	{
		return dbLastClose;
	}
	GWFloat  GetShowSettlement()
	{//显示顺序 
		if(dbNewSettlement.IsValid())
		{
			return dbNewSettlement;
		}
		if(dbLastSettlement.IsValid())
		{
			return dbLastSettlement;
		}
		return dbLastClose;

	}
	GWFloat  GetCalSettlement()
	{//计算顺序
		if(dbLastSettlement.IsValid())
		{
			return dbLastSettlement;
		}
		return dbLastClose;
	}

	const CString& GetMarket()
	{
		return strMarket;
	}

	const CString& GetCode()
	{
		return strCode;
	}

	const CString& GetName()
	{
		return strName;
	}

	const CString& GetCategory()
	{
		return strCategory;
	}

	const CString& GetKeyWords()
	{
		return strKeyWords;
	}

	const CString GetObject()
	{
		CString strObj;
		strObj.Format(_T("%s.%s"), strMarket, strCode);

		return strObj;
	}
	BOOL IsYL1HOpen()
	{
		if(dwPrivilege & em_Symbol_Privilege_TA)
		{
			return TRUE;
		}
		 return FALSE; 
	}

	BOOL IsSLSPOpen()
	{
		if(dwPrivilege & em_Symbol_Privilege_SLSP)
		{
			return TRUE;
		}
		return FALSE; 
	}
	BOOL IsHasSLSPRight()
	{
		return (dwAllRight > 0);
	}
	void SetRight(DWORD dwRight)
	{
		dwAllRight = dwRight;
	}
};

DECLARE_VECTOR(SymbolObj*, SYMBOL)

DECLARE_MAP(SymbolObj*, int, SYMBOLREGISTER)	

typedef struct tagShortcutObj
{
	//UINT command;							// 命令
	CString command;						// 命令内容
} ShortcutObj;



struct MarketInfo
{
	CString strMarketName;
	CString strCName;				// 中文名称
	CString strCurrencyType;		// 货币类型
	CString strQuoteUnit;			// 报价单位
	long    nLastTradeDate;			//上一个交易日
	long		nTradeDate;			// 市场交易时间 
	TRADETIMEVEC vecTradeTime;		// 交易时间段 HHMM 格式
	TRADETIMEVEC vecTradeTimeFull;	// 当天交易整个时段，time_t 格式
	SYMBOLVEC	vecSymbols;			// 市场下股票代码

	BOOL	bOmit;					// 是否被忽略的市场
	BOOL	bVirtual;				// 是否虚拟市场
	int nTradeGearNum;			//买卖档位
};

DECLARE_VECTOR(MarketInfo*, MRKET)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum GWPackType		// 数据包类型
{
	PT_SECURITY	= 0,		// 码表
	PT_DEAL,				// 逐笔
	PT_PUSHEDSNAPSHOT,		// 推送快照
	PT_SNAPSHOT,			// 请求快照
	PT_MARKTESORTDATA,		// 市场排序数据
	PT_KLINE,				// K线数据
	PT_MARKETINFO,			// 市场信息
	PT_MARKETINIT,			// 市场更新
	PT_MULTISNAPSHOT,		// 多包快照
	PT_PRECLOSE_CHANGE,		// 昨收初始化

	PT_DAYKLINE,			// 日线数据
	PT_NEWS,				// 新闻公告
	PT_NEWS_RB,			// 新闻(右下角弹窗)

	PT_PRICETABLE,			// 分价表
	PT_ORDERLIST,			// 龙虎榜
	PT_SYMBOLNEWS,			// 股票对应新闻

	PT_DRAWLINE_SHARED,		// 画线分享 (非即时通道)
	PT_INFOBOM,						// 当天信息地雷
	PT_INFOBOMBBYDATE,				// 指定日期信息地雷
	PT_INFOBOMHIS,					// 历史信息地雷
	PT_INFOBOM_PUSH,						// 信息地雷(推送)

	PT_SERVERTA_BYDATE,				// 银利一号分时指标数据
	PT_SERVERTA_HISTORY,				// 银利一号历史指标数据
	PT_SERVERTA_PUSH,				// 银利一号数据推送

	PT_BIGNEWS_BYDATE,				// 新闻数据
	PT_BIGNEWS_PUSH,				// 新闻数据推送
	PT_ECONOMICDATA,				// 重要经济数据
	PT_HISECONOMICDATA,		// 历时重要经济数据数量
	PT_COMMONVER,			// 本地数据版本信息
	PT_CLEARLOCALDATA,		//清空本地数据
	PT_NEWS_CUTOMIZED,		// 画线分享 (非即时通道)

	
	//本地type 与服务端保持一致 by w.s.h
	PROTID_BSHOLDPOSITION_REQUEST = 106,		// 多空持仓请求
	PROTID_BSHOLDPOSITION_PUSH = 107,		// 多空持仓推送
	PROTID_BSTREND_REQUEST = 108,		// 多空趋势请求
	PROTID_BSTREND_PUSH = 109,		// 多空趋势推送
	PROTID_ETFPOSITION_REQUEST = 104,	// ETF持仓请求
	PROTID_ETFPOSITION_PUSH = 105,		// ETF持仓推送

	PROTID_LSP_POSITION_PUSH = 114,	//新协议， 总持仓比推送
	PROTID_LSP_POSITION_REQUEST = 115,

	PROTID_LSP_TREND_PUSH = 116,	//每分钟SLSP
	PROTID_LSP_TREND_REQUEST = 117,	//每分钟SLSP请求

	PROTID_TAIJISIGNAL_PUSH = 150,		// 太极信号推送
	PROTID_QKSIGNAL_PUSH = 151,		// 乾坤信号推送
	PROTID_QKSIGNAL_REQUEST = 120,		// 乾坤信号请求	

	PROTID_QK_MAP_PUSH = 156,		// 乾坤图推送
	PROTID_QK_MAP_REQUEST = 157,		// 乾坤图请求

	PROTID_BYJZ_PUSH = 158,		// 乾坤图推送
	PROTID_BYJZ_REQUEST = 159,		// 乾坤图请求


	PT_UNINIT,				// 未初始化数据
};

enum GWMINUTELINE_SUB_TYPE			// 分钟线子类型
{
	MST_CURRENT_DAY		= 1,		// 当日走势
	MST_HISTORY_DAY,				// 历史某日走势
	MST_MULTI_DAY,					// 多日走势
	MST_SERIES,						// 连续走势
	MST_TRADEDATE					// 交易日期
};

enum GWINITINFO_SUB_TYPE			// 初始化市场子类型
{
	IST_INITSINGLE		= 1,		// 单个市场初始化
	IST_INITALL						// 全部市场初始化
};

struct IMessageRevcer;

class GWBasePack
{
public:
	virtual ~GWBasePack() {}

	GWBasePack() 
	{
		Reset();
	}

	virtual void Reset()
	{
		nReqID		= -1;
		nType		= PT_UNINIT;
		nCount		= 0;
		strPackName = _T("");
		pSymbolObj  = NULL;
		nPackKey    = 0;
		pRecver		= NULL;
		bLostFlag   = FALSE;
		nTickCount  = 0;
		bSysErrorPack = FALSE;
	}

	int			nReqID;				// 包请求ID
	GWPackType	nType;				// 包类型
	UINT		nCount;				// 数据量
	CString		strPackName;		// 包名称
	ULONG_PTR	nPackKey;			// 用户请求数据时传递的Key，不做处理，直接返回给用户
	ULONG_PTR	nSysKey;			// 系统Key
	SymbolObj*	pSymbolObj;			// 相关对象
	IMessageRevcer* pRecver;		// 消息接收器
	BOOL		bLostFlag;			// 丢包标示
	UINT		nTickCount;			// 数据包传输时间
	BOOL		bSysErrorPack;		// 系统错误数据包
	BOOL		bIsHandled;			//是否被处理过
};

class GWMarketInitPack: public GWBasePack
{
public:
	int nState;		//状态  0：市场清盘开始  1 市场清盘完成
}; 

struct GWDealInfo
{
	time_t tmTime;
	double dbPrice;
	double direct;
	double dbShoushu; // 手数
	int	   nType;	  // 性质


};

class GWCodeListPack : public GWBasePack	// 码表封包
{
public:
	SYMBOLVEC m_vecSymbols;
};

DECLARE_VECTOR(GWDealInfo, DEAL)

class GWDealPack : public GWBasePack		// 逐步成交封包
{
public:
	UINT nDealsTotalCount;					// 服务端当前Deal总数
	DEALVEC vecDeals;

	time_t BeginTM()
	{
		if (vecDeals.empty())
		{
			return 0;
		}

		return vecDeals[0].tmTime;
	}

	time_t EndTM()
	{
		if (vecDeals.empty())
		{
			return 0;
		}

		return (*vecDeals.rbegin()).tmTime;
	}
};

class GWSnapShotPack : public GWBasePack		// 快照数据
{
public:
	SnapShotData	data;

	GWSnapShotPack()
	{
		memset(&data, 0, sizeof(data));
	}

	virtual void Reset()
	{
		memset(&data, 0, sizeof(data));
		GWBasePack::Reset();
	}
};

struct SymbolSnapshotInfo						// 商品信息
{
	SymbolObj* pSymbolInfo;
	SnapShotData snapShotData;

	void operator =(const SymbolSnapshotInfo& info)
	{
		pSymbolInfo = info.pSymbolInfo;
		memcpy(&snapShotData, &info.snapShotData, sizeof(snapShotData));
	}
};

DECLARE_VECTOR(SymbolSnapshotInfo, SYMBOLSNAPSHOT)

class GWMultiSnapShotPack : public GWBasePack		// 快照数据
{
public:
	SYMBOLSNAPSHOTVEC vecSnapshot;
};

DECLARE_VECTOR(SymbolSnapshotInfo, SYMBOLINFO)

class GWMarketSortPack : public GWBasePack		// 市场排序数据
{
public:
	CString strMarketName;						// 市场名称

	SYMBOLINFOVEC vecSortSymbol;					// 数据信息
};

class GWMarketInfoPack : public GWBasePack		// 市场信息包
{
public:
	MRKETVEC	vecMarket;

};

typedef struct 
{
	long		tradedate;				// 交易日
	time_t		day;					// 时间
	double		open;					// 开盘价
	double		high;					// 最高价
	double		low;					// 最低价
	double		close;					// 收盘价
	double		vol;					// 成交量
	double		amount;					// 成交金额
	double		preclose;				// 前收
	double		totalvol;				// 总量
	double		totalamount;			// 总额
	double		avg;					// 均价

} GWKline;

DECLARE_VECTOR(GWKline, GWKLINE)

enum GWKLINE_TYPE
{
	KT_1MIN			= 0,		// 1分钟
	KT_3MIN,
	KT_5MIN,
	KT_15MIN,
	KT_30MIN,
	KT_60MIN,
	KT_120MIN,
	KT_180MIN,
	KT_240MIN,

	KT_DAY			= 20, 	// 日线
	KT_WEEK,				// 周
	KT_MONTH,				// 月
	KT_6MONTH,				// 半年
	KT_YEAR					// 年
};

class GWKLinePack : public GWBasePack		// K线数据包
{
public:
	GWKLINE_TYPE		nKType;					// K线类型		
	GWKLINEVEC			vecKlines;				// 数据数组

	time_t BeginTM()
	{
		if (vecKlines.empty())
		{
			return 0;
		}

		return vecKlines[0].day;
	}

	time_t EndTM()
	{
		if (vecKlines.empty())
		{
			return 0;
		}

		return (*vecKlines.rbegin()).day;
	}
};


// add by carrot 2013-1-24  新闻资讯
typedef struct tagNews
{
	time_t	tNews;				// 时间
	__int64	nID;				// ID
	CString	strTime;			// 时间（服务端直接以字符形式传过来）
	CString	strTitle;			// 标题
	CString strWindowCaption;	//窗口标题
	CString	strSummary;			// 摘要
	CString	strContent;			// 内容
	CString	strUrl;				// 连接
	__int64		lTimeOut;		// 显示时长
	int			nShowType;		// 新闻显示类型  0在客户端显示，1在浏览器中显示
	CString  strNewsVer;				// 新闻对应点客户端版本
	DWORD dwChannelID;	//新闻type  7:赢在大户室  1~6 银市快讯  100 森德百家
	DWORD dwImageID;	//头像ID
	CString strImage; //头像
}NEWS, *PNEWS;

DECLARE_VECTOR(NEWS, GWNEWS)
class GWNewsPack : public GWBasePack		// 新闻数据包
{
public:
	enum { N_Announcement = 0, N_Notice, N_NEWS };
	GWNEWSVEC	vecNews;
	int			nNewsType;
};

struct PriceInfo			// 分价表
{
	GWFloat fPrice;			// 价格
	int		nBuyVol;		// 买量
	int		nSellVol;		// 卖量
};

DECLARE_VECTOR(PriceInfo, PRICEINFO)
class GWPriceTablePack : public GWBasePack		// 新闻数据包
{
public:
	PRICEINFOVEC	vecPriceInfo;
};

struct OrderListInfo
{
	int m_longMin;		//多小
	int m_longMid;		//多中
	int m_longMax;		//多大
	int m_longMore;		//多超大
	int m_shortMin;
	int m_shortMid;
	int m_shortMax;
	int m_shortMore;
	__int64 m_tradeVolume;	//总量
};

class GWOrderListPack : public GWBasePack	// 龙虎榜
{
public:
	OrderListInfo	orderList;
};

// 画线分享 发送结构
struct DrawLineShared
{
	CString	title;				// 标题
	int			type;				// 类型
	CString	comment;		// 描述
	CString	obj;				// 品种
	CString	owner;			// 作者
	CString	time;				// 时间
	CString	link;				// 链接
};

class GWDrawLineShared : public GWBasePack	// 画线工具分享
{
public:
	int nRet;			// 分享返回值
};

class GWInfoBomPack : public GWBasePack	// 信息地雷数据
{
public:
	CString strMarket;			// 市场
public:
	GWINFOBOMVEC vecInfoBom;

	time_t BeginTM()
	{
		if (vecInfoBom.empty())
		{
			return 0;
		}

		return vecInfoBom[0].dwUpdateDateTime;
	}

	time_t EndTM()
	{
		if (vecInfoBom.empty())
		{
			return 0;
		}

		return (*vecInfoBom.rbegin()).dwUpdateDateTime;
	}
};

typedef struct tagServerTAItem		//来自服务器指标
{
	enum EMServerTAType
	{
		em_ServerTA_None = 0,
		em_ServerTA_BuyBuild = 0X1,	//买入开仓
		em_ServerTA_BuyKilled = 0X2,	//买入平仓
		em_ServerTA_SellBuild = 0X4,	//卖出开仓
		em_ServerTA_SellKilled = 0X8,	//卖出平仓
		em_ServerTA_All = 0XFFFFFFFF,
	};	
	DWORD dwTAID;		//指标ID
	DWORD dwTAType;		//指标类型
	time_t dwTriggerTime;	//触发时间 
	double  dfTriggerPrice;	//触发价格
	DWORD  dwBuyBuildVol;	//建议买入开仓量
	DWORD  dwBuyKilledVol;	//建议买入平仓量
	DWORD  dwSellBuildVol;	//建议卖出开仓量
	DWORD  dwSellKilleddVol;	//建议卖出平仓量
	DWORD dwTotolVol;		//持仓总量
	DWORD	dwBuyTotal;			//多单总量
	double dfBuyAvgPrice;		//多单均价
	DWORD dwSellTotal;			//空单总量
	double dfSellAvgPrice;		//空单均价
	time_t	dwTradeDate;			// 交易日
	time_t	dwUpdateTime;			// 更新时间
	TCHAR szTAName[MAX_COMMON_NAME_LEN + 1];

	DWORD dwMinIndex;		//resever
	TCHAR szSerialID[MAX_COMMON_TEXT_LEN + 1];	//单号
	double dfStopProfitPrice;		//建议止盈价
	double dfStopLossPrice;			//建议止损价
	DWORD dwResever[2];		//resever
	BYTE byReserved[128];
	bool operator==(const tagServerTAItem& item)
	{
		if (dwTAID == item.dwTAID )
		{
			return true;
		}
		else
			return false;
	}
	tagServerTAItem()
	{
		ZeroMemory(this, sizeof(tagServerTAItem));
		dwMinIndex = -1;
	}
	BOOL FormatTAType()
	{
		dwTAType = 0;
		int nVolSize = 0;
		if(dwBuyBuildVol > 0)
		{
			dwTAType |= em_ServerTA_BuyBuild;
			nVolSize ++;
		}
		if(dwBuyKilledVol > 0)
		{
			dwTAType |= em_ServerTA_BuyKilled;
			nVolSize ++;
		}
		if(dwSellBuildVol > 0)
		{
			dwTAType |= em_ServerTA_SellBuild;
			nVolSize ++;
		}
		if(dwSellKilleddVol > 0)
		{
			dwTAType |= em_ServerTA_SellKilled;
			nVolSize ++;
		}
		if(nVolSize > 2  || nVolSize <= 0)
		{
			return FALSE;
		}
		return TRUE;
	}
} GWServerTA;

DECLARE_VECTOR(GWServerTA, GWSERVERTA)

class GWServerTAPack : public GWBasePack	// 银利一号指标数据
{
public: 
public:
	GWSERVERTAVEC vecServerTA;

	time_t BeginTM()
	{
		if (vecServerTA.empty())
		{
			return 0;
		}

		return vecServerTA[0].dwUpdateTime;
	}

	time_t EndTM()
	{
		if (vecServerTA.empty())
		{
			return 0;
		}

		return (*vecServerTA.rbegin()).dwUpdateTime;
	}
};


typedef struct tagBigNews
{
	enum EMNewsStat
	{
		em_News_Normal = 0,	//普通状态
		em_News_Add = 2,
		em_News_Del = 3,
		em_News_Update = 4,
		em_News_MarkRefresh = 0XFF,//标准为刷新状态
	}; 
	INT64 dwNewsID;				// 新闻ID
	int	 nType;					// 类型  
	TCHAR szCode[MAX_COMMON_NAME_LEN + 1];	//对应的品种
	TCHAR szTitle[MAX_COMMON_TEXT_LEN + 1];			// 标题
	TCHAR szSummary[MAX_COMMON_TEXT_LEN + 1];	// 摘要
	TCHAR szLinkURL[MAX_COMMON_TEXT_LEN + 1];	// URL
	TCHAR szAuthor[MAX_COMMON_NAME_LEN + 1];			// 作者 
	TCHAR szNewsVer[MAX_COMMON_NAME_LEN + 1];			// 版本 
	time_t dwTradeDate;			// 交易日
	time_t tDeployTime;			// 发布时间
	time_t dwCreateTime;			// 创建时间
	time_t dwUpdateDateTime;		// 更新时间
	int nState;						// 状态
	DWORD dwAppendData;			//其他属性
	DWORD lTimeOut;		// 显示时长
	int nShowType;		// 新闻显示类型  0在客户端显示，1在浏览器中显示

	//预留字段，留待以后扩展，为了与前面版本的兼容，不能增加整体大小
	// 4 DWORD  + 128 BYTE
	long nSubType;			//子类型
	long nEffect;			//特效
	DWORD dwUserFlag;		//用户自用
	DWORD dwReserver0;	//预留
	TCHAR szKind[16];		//种类
	TCHAR szReserver[112];	//预留

	bool operator==(const tagBigNews& item)
	{
		if (dwNewsID == item.dwNewsID )
		{
			return true;
		}
		else
			return false;
	}
	tagBigNews()
	{ 
		ZeroMemory(this, sizeof(tagBigNews)); 
	}
} GWBigNews;

DECLARE_VECTOR(GWBigNews, GWBIGNEWS)
DECLARE_VECTOR(GWBigNews*, GWPBIGNEWS)

#define  MAX_SHOW_NEWS_NUM		100
class GWBigNewsPack : public GWBasePack	// 新闻信息数据
{
public:
	CString strCode;			// 品种
	int nNewsType;				//类型
public:
	GWBIGNEWSVEC vecBigNews;

	time_t BeginTM()
	{
		if (vecBigNews.empty())
		{
			return 0;
		}

		return vecBigNews[0].dwUpdateDateTime;
	}

	time_t EndTM()
	{
		if (vecBigNews.empty())
		{
			return 0;
		}

		return (*vecBigNews.rbegin()).dwUpdateDateTime;
	}
};

typedef struct tagItemVer
{
	int nType;	//类型
	DWORD dwVer; //版本号
	BYTE byReserver[32];
	tagItemVer()
	{
		ZeroMemory(this, sizeof(tagItemVer));
	}
}GWItemVer;

DECLARE_VECTOR(GWItemVer, GWITEMVER)
class GWCommonVerPack : public GWBasePack	// 数据版本信息
{
public: 
public:
	GWITEMVERVEC vecCommonVer;
};

typedef struct tagClearDataCommand
{
	TCHAR szMarket[MAX_COMMON_NAME_LEN + 1];
	TCHAR szSymbol[MAX_COMMON_NAME_LEN + 1];
	MarketInfo * pMarket;
	SymbolObj * pSymbol;
	int nCmdType;
	DWORD dwLastVersion;
	DWORD dwAppendData;
	tagClearDataCommand()
	{
		ZeroMemory(this, sizeof(tagClearDataCommand));
	}

}GWClearDataCommand;
class GWClearDataPack : public GWBasePack	// 清除本地缓存命令
{
public: 
	tagClearDataCommand tClearDataCmd;
public:

};
// add by carrot 2013-2-13  重要经济数据
typedef struct tagEconomicData
{
	int				nID;						// 
	CString		strName;				// 数据名称
	time_t		tPubDate;				// 发布时间
	time_t      tPoint;					// 发布时间点
	CString		strReleaseValue;	// 发布值
	CString		strExpectedValue;	// 预期值
	CString		strPreValue;			// 前值
	int				nType;					// 发布值与前值的关系 (小于,等于,大于)
	CString		strRoom;				// 直播室地址
}ECONOMICDATA, *PECONOMICDATA;

DECLARE_VECTOR(ECONOMICDATA, GWECONOMICDATA)
class GWEconomicDataPack : public GWBasePack		// 重要经济数据
{
public:
	GWECONOMICDATAVEC	vecEconomicData;
};
class GWHisEconomicDataPack : public GWBasePack
{
public:
	GWHisEconomicDataPack()
	{
		m_nCount = 0;
		m_nMaxID = 0;
	}
public:
	INT64 m_nCount;
	INT64 m_nMaxID;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 键盘精灵
enum GW_WIZZARD_OBJ_TYPE
{
	GW_COMMODITY = 0,						// 商品,该条目是一只商品
	GW_COMMAND,								// 命令，该条目是一个命令，确定会抛出一个COMMAND
};

#define WIZZARD_SHOW_SHORKEY				0x0001			// 显示快捷键
#define WIZZARD_SHOW_SYMBOL					0x0002			// 显示商品
#define WIZZARD_SHOW_ALL					0xFFFF			// 全部显示

union GW_WIZZARD_EXDATA				// 对象数据
{
	SymbolObj*		pSymbolObj;		// 商品对象
	ShortcutObj*	pShortcutObj;	// 快捷键对象
	VOID*			inner;
};

class GW_WIZZARD_OBJ						// 键盘精灵管理对象
{
public:
	GW_WIZZARD_OBJ_TYPE type;				// 类型
	CString key;							// 代码
	CString name;							// 名称
	CString shortkey;						// 快捷键

	UINT mainpriority;						// 匹配优先级
#define PRIORITY_KEY		0
#define PRIORITY_NAME		1
	UINT subpriority;						// 匹配从优先级

	GW_WIZZARD_EXDATA		exdata;			// 附加数据

	GW_WIZZARD_OBJ()
	{
		exdata.inner = NULL;
	}

	~GW_WIZZARD_OBJ()
	{
		if (type == GW_COMMAND)
		{
			delete exdata.pShortcutObj;
		}
		else if (type == GW_COMMODITY)
		{
			delete exdata.pSymbolObj;
		}
		else
		{
			ASSERT(FALSE);
			delete exdata.inner;
		}
	}
};

DECLARE_LIST(GW_WIZZARD_OBJ*, WIZZARDOBJ)
DECLARE_LIST(MarketInfo*, MARKET)
DECLARE_MAP(CString, MarketInfo*, MARKET)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct QuickJumpCommand
{
	CString		strClassName;			// 类名
	CString		strParam;				// 参数
	SymbolObj	*pSymbolObj;			// 商品对象
	BOOL		bNeedRelease;			// 是否需要是否
	CView * pActiveView;				//当前焦点活动VIEW
	QuickJumpCommand()
	{
		pSymbolObj = NULL;
		bNeedRelease = FALSE;
		pActiveView = NULL;
	}
};

enum EMCommandID
{
	em_Cmd_NoUser =  0,
	em_Cmd_Bring2TopMost = 255,
};
struct DirectCommand					// 直接命令
{
	CString strCmd;						// 命令
	DWORD dwAppendData;					//附加数据
	DirectCommand()
	{
		dwAppendData = 0;
	}
};

typedef struct GWLocalFileHeader
{
	union
	{
		long version;							// 版本号	
		char pad[128];
	} hdr;
	BOOL		 complete;						// 数据是否完整
	GWPackType	 type;							// 类型
	time_t		 update;						// 最后更新时间
	time_t		 begin;							// 数据开始时间
	time_t		 end;							// 数据结束时间
	UINT		 count;							// 数据量
	char		 reserve[256];					// 留待扩展
} GWFHDR;


struct GWObjCompareEx
{
	bool operator()(GWInfoBom& info1, GWInfoBom& info2)
	{
		return info1.dwTradeDate < info2.dwTradeDate;
	}
};

struct IMessageRevcer;

struct GWRequestInfo							// 请求内容
{
	int			nType;							// 类型
	time_t		tmBegin;						// 起点
	time_t		tmEnd;							// 终点
	SymbolObj*	pSymbolObj;
	int			nExpectCount;					// 期望数目
	BOOL		bRightDir;						// 方向
	int			nOffset;						// 偏移数目
	BOOL		bWantAll;						// 是否所有数据都要
	long		nDate;							// 请求的日期
	UINT_PTR    nKey;							// 附加数据
	IMessageRevcer* pRecv;
	HANDLE		hEvent;							// 同步对象
};

// 网络登录数据
typedef struct tagLogInfo
{	
	int		   nClientType;							// 客户端类型
	CString strClientVersion;					// 客户端版本
	CString strLogInIP;							//登陆服务器地址
	int		   nPort;									//登陆服务器端口
	CString strUserName;						//登陆的用户名称或用户手机号
	CString strUserPwd;							//登陆密码
	//以下为登陆服务器返回值
	CString strKey;								//md5(用户的手机号+密码+行情服务器key)
	CString strHQIP;								//登陆服务器根据负载自动分配的行情服务器地址
	int       nHQPort;								//登陆服务器根据负载自动分配的行情服务器端口
	CString strErr;									//登陆服务器返回的验证失败信息描述
	CString strPhoneNum;						//登陆用户手机号，需要把手机号发送给行情服务器进行登陆
	CString strUserID;							//登陆用户名
	CString strDownLoadIP;						//下载非行情数据的地址
	int		   nDownLoadPort;					//下载非行情数据的端口
	CString strDownLoadUser;					//下载非行情数据的服务器登陆用户名
	CString strDownLoadPwd;					//下载非行情数据的服务器的登陆密码
	int		   nUserType;							//用户类型
	CString strJsonInfo;							//json格式的扩展信息(模拟交易服务器地址等信息)
	tagLogInfo():nPort(0),nHQPort(0),nDownLoadPort(0),nClientType(0)
	{
	}
	BOOL IsInvalid()
	{
		return (strErr == _T(""));
		//return (nPort <= 0 || nHQPort <= 0 || nDownLoadPort <= 0 || strLogInIP.IsEmpty() || strUserName.IsEmpty() || strUserPwd.IsEmpty() || strKey.IsEmpty()
		//	|| strHQIP.IsEmpty() || strPhoneNum.IsEmpty() || strUserID.IsEmpty() || strDownLoadIP.IsEmpty() || strDownLoadUser.IsEmpty() || strDownLoadPwd.IsEmpty());
	}
}NETLOGININFO,*PNEWLOGININFO;


// add by carrot 2013-01-10
typedef struct tagEntrustClientData				// 交易客户端						-- 一键交易
{
	CString m_strText;					// 交易端描述
	CString m_strUrl;						// 下载地址
	CString m_strKey;					// GUID
	CString m_strInstallApp;			// 安装包名字
	CString m_strAppName;				// 可执行文件名称
	CString m_strVersion;				// 版本号
	CString m_strCmd;					//	命令行参数
	CString m_strStatus;				// 安装状态
	CString m_strInstallPath;			// 安装目录
	bool operator==(const tagEntrustClientData &data)
	{
		if (m_strText == data.m_strText
			&& m_strUrl == data.m_strUrl
			&& m_strKey == data.m_strKey
			&& m_strAppName == data.m_strAppName
			&& m_strVersion == data.m_strVersion)
			return true;
		else
			return false;
	}
	void EmptyData()
	{
		m_strText = "";					// 交易端描述
		m_strUrl = "";						// 下载地址
		m_strKey = "";						// GUID
		m_strInstallApp = "";				// 安装包名字
		m_strAppName = "";				// 可执行文件名称
		m_strVersion = "";				// 版本号
		m_strCmd = "";					//	命令行参数
		m_strStatus = "";					// 安装状态
		m_strInstallPath = "";			// 安装目录
	}
}ENTRUSTCLIENTDATA,*PENTRUSTCLIENTDATA;

typedef struct tagEntrustData					// 一键交易配置信息
{
	CString m_strText;			// 描述
	CString m_strName;			// 文件名
	CString m_strPath;			// 文件路径
	CString m_strVersion;		// 文件版本

	bool operator==(const tagEntrustData& data)
	{
		if ((m_strText == data.m_strText) &&
			(m_strName = data.m_strName) && 
			(m_strPath = data.m_strPath) &&
			(m_strVersion = data.m_strVersion))
			return true;
		else
			return false;
	}
}ENTRUSTDATA,*PENTRUSTDATA;

struct GWMenuItemEx
{
#define UI_MENUITEM			0
#define UI_TOOLBAR			1
	int nType;					// UI类型
	CString strText;			// 菜单文字
	CString strCommand;			// 菜单命令
	CString strKey;				// 菜单key
	CString strDestView;		//目标窗口
	HMENU	hMenu;				// 所属菜单
	BOOL	bInitState;			// 默认状态
	UINT	uID;				// 系统分配的ID
	BOOL	bManual;			// 用程序控制状态
	BOOL	bEnable;			// 状态
	BOOL	bChecked;			// 是否为选中状态
};

struct tagIndicatorItem
{
	CString strName;	//UI名称
	CString strAlias;	//K线图指标实际名称
	CString strChineseName;
	int nDescptionLineNum;
	CString strDescptionArr[10];
	BYTE byIndex;	//
};

typedef CArray<tagIndicatorItem, tagIndicatorItem&> CIndicatorArr;

#define MINLINE_BOMB_TYPE			_T("8")
#define KLINE_BOMB_TYPE				_T("4")

struct tagInfoBombItem		//信息地雷
{
	enum EMBombType
	{//显示的信息地雷类型
		em_Bomb_None,
		em_Bomb_ExpertView		= 0X1,	//专家观点
		em_Bomb_FinanceCalendar = 0X2, //财经日历
		em_Bomb_NewsHeadLine	= 0X4, //头条 
		em_Bomb_NewsBaijia	=	0X8, //百家
		em_Bomb_ALL				= 0XFFFFFFF, //头条 
	};
	DWORD dwID;			//ID
	int nType;			//类型
	DWORD dwDateTime;	//时间 time_t
	DWORD dwCreateTime; // 创建日期
	DWORD dwRankID;		//评级
	DWORD dwImageID;	//头像ID
	CString strTitle;	//标题
	CString strSummary;	//摘要
	CString strLinkURL;	//对应
	CString strAuthor;	//作者
	CString strToolTip;	//提示信息
	DWORD dwTradeDay;	//交易日 tradeday
	int dw1MinIndex;	//对应1分钟的索引

	int		nTargetUser;  //目标用户
	DWORD dwReserved[3];  //预留字段
	//以下是附加信息，由系统填充
	CRect rcBomb;		//感知区域
	DWORD dwAppendData;
	BOOL bVisible;		//是否可见
	BOOL bFocused;		//是否当前焦点
	tagInfoBombItem()
	{
		nType = em_Bomb_None;
		dwDateTime = 0;
		dwRankID = 0;
		rcBomb.SetRectEmpty();
		dwAppendData = 0;
		bVisible = FALSE;
		bFocused = FALSE;
		dw1MinIndex = -1;
	}
};

typedef CArray<tagInfoBombItem, tagInfoBombItem&> CInfoBombArr;

// 雷达预警
typedef struct tagRadarCondition
{
	int     lID;				
	time_t tTime;			// 设置时间
	CString strName;		// 预警类型
	CString strType;		// 预警条件
	double  dValue;			// 预警阀值
	int			nActive;		// 是否可用
	CString strUnit;			// 单位
}RADARCON,*PRADARCON;
DECLARE_VECTOR(RADARCON, RADARCONTION)

typedef struct tagRadarHis
{
	int     lID;				
	time_t tTime;			// 设置时间
	time_t  tActive;			// 触发时间
	CString strName;		// 预警类型
	CString strType;		// 预警条件
	double  dValue;			// 预警值
	CString strUnit;			// 单位
	double	dActiveValue;	// 触发值
}RADARHIS,*PRADARHIS;
DECLARE_VECTOR(RADARHIS, RADARHISTORY)

typedef struct tagSymbolRadarsState
{
	SymbolObj* pSymobj;
	int nState;
	BOOL bReLoad;
	BOOL bPopMsg;			// 弹出对话框
	tagSymbolRadarsState()
	{
		pSymobj = NULL;
		nState = 0;
		bReLoad = FALSE;
		bPopMsg = FALSE;
	}
}SYMBOLRADARSSTATE,* PSYMBOLRADARSSTATE;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
////////////////////// 头寸 /////////////////////////////////////////
typedef struct tagBSHoldPosition
{//多空持仓
	time_t dwDateTime;	//时间
	DWORD dwTickCount;	//ID
	DWORD dwBuyPosition;	//多头持仓
	DWORD dwSellPosition;	//空头持仓
}GWBSHoldPositon;
DECLARE_VECTOR(GWBSHoldPositon, GWBSHOLDPOSTION)

typedef struct tagBSHoldPositionEx
{//多空持仓
	SymbolObj * pSymbol;
	time_t dwDateTime;	//时间
	DWORD dwTickCount;	//ID
	double dfBuyPosition;	//多头持仓
	double dfSellPosition;	//空头持仓
	double dfRate;		//持仓（所有品种总量）占比  
}GWBSHoldPositonEx;

DECLARE_VECTOR(GWBSHoldPositonEx, GWBSHOLDPOSTIONEX) 

class GWBSHoldPositionPack : public GWBasePack		// 多空持仓
{
public:
	GWBSHOLDPOSTIONEXVEC	vecBSHoldPositionEx; //新版 多空总持仓

	GWBSHOLDPOSTIONVEC	vecBSHoldPosition;	//老版本
	BOOL bNewVer;	//是否新版
};


typedef struct tagBSTrend
{//多空趋势
	int	dwID;
	time_t dwDateTime;	//时间
	DWORD dwBuyBigDeal;	//多头大单
	DWORD dwBuyMidDeal;	//多头中单
	DWORD dwBuySmallDeal;	//多头小单
	DWORD dwSellBigDeal;	//空头大单
	DWORD dwSellMidDeal;	//空头中单
	DWORD dwSellSmallDeal;	//空头小单
	time_t dwUpdateTime;	//所属时间轴
}GWBSTrend;

DECLARE_VECTOR(GWBSTrend, GWBSTREND)



typedef struct tagBSTrendEx
{//多空趋势
	int	dwID;
	time_t dwDateTime;	//时间 
	float dfBuyFactor;	//多 分钟指建仓多单
	float dfSellFactor;//空  分钟指建仓空单
	float dfKillOIBuyFactor;;	//平仓多单
	float dfKillOISellFactor;//平仓空单
	BYTE byReserved[32];
}GWBSTrendEx;

DECLARE_VECTOR(GWBSTrendEx, GWBSTRENDEX)
class GWBSTreendPack : public GWBasePack		// 多空持仓
{ 
public:
	GWBSTRENDEXVEC	vecBSTrendEx; //新版版本 1分钟SLSP
	GWBSTRENDVEC	vecBSTrend;	//老版本 5分钟SLSP
	int nPeriod;	//周期
	BOOL bNewVer;	//是否新版
public:
	time_t BeginTM()
	{
		if (vecBSTrendEx.empty())
		{
			return 0;
		}
		return vecBSTrendEx[0].dwDateTime;
	}

	time_t EndTM()
	{
		if (vecBSTrendEx.empty())
		{
			return 0;
		}
		return (*vecBSTrendEx.rbegin()).dwDateTime;
	}
};


typedef struct tagETFPositon
{//ETF持仓
	enum
	{
		em_EFGGold = 1,	//黄金
		em_EFGSliver = 2,	//白银
	};
	int nType;		//白银 or 黄金 ETF
	int dwID;		//ID
	time_t dwDateTime;	//日期
	double dfETFPosition;	//净持仓
	double dfAdd ;	//持仓增减
}GWETFPositon;

DECLARE_VECTOR(GWETFPositon, GWETFPOSTION)

class GWETFPositionPack : public GWBasePack		// 多空持仓
{
public:
	int nETFType;	//类型
	GWETFPOSTIONVEC	vecETFPosition;
};


typedef struct tagExpertInfo	//关注列表
{ 
	enum EMFoucusType
	{
		em_Focus_On = 1,	//关注
		em_Focus_Off = 3,	//取消关注
	};
	DWORD		dwExpertID;		// 专家ID 
	TCHAR		szExpertName[32];			//专家名称 
	int  nStatus;				//状态	1 关注    3 取消关注
	time_t  dwUpdateTime;		//更新时间
	DWORD dwType;		//0 老师列表 1 直播室列表
	bool operator==(const tagExpertInfo& item)
	{
		if (dwExpertID == item.dwExpertID && dwType == item.dwType)
		{
			return true;
		}
		else
			return false;
	}
	tagExpertInfo()
	{ 
		ZeroMemory(this, sizeof(tagExpertInfo)); 
	}
} GWExpertInfo;

DECLARE_VECTOR(GWExpertInfo, GWEXPERTINFO)

class GWExpertPack : public GWBasePack	// 关注列表
{ 
public:
	GWEXPERTINFOVEC vecExpertInfo; 
};


typedef struct tagSymbolRight	//品种权限列表
{  
	TCHAR szMarket[32];		//市场
	TCHAR szSymbol[32];		//品种名称  
	DWORD dwRight;		//权限 
	tagSymbolRight()
	{ 
		ZeroMemory(this, sizeof(tagSymbolRight)); 
	}
} GWSymbolRight;

DECLARE_VECTOR(GWSymbolRight, GWSYMBOLRIGHT)

typedef struct tagTaiJiSignal
{//太极信号
	SymbolObj * pSymbol;	//品种
	time_t dwOccurTime;		//触发时间
	double dfPrice;		//当时价格
	int nSignalType;	//信号
	int nKPeriode;	//周期
	enum TaijiSignalType
	{
		OUT_KUN2KUN1_TO_UPPER = -2,		//从KUN2KUN1(KUN2_TO_KUN1) 区域向上出去
		INTO_KUN2KUN1_FROM_KUN1UPPER = -1,     	//进入KUN2KUN1(KUN2_TO_KUN1) 区域(从坤1线上方)
		INTO_QIAN1QIAN2_FROM_QIAN1LOWER = 1,  	//进入QIAN1QIAN2(QIAN1_TO_QIAN2) 区域(从乾1线下方)
		OUT_QIAN1QIAN2_TO_LOWER = 2  		//从QIAN1QIAN2(QIAN1QIAN2) 区域向下出去
	};
	tagTaiJiSignal()
	{
		ZeroMemory(this, sizeof(tagTaiJiSignal));
	}
	BOOL IsSignalOccur()
	{//是否触发了信号
		if(INTO_KUN2KUN1_FROM_KUN1UPPER == nSignalType  || 
			INTO_QIAN1QIAN2_FROM_QIAN1LOWER == nSignalType)
		{
			return TRUE;
		}
		return FALSE;
	}
}GWTaiJiSignal;


DECLARE_VECTOR(GWTaiJiSignal, GWTAIJISIGNAL)
class GWTaijiSignalPack : public GWBasePack	// 太极信号推送
{ 
public:
	GWTAIJISIGNALVEC	vecTaijiSignal;
};

//乾坤线
typedef struct tagQKSignal
{
	enum EMQKKind
	{
		em_QK_None = 0,
		em_QK_Line = 1, //乾坤线
		em_QK_Map = 2,	//乾坤图
		em_BYJZ = 3,	//博弈矩阵
	};
	enum EMQKTrendType
	{
		em_Trend_Down = -1,	//趋势向下
		em_Trend_Zero = 0,	//趋势不明
		em_Trend_Up = 1,	//趋势向下	
	};
	SymbolObj * pSymbol;//品种,本地缓存数据无效
	int nPeriod;	//周期
	int nBS;	//多空标志
	time_t dwTradeDay;	//交易日
	time_t dwLastOccurTime;//上一次信号时间
	time_t dwOccurTime; //触发时间
	time_t dwExpireTime;	//有效时间
	time_t  dwUpdateTime;	//更新时间
	int nState;		//状态，增，删
	int nSignalKind;	//信号分类
	DWORD dwResered[4];
	tagQKSignal()
	{
		ZeroMemory(this, sizeof(tagQKSignal));
	}
}GWQKSignal;
 

DECLARE_VECTOR(GWQKSignal, GWQKSIGNAL)
class GWQKSignalPack : public GWBasePack	// 乾坤线信号
{ 
public:
	int nPeriod;		//周期
	int nSignalKind;	//信号分类
	GWQKSIGNALVEC	vecQKSignal;//信号集合
	GWQKSIGNALVEC	vecQKSignalLatest;	//最新的延续信号
public:
	time_t BeginTM()
	{
		if (vecQKSignal.empty())
		{
			return 0;
		}
		return vecQKSignal[0].dwUpdateTime;
	}

	time_t EndTM()
	{
		if (vecQKSignal.empty())
		{
			return 0;
		}
		return (*vecQKSignal.rbegin()).dwUpdateTime;
	}
};


struct GWBYJZItem  //博弈矩阵
{
	SymbolObj * pSymbol;
	int nPeriod;		//周期
	int nSignalType;
	DWORD dwID;
	time_t dwUpdateTime; //时间
	DWORD dwTradeDay;	//交易日 tradeday
	int  nBS;			//买卖信号
	double dfBound1;//边界1
	double dfBound2; //边界2
	double dfReserved[2]; 
	int dwKIndex;	//对应K线的索引
	DWORD dwReserved[2];
	GWBYJZItem()
	{
		ZeroMemory(this, sizeof(GWBYJZItem));
		dwKIndex = -1;
	}
};




struct GWBYJZYZSItem  //博弈止盈止损
{
	GWBYJZItem tBYJZData;
	double dfZY; //止盈
	double dfZS;//止损
	double dfBSZS; //保守止损
	double dfJJZS;//激进止损 
	GWBYJZYZSItem()
	{
		ZeroMemory(this, sizeof(GWBYJZYZSItem));
	}
};



DECLARE_VECTOR(GWBYJZItem, GWBYJW)
DECLARE_VECTOR(GWBYJZYZSItem, GWGWBYJZYZS) 
class GWBYJZPack : public GWBasePack	// 博弈矩阵
{ 
public:
	int nPeriod;		//周期
	int nSignalKind;	//信号分类
	GWBYJWVEC	vecBYJZ;//数据集合 
	GWBYJZYZSItem tBYZSZYData;//止损止盈
public:
	time_t BeginTM()
	{
		if (vecBYJZ.empty())
		{
			return 0;
		}
		return vecBYJZ[0].dwUpdateTime;
	}

	time_t EndTM()
	{
		if (vecBYJZ.empty())
		{
			return 0;
		}
		return (*vecBYJZ.rbegin()).dwUpdateTime;
	}
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct GWObjCompare
{
	bool operator()(GWDealInfo& deal1, GWDealInfo& deal2)
	{
		return deal1.tmTime < deal2.tmTime;
	}

	bool operator()(GWKline& k1, GWKline& k2)
	{
		return k1.day < k2.day;
	}
	bool operator()(PriceInfo& info1, PriceInfo& info2)
	{
		return info1.fPrice > info2.fPrice;
	}

	bool operator()(GWInfoBom& info1, GWInfoBom& info2)
	{
		return info1.tDeployTime < info2.tDeployTime;
	}

	bool operator()(GWBigNews& info1, GWBigNews& info2)
	{
		return info1.tDeployTime < info2.tDeployTime;
	}
	bool operator()(GWServerTA& info1, GWServerTA& info2)
	{
		return info1.dwTriggerTime < info2.dwTriggerTime;
	}
	bool operator()(tagInfoBombItem& info1, tagInfoBombItem& info2)
	{
		return info1.dw1MinIndex < info2.dw1MinIndex;
	}
};