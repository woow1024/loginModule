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
	LogDisabled		= 0,			// ���� 
	LogFull			= 1,			// ����
	LogNormal		= 2,			// ��ͨ
	LogWarning		= 3,			// ����
	LogError		= 4				// ����
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
		{//ĩβ�Ƿ���������
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
			strText += _T("��");
		}
		else if (GetValue() > TEN_THOUSAND && GetValue() < TEN_MILLION )
		{
			strText.Format(_T("%.2f"), (GetValue() / TEN_THOUSAND+dfDiff));
			strText += _T("��");
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

		if (GetDigit() == v.GetDigit())				// С��λ����ֱͬ�Ӽ��Ϳ�����
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

		if (GetDigit() == v.GetDigit())				// С��λ����ֱͬ�ӼӾͿ�����
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
	__int64		iValue;			// ֵ
	int			nDigit;			// С����
	BOOL		bInit;			// �Ƿ��ʼ��
};

typedef struct tagInfoBomItem
{
	enum EMBombStat
	{
		em_Bomb_Add = 2,
		em_Bomb_Del = 3,
		em_Bomb_Update = 432,
	};
	DWORD		dwArticalID;		// ����ID
	DWORD		dwChannelID;		//Ƶ��
	int	nType;						// ����  

	time_t	tDeployTime;			// ��������
	time_t	dwTradeDate;			// ������ 
	TCHAR	strSymbol[32];			//Ʒ��
	TCHAR		strTitle[256];			// ����
	TCHAR		strSummary[256];	// ժҪ
	TCHAR		strURL[256];		//url
	time_t	dwCreateTime;			// ��������
	time_t  dwUpdateDateTime;		// ��������
	int			nState;				// ״̬
	DWORD dwRankID;					// ����
	TCHAR		strAuthor[64];		// ���� 
	DWORD dwImageID;		//ͷ��ID
	int		nTargetUser;		//Ŀ���û� 0��ȫ����1��ֱ���ң�2��δ���3������
	DWORD dwReserved[3];	//Ԥ���ֶ�
	BYTE  byReserved[256];	//Ԥ���ֶ�

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
	time_t	tmTime;				// ����ʱ��
	GWFloat  dbOpen;				// ����
	GWFloat  dbHigh;				// ���
	GWFloat  dbLow;				// ���
	GWFloat  dbClose;			// ����
	GWFloat  dbPreClose;			// ǰ��
	GWFloat  dbChange;			// �ǵ�
	GWFloat  dbChangeRate;		// �ǵ���
	GWFloat	dbAmplitude;		// ���
	GWFloat  dbAverage;			// ����
	GWFloat  dbEntrustDiff;		// ί��
	GWFloat  dbEntrustPercent;	// ί��

	GWFloat	dbPositions;		// �ֲ���

	GWFloat  dbVolume;			// �ɽ���
	GWFloat  dbAmount;			// �ɽ���

	GWFloat  dbRealVolumeNow;		//����ʵ�ʵ� ����
	GWFloat  dbVolumeNow;		// ������������ʾ��
	GWFloat  dbAmountNow;		// �ֶ� 

	GWFloat  dbBuyPrice[10];		// ��1 - ��5
	GWFloat  dbSellPrice[10];	// ��1 - ��5
	GWFloat  dbBuyVolume[10];	// ��1 - ��5	��
	GWFloat  dbSellVolume[10];	// ��1 - ��5	��
	GWFloat  dbBuyAmount[10];	// ��1 - ��5	��
	GWFloat  dbSellAmount[10];	// ��1 - ��5	��
};

struct MarketInfo;
#define  MAX_KLINE_VER_NUM			16
enum emKVerType
{
	em_KVerDay = 0,	//����
	em_KVer1Min,	//1����
	em_KVer15Min,	//15����
	em_VerInfoBomb,	//��Ϣ���� 
	em_VerServerTA,	//����һ��ָ��
	em_VerF1,		//�����ϸ
	em_VerEnd,
};
struct TRADETIME_SECTION
{
	DWORD nBegin;
	DWORD nEnd;
};
DECLARE_VECTOR(TRADETIME_SECTION, TRADETIME)			// ����ʱ���
struct SymbolObj
{
	CString strMarket;				// �г�
	CString	strCode;				// ����
	CString strName;				// ������
	CString	strCategory;			// ����
	CString strKeyWords;			// �ؼ���
	GWFloat dbLastClose;			// ������
	GWFloat dbLastSettlement;		// �����
	GWFloat dbNewSettlement;	// �µ����
	int	   nDigit;					// С��λ��
	CString strCurrencyType;		// ��������
	CString strQuoteUnit;			// ���۵�λ
	BOOL	bLastCloseInit;			// �Ƿ�ɹ���ʼ������

	MarketInfo *pMarketIfo;			// ��Ʊ�����г���Ϣ
	BOOL	bSelfSotck;				// �Ƿ���ѡ��

	SnapShotData	snapshotData;	// �������һ�ʿ���
	DWORD nKVerArr[MAX_KLINE_VER_NUM];			//K�߰汾��
	DWORD dwPrivilege;				//Ȩ�޿���
	DWORD dwAllRight;				//�Ƿ���� SLSPȨ��
	//GWFloat dbDiancha;		//���	ͨ�������۳������
	GWFloat dbShouxufei;	//������	�������֮��
	GWFloat dbBaozhengjin;	//��֤�����	����ٷ�֮��
	GWFloat dbUnit;		//��λ 1��
	
	//TT �汾��չ
	CString strGuapaiFang;		//���Ʒ�
	CString strTradetype;		//�ֻ��ɻع�
	CString strDir;		//�ջ�����
	CString strApplytime; //���տ����ޣ�180��)
	CString strReadytime; //׼����(30�죩
	CString strBrand;		//���Ϲ��ꡢ�����������׼Ҫ���Ʒ��
	enum EMSymbolType
	{
		em_Symbol_Type_PreciousMetals = 0, //�����
		em_Symbol_Type__Stock,	//��Ʊ
		em_Symbol_Type_Index,	//ָ��
		em_Symbol_Type_Futures,	//�ڻ�Foreign currency
		em_Symbol_Type_ForeignCurrency, //���
		em_Symbol_Type_Warrant,	//Ȩ֤
		em_Symbol_Type_Bond,	//ծȯ
		em_Symbol_Type_Other1,
		em_Symbol_Type_Other2,
		em_Symbol_Type_Other3,
	};
	enum EMPrivilege
	{
		em_Symbol_Privilege_TA = 0X1, //�Ƿ��������һ������
		em_Symbol_Privilege_SLSP = 0X2, //�Ƿ����SLSP����
		em_Symbol_Privilege__Other1  = 0X4,
		em_Symbol_Privilege__Other2 = 0X8,
		em_Symbol_Privilege__Other3 = 0X10,
	};
	int nSymbolType;	//����  0�� �����   1 ��Ʊ 2 ָ��  3�ڻ�...
	TRADETIMEVEC vecTradeTime;		// ����ʱ��� HHMM ��ʽ
	TRADETIMEVEC vecTradeTimeFull;	// ���콻������ʱ�Σ�time_t ��ʽ

	CString	strParent;				// ���ڵ�
	CSTRINGVEC vecChildren;			//  �ӽڵ�

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
		if (symbolObj.strCode == strCode)		// ������ͬ
		{
			if (symbolObj.strMarket == strMarket)	// �г���ͬ
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
	{//��ʾ˳�� 
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
	{//����˳��
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
	//UINT command;							// ����
	CString command;						// ��������
} ShortcutObj;



struct MarketInfo
{
	CString strMarketName;
	CString strCName;				// ��������
	CString strCurrencyType;		// ��������
	CString strQuoteUnit;			// ���۵�λ
	long    nLastTradeDate;			//��һ��������
	long		nTradeDate;			// �г�����ʱ�� 
	TRADETIMEVEC vecTradeTime;		// ����ʱ��� HHMM ��ʽ
	TRADETIMEVEC vecTradeTimeFull;	// ���콻������ʱ�Σ�time_t ��ʽ
	SYMBOLVEC	vecSymbols;			// �г��¹�Ʊ����

	BOOL	bOmit;					// �Ƿ񱻺��Ե��г�
	BOOL	bVirtual;				// �Ƿ������г�
	int nTradeGearNum;			//������λ
};

DECLARE_VECTOR(MarketInfo*, MRKET)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum GWPackType		// ���ݰ�����
{
	PT_SECURITY	= 0,		// ���
	PT_DEAL,				// ���
	PT_PUSHEDSNAPSHOT,		// ���Ϳ���
	PT_SNAPSHOT,			// �������
	PT_MARKTESORTDATA,		// �г���������
	PT_KLINE,				// K������
	PT_MARKETINFO,			// �г���Ϣ
	PT_MARKETINIT,			// �г�����
	PT_MULTISNAPSHOT,		// �������
	PT_PRECLOSE_CHANGE,		// ���ճ�ʼ��

	PT_DAYKLINE,			// ��������
	PT_NEWS,				// ���Ź���
	PT_NEWS_RB,			// ����(���½ǵ���)

	PT_PRICETABLE,			// �ּ۱�
	PT_ORDERLIST,			// ������
	PT_SYMBOLNEWS,			// ��Ʊ��Ӧ����

	PT_DRAWLINE_SHARED,		// ���߷��� (�Ǽ�ʱͨ��)
	PT_INFOBOM,						// ������Ϣ����
	PT_INFOBOMBBYDATE,				// ָ��������Ϣ����
	PT_INFOBOMHIS,					// ��ʷ��Ϣ����
	PT_INFOBOM_PUSH,						// ��Ϣ����(����)

	PT_SERVERTA_BYDATE,				// ����һ�ŷ�ʱָ������
	PT_SERVERTA_HISTORY,				// ����һ����ʷָ������
	PT_SERVERTA_PUSH,				// ����һ����������

	PT_BIGNEWS_BYDATE,				// ��������
	PT_BIGNEWS_PUSH,				// ������������
	PT_ECONOMICDATA,				// ��Ҫ��������
	PT_HISECONOMICDATA,		// ��ʱ��Ҫ������������
	PT_COMMONVER,			// �������ݰ汾��Ϣ
	PT_CLEARLOCALDATA,		//��ձ�������
	PT_NEWS_CUTOMIZED,		// ���߷��� (�Ǽ�ʱͨ��)

	
	//����type �����˱���һ�� by w.s.h
	PROTID_BSHOLDPOSITION_REQUEST = 106,		// ��ճֲ�����
	PROTID_BSHOLDPOSITION_PUSH = 107,		// ��ճֲ�����
	PROTID_BSTREND_REQUEST = 108,		// �����������
	PROTID_BSTREND_PUSH = 109,		// �����������
	PROTID_ETFPOSITION_REQUEST = 104,	// ETF�ֲ�����
	PROTID_ETFPOSITION_PUSH = 105,		// ETF�ֲ�����

	PROTID_LSP_POSITION_PUSH = 114,	//��Э�飬 �ֱֲܳ�����
	PROTID_LSP_POSITION_REQUEST = 115,

	PROTID_LSP_TREND_PUSH = 116,	//ÿ����SLSP
	PROTID_LSP_TREND_REQUEST = 117,	//ÿ����SLSP����

	PROTID_TAIJISIGNAL_PUSH = 150,		// ̫���ź�����
	PROTID_QKSIGNAL_PUSH = 151,		// Ǭ���ź�����
	PROTID_QKSIGNAL_REQUEST = 120,		// Ǭ���ź�����	

	PROTID_QK_MAP_PUSH = 156,		// Ǭ��ͼ����
	PROTID_QK_MAP_REQUEST = 157,		// Ǭ��ͼ����

	PROTID_BYJZ_PUSH = 158,		// Ǭ��ͼ����
	PROTID_BYJZ_REQUEST = 159,		// Ǭ��ͼ����


	PT_UNINIT,				// δ��ʼ������
};

enum GWMINUTELINE_SUB_TYPE			// ������������
{
	MST_CURRENT_DAY		= 1,		// ��������
	MST_HISTORY_DAY,				// ��ʷĳ������
	MST_MULTI_DAY,					// ��������
	MST_SERIES,						// ��������
	MST_TRADEDATE					// ��������
};

enum GWINITINFO_SUB_TYPE			// ��ʼ���г�������
{
	IST_INITSINGLE		= 1,		// �����г���ʼ��
	IST_INITALL						// ȫ���г���ʼ��
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

	int			nReqID;				// ������ID
	GWPackType	nType;				// ������
	UINT		nCount;				// ������
	CString		strPackName;		// ������
	ULONG_PTR	nPackKey;			// �û���������ʱ���ݵ�Key����������ֱ�ӷ��ظ��û�
	ULONG_PTR	nSysKey;			// ϵͳKey
	SymbolObj*	pSymbolObj;			// ��ض���
	IMessageRevcer* pRecver;		// ��Ϣ������
	BOOL		bLostFlag;			// ������ʾ
	UINT		nTickCount;			// ���ݰ�����ʱ��
	BOOL		bSysErrorPack;		// ϵͳ�������ݰ�
	BOOL		bIsHandled;			//�Ƿ񱻴����
};

class GWMarketInitPack: public GWBasePack
{
public:
	int nState;		//״̬  0���г����̿�ʼ  1 �г��������
}; 

struct GWDealInfo
{
	time_t tmTime;
	double dbPrice;
	double direct;
	double dbShoushu; // ����
	int	   nType;	  // ����


};

class GWCodeListPack : public GWBasePack	// �����
{
public:
	SYMBOLVEC m_vecSymbols;
};

DECLARE_VECTOR(GWDealInfo, DEAL)

class GWDealPack : public GWBasePack		// �𲽳ɽ����
{
public:
	UINT nDealsTotalCount;					// ����˵�ǰDeal����
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

class GWSnapShotPack : public GWBasePack		// ��������
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

struct SymbolSnapshotInfo						// ��Ʒ��Ϣ
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

class GWMultiSnapShotPack : public GWBasePack		// ��������
{
public:
	SYMBOLSNAPSHOTVEC vecSnapshot;
};

DECLARE_VECTOR(SymbolSnapshotInfo, SYMBOLINFO)

class GWMarketSortPack : public GWBasePack		// �г���������
{
public:
	CString strMarketName;						// �г�����

	SYMBOLINFOVEC vecSortSymbol;					// ������Ϣ
};

class GWMarketInfoPack : public GWBasePack		// �г���Ϣ��
{
public:
	MRKETVEC	vecMarket;

};

typedef struct 
{
	long		tradedate;				// ������
	time_t		day;					// ʱ��
	double		open;					// ���̼�
	double		high;					// ��߼�
	double		low;					// ��ͼ�
	double		close;					// ���̼�
	double		vol;					// �ɽ���
	double		amount;					// �ɽ����
	double		preclose;				// ǰ��
	double		totalvol;				// ����
	double		totalamount;			// �ܶ�
	double		avg;					// ����

} GWKline;

DECLARE_VECTOR(GWKline, GWKLINE)

enum GWKLINE_TYPE
{
	KT_1MIN			= 0,		// 1����
	KT_3MIN,
	KT_5MIN,
	KT_15MIN,
	KT_30MIN,
	KT_60MIN,
	KT_120MIN,
	KT_180MIN,
	KT_240MIN,

	KT_DAY			= 20, 	// ����
	KT_WEEK,				// ��
	KT_MONTH,				// ��
	KT_6MONTH,				// ����
	KT_YEAR					// ��
};

class GWKLinePack : public GWBasePack		// K�����ݰ�
{
public:
	GWKLINE_TYPE		nKType;					// K������		
	GWKLINEVEC			vecKlines;				// ��������

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


// add by carrot 2013-1-24  ������Ѷ
typedef struct tagNews
{
	time_t	tNews;				// ʱ��
	__int64	nID;				// ID
	CString	strTime;			// ʱ�䣨�����ֱ�����ַ���ʽ��������
	CString	strTitle;			// ����
	CString strWindowCaption;	//���ڱ���
	CString	strSummary;			// ժҪ
	CString	strContent;			// ����
	CString	strUrl;				// ����
	__int64		lTimeOut;		// ��ʾʱ��
	int			nShowType;		// ������ʾ����  0�ڿͻ�����ʾ��1�����������ʾ
	CString  strNewsVer;				// ���Ŷ�Ӧ��ͻ��˰汾
	DWORD dwChannelID;	//����type  7:Ӯ�ڴ���  1~6 ���п�Ѷ  100 ɭ�°ټ�
	DWORD dwImageID;	//ͷ��ID
	CString strImage; //ͷ��
}NEWS, *PNEWS;

DECLARE_VECTOR(NEWS, GWNEWS)
class GWNewsPack : public GWBasePack		// �������ݰ�
{
public:
	enum { N_Announcement = 0, N_Notice, N_NEWS };
	GWNEWSVEC	vecNews;
	int			nNewsType;
};

struct PriceInfo			// �ּ۱�
{
	GWFloat fPrice;			// �۸�
	int		nBuyVol;		// ����
	int		nSellVol;		// ����
};

DECLARE_VECTOR(PriceInfo, PRICEINFO)
class GWPriceTablePack : public GWBasePack		// �������ݰ�
{
public:
	PRICEINFOVEC	vecPriceInfo;
};

struct OrderListInfo
{
	int m_longMin;		//��С
	int m_longMid;		//����
	int m_longMax;		//���
	int m_longMore;		//�೬��
	int m_shortMin;
	int m_shortMid;
	int m_shortMax;
	int m_shortMore;
	__int64 m_tradeVolume;	//����
};

class GWOrderListPack : public GWBasePack	// ������
{
public:
	OrderListInfo	orderList;
};

// ���߷��� ���ͽṹ
struct DrawLineShared
{
	CString	title;				// ����
	int			type;				// ����
	CString	comment;		// ����
	CString	obj;				// Ʒ��
	CString	owner;			// ����
	CString	time;				// ʱ��
	CString	link;				// ����
};

class GWDrawLineShared : public GWBasePack	// ���߹��߷���
{
public:
	int nRet;			// ������ֵ
};

class GWInfoBomPack : public GWBasePack	// ��Ϣ��������
{
public:
	CString strMarket;			// �г�
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

typedef struct tagServerTAItem		//���Է�����ָ��
{
	enum EMServerTAType
	{
		em_ServerTA_None = 0,
		em_ServerTA_BuyBuild = 0X1,	//���뿪��
		em_ServerTA_BuyKilled = 0X2,	//����ƽ��
		em_ServerTA_SellBuild = 0X4,	//��������
		em_ServerTA_SellKilled = 0X8,	//����ƽ��
		em_ServerTA_All = 0XFFFFFFFF,
	};	
	DWORD dwTAID;		//ָ��ID
	DWORD dwTAType;		//ָ������
	time_t dwTriggerTime;	//����ʱ�� 
	double  dfTriggerPrice;	//�����۸�
	DWORD  dwBuyBuildVol;	//�������뿪����
	DWORD  dwBuyKilledVol;	//��������ƽ����
	DWORD  dwSellBuildVol;	//��������������
	DWORD  dwSellKilleddVol;	//��������ƽ����
	DWORD dwTotolVol;		//�ֲ�����
	DWORD	dwBuyTotal;			//�൥����
	double dfBuyAvgPrice;		//�൥����
	DWORD dwSellTotal;			//�յ�����
	double dfSellAvgPrice;		//�յ�����
	time_t	dwTradeDate;			// ������
	time_t	dwUpdateTime;			// ����ʱ��
	TCHAR szTAName[MAX_COMMON_NAME_LEN + 1];

	DWORD dwMinIndex;		//resever
	TCHAR szSerialID[MAX_COMMON_TEXT_LEN + 1];	//����
	double dfStopProfitPrice;		//����ֹӯ��
	double dfStopLossPrice;			//����ֹ���
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

class GWServerTAPack : public GWBasePack	// ����һ��ָ������
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
		em_News_Normal = 0,	//��ͨ״̬
		em_News_Add = 2,
		em_News_Del = 3,
		em_News_Update = 4,
		em_News_MarkRefresh = 0XFF,//��׼Ϊˢ��״̬
	}; 
	INT64 dwNewsID;				// ����ID
	int	 nType;					// ����  
	TCHAR szCode[MAX_COMMON_NAME_LEN + 1];	//��Ӧ��Ʒ��
	TCHAR szTitle[MAX_COMMON_TEXT_LEN + 1];			// ����
	TCHAR szSummary[MAX_COMMON_TEXT_LEN + 1];	// ժҪ
	TCHAR szLinkURL[MAX_COMMON_TEXT_LEN + 1];	// URL
	TCHAR szAuthor[MAX_COMMON_NAME_LEN + 1];			// ���� 
	TCHAR szNewsVer[MAX_COMMON_NAME_LEN + 1];			// �汾 
	time_t dwTradeDate;			// ������
	time_t tDeployTime;			// ����ʱ��
	time_t dwCreateTime;			// ����ʱ��
	time_t dwUpdateDateTime;		// ����ʱ��
	int nState;						// ״̬
	DWORD dwAppendData;			//��������
	DWORD lTimeOut;		// ��ʾʱ��
	int nShowType;		// ������ʾ����  0�ڿͻ�����ʾ��1�����������ʾ

	//Ԥ���ֶΣ������Ժ���չ��Ϊ����ǰ��汾�ļ��ݣ��������������С
	// 4 DWORD  + 128 BYTE
	long nSubType;			//������
	long nEffect;			//��Ч
	DWORD dwUserFlag;		//�û�����
	DWORD dwReserver0;	//Ԥ��
	TCHAR szKind[16];		//����
	TCHAR szReserver[112];	//Ԥ��

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
class GWBigNewsPack : public GWBasePack	// ������Ϣ����
{
public:
	CString strCode;			// Ʒ��
	int nNewsType;				//����
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
	int nType;	//����
	DWORD dwVer; //�汾��
	BYTE byReserver[32];
	tagItemVer()
	{
		ZeroMemory(this, sizeof(tagItemVer));
	}
}GWItemVer;

DECLARE_VECTOR(GWItemVer, GWITEMVER)
class GWCommonVerPack : public GWBasePack	// ���ݰ汾��Ϣ
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
class GWClearDataPack : public GWBasePack	// ������ػ�������
{
public: 
	tagClearDataCommand tClearDataCmd;
public:

};
// add by carrot 2013-2-13  ��Ҫ��������
typedef struct tagEconomicData
{
	int				nID;						// 
	CString		strName;				// ��������
	time_t		tPubDate;				// ����ʱ��
	time_t      tPoint;					// ����ʱ���
	CString		strReleaseValue;	// ����ֵ
	CString		strExpectedValue;	// Ԥ��ֵ
	CString		strPreValue;			// ǰֵ
	int				nType;					// ����ֵ��ǰֵ�Ĺ�ϵ (С��,����,����)
	CString		strRoom;				// ֱ���ҵ�ַ
}ECONOMICDATA, *PECONOMICDATA;

DECLARE_VECTOR(ECONOMICDATA, GWECONOMICDATA)
class GWEconomicDataPack : public GWBasePack		// ��Ҫ��������
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
// ���̾���
enum GW_WIZZARD_OBJ_TYPE
{
	GW_COMMODITY = 0,						// ��Ʒ,����Ŀ��һֻ��Ʒ
	GW_COMMAND,								// �������Ŀ��һ�����ȷ�����׳�һ��COMMAND
};

#define WIZZARD_SHOW_SHORKEY				0x0001			// ��ʾ��ݼ�
#define WIZZARD_SHOW_SYMBOL					0x0002			// ��ʾ��Ʒ
#define WIZZARD_SHOW_ALL					0xFFFF			// ȫ����ʾ

union GW_WIZZARD_EXDATA				// ��������
{
	SymbolObj*		pSymbolObj;		// ��Ʒ����
	ShortcutObj*	pShortcutObj;	// ��ݼ�����
	VOID*			inner;
};

class GW_WIZZARD_OBJ						// ���̾���������
{
public:
	GW_WIZZARD_OBJ_TYPE type;				// ����
	CString key;							// ����
	CString name;							// ����
	CString shortkey;						// ��ݼ�

	UINT mainpriority;						// ƥ�����ȼ�
#define PRIORITY_KEY		0
#define PRIORITY_NAME		1
	UINT subpriority;						// ƥ������ȼ�

	GW_WIZZARD_EXDATA		exdata;			// ��������

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
	CString		strClassName;			// ����
	CString		strParam;				// ����
	SymbolObj	*pSymbolObj;			// ��Ʒ����
	BOOL		bNeedRelease;			// �Ƿ���Ҫ�Ƿ�
	CView * pActiveView;				//��ǰ����VIEW
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
struct DirectCommand					// ֱ������
{
	CString strCmd;						// ����
	DWORD dwAppendData;					//��������
	DirectCommand()
	{
		dwAppendData = 0;
	}
};

typedef struct GWLocalFileHeader
{
	union
	{
		long version;							// �汾��	
		char pad[128];
	} hdr;
	BOOL		 complete;						// �����Ƿ�����
	GWPackType	 type;							// ����
	time_t		 update;						// ������ʱ��
	time_t		 begin;							// ���ݿ�ʼʱ��
	time_t		 end;							// ���ݽ���ʱ��
	UINT		 count;							// ������
	char		 reserve[256];					// ������չ
} GWFHDR;


struct GWObjCompareEx
{
	bool operator()(GWInfoBom& info1, GWInfoBom& info2)
	{
		return info1.dwTradeDate < info2.dwTradeDate;
	}
};

struct IMessageRevcer;

struct GWRequestInfo							// ��������
{
	int			nType;							// ����
	time_t		tmBegin;						// ���
	time_t		tmEnd;							// �յ�
	SymbolObj*	pSymbolObj;
	int			nExpectCount;					// ������Ŀ
	BOOL		bRightDir;						// ����
	int			nOffset;						// ƫ����Ŀ
	BOOL		bWantAll;						// �Ƿ��������ݶ�Ҫ
	long		nDate;							// ���������
	UINT_PTR    nKey;							// ��������
	IMessageRevcer* pRecv;
	HANDLE		hEvent;							// ͬ������
};

// �����¼����
typedef struct tagLogInfo
{	
	int		   nClientType;							// �ͻ�������
	CString strClientVersion;					// �ͻ��˰汾
	CString strLogInIP;							//��½��������ַ
	int		   nPort;									//��½�������˿�
	CString strUserName;						//��½���û����ƻ��û��ֻ���
	CString strUserPwd;							//��½����
	//����Ϊ��½����������ֵ
	CString strKey;								//md5(�û����ֻ���+����+���������key)
	CString strHQIP;								//��½���������ݸ����Զ�����������������ַ
	int       nHQPort;								//��½���������ݸ����Զ����������������˿�
	CString strErr;									//��½���������ص���֤ʧ����Ϣ����
	CString strPhoneNum;						//��½�û��ֻ��ţ���Ҫ���ֻ��ŷ��͸�������������е�½
	CString strUserID;							//��½�û���
	CString strDownLoadIP;						//���ط��������ݵĵ�ַ
	int		   nDownLoadPort;					//���ط��������ݵĶ˿�
	CString strDownLoadUser;					//���ط��������ݵķ�������½�û���
	CString strDownLoadPwd;					//���ط��������ݵķ������ĵ�½����
	int		   nUserType;							//�û�����
	CString strJsonInfo;							//json��ʽ����չ��Ϣ(ģ�⽻�׷�������ַ����Ϣ)
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
typedef struct tagEntrustClientData				// ���׿ͻ���						-- һ������
{
	CString m_strText;					// ���׶�����
	CString m_strUrl;						// ���ص�ַ
	CString m_strKey;					// GUID
	CString m_strInstallApp;			// ��װ������
	CString m_strAppName;				// ��ִ���ļ�����
	CString m_strVersion;				// �汾��
	CString m_strCmd;					//	�����в���
	CString m_strStatus;				// ��װ״̬
	CString m_strInstallPath;			// ��װĿ¼
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
		m_strText = "";					// ���׶�����
		m_strUrl = "";						// ���ص�ַ
		m_strKey = "";						// GUID
		m_strInstallApp = "";				// ��װ������
		m_strAppName = "";				// ��ִ���ļ�����
		m_strVersion = "";				// �汾��
		m_strCmd = "";					//	�����в���
		m_strStatus = "";					// ��װ״̬
		m_strInstallPath = "";			// ��װĿ¼
	}
}ENTRUSTCLIENTDATA,*PENTRUSTCLIENTDATA;

typedef struct tagEntrustData					// һ������������Ϣ
{
	CString m_strText;			// ����
	CString m_strName;			// �ļ���
	CString m_strPath;			// �ļ�·��
	CString m_strVersion;		// �ļ��汾

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
	int nType;					// UI����
	CString strText;			// �˵�����
	CString strCommand;			// �˵�����
	CString strKey;				// �˵�key
	CString strDestView;		//Ŀ�괰��
	HMENU	hMenu;				// �����˵�
	BOOL	bInitState;			// Ĭ��״̬
	UINT	uID;				// ϵͳ�����ID
	BOOL	bManual;			// �ó������״̬
	BOOL	bEnable;			// ״̬
	BOOL	bChecked;			// �Ƿ�Ϊѡ��״̬
};

struct tagIndicatorItem
{
	CString strName;	//UI����
	CString strAlias;	//K��ͼָ��ʵ������
	CString strChineseName;
	int nDescptionLineNum;
	CString strDescptionArr[10];
	BYTE byIndex;	//
};

typedef CArray<tagIndicatorItem, tagIndicatorItem&> CIndicatorArr;

#define MINLINE_BOMB_TYPE			_T("8")
#define KLINE_BOMB_TYPE				_T("4")

struct tagInfoBombItem		//��Ϣ����
{
	enum EMBombType
	{//��ʾ����Ϣ��������
		em_Bomb_None,
		em_Bomb_ExpertView		= 0X1,	//ר�ҹ۵�
		em_Bomb_FinanceCalendar = 0X2, //�ƾ�����
		em_Bomb_NewsHeadLine	= 0X4, //ͷ�� 
		em_Bomb_NewsBaijia	=	0X8, //�ټ�
		em_Bomb_ALL				= 0XFFFFFFF, //ͷ�� 
	};
	DWORD dwID;			//ID
	int nType;			//����
	DWORD dwDateTime;	//ʱ�� time_t
	DWORD dwCreateTime; // ��������
	DWORD dwRankID;		//����
	DWORD dwImageID;	//ͷ��ID
	CString strTitle;	//����
	CString strSummary;	//ժҪ
	CString strLinkURL;	//��Ӧ
	CString strAuthor;	//����
	CString strToolTip;	//��ʾ��Ϣ
	DWORD dwTradeDay;	//������ tradeday
	int dw1MinIndex;	//��Ӧ1���ӵ�����

	int		nTargetUser;  //Ŀ���û�
	DWORD dwReserved[3];  //Ԥ���ֶ�
	//�����Ǹ�����Ϣ����ϵͳ���
	CRect rcBomb;		//��֪����
	DWORD dwAppendData;
	BOOL bVisible;		//�Ƿ�ɼ�
	BOOL bFocused;		//�Ƿ�ǰ����
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

// �״�Ԥ��
typedef struct tagRadarCondition
{
	int     lID;				
	time_t tTime;			// ����ʱ��
	CString strName;		// Ԥ������
	CString strType;		// Ԥ������
	double  dValue;			// Ԥ����ֵ
	int			nActive;		// �Ƿ����
	CString strUnit;			// ��λ
}RADARCON,*PRADARCON;
DECLARE_VECTOR(RADARCON, RADARCONTION)

typedef struct tagRadarHis
{
	int     lID;				
	time_t tTime;			// ����ʱ��
	time_t  tActive;			// ����ʱ��
	CString strName;		// Ԥ������
	CString strType;		// Ԥ������
	double  dValue;			// Ԥ��ֵ
	CString strUnit;			// ��λ
	double	dActiveValue;	// ����ֵ
}RADARHIS,*PRADARHIS;
DECLARE_VECTOR(RADARHIS, RADARHISTORY)

typedef struct tagSymbolRadarsState
{
	SymbolObj* pSymobj;
	int nState;
	BOOL bReLoad;
	BOOL bPopMsg;			// �����Ի���
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
////////////////////// ͷ�� /////////////////////////////////////////
typedef struct tagBSHoldPosition
{//��ճֲ�
	time_t dwDateTime;	//ʱ��
	DWORD dwTickCount;	//ID
	DWORD dwBuyPosition;	//��ͷ�ֲ�
	DWORD dwSellPosition;	//��ͷ�ֲ�
}GWBSHoldPositon;
DECLARE_VECTOR(GWBSHoldPositon, GWBSHOLDPOSTION)

typedef struct tagBSHoldPositionEx
{//��ճֲ�
	SymbolObj * pSymbol;
	time_t dwDateTime;	//ʱ��
	DWORD dwTickCount;	//ID
	double dfBuyPosition;	//��ͷ�ֲ�
	double dfSellPosition;	//��ͷ�ֲ�
	double dfRate;		//�ֲ֣�����Ʒ��������ռ��  
}GWBSHoldPositonEx;

DECLARE_VECTOR(GWBSHoldPositonEx, GWBSHOLDPOSTIONEX) 

class GWBSHoldPositionPack : public GWBasePack		// ��ճֲ�
{
public:
	GWBSHOLDPOSTIONEXVEC	vecBSHoldPositionEx; //�°� ����ֲܳ�

	GWBSHOLDPOSTIONVEC	vecBSHoldPosition;	//�ϰ汾
	BOOL bNewVer;	//�Ƿ��°�
};


typedef struct tagBSTrend
{//�������
	int	dwID;
	time_t dwDateTime;	//ʱ��
	DWORD dwBuyBigDeal;	//��ͷ��
	DWORD dwBuyMidDeal;	//��ͷ�е�
	DWORD dwBuySmallDeal;	//��ͷС��
	DWORD dwSellBigDeal;	//��ͷ��
	DWORD dwSellMidDeal;	//��ͷ�е�
	DWORD dwSellSmallDeal;	//��ͷС��
	time_t dwUpdateTime;	//����ʱ����
}GWBSTrend;

DECLARE_VECTOR(GWBSTrend, GWBSTREND)



typedef struct tagBSTrendEx
{//�������
	int	dwID;
	time_t dwDateTime;	//ʱ�� 
	float dfBuyFactor;	//�� ����ָ���ֶ൥
	float dfSellFactor;//��  ����ָ���ֿյ�
	float dfKillOIBuyFactor;;	//ƽ�ֶ൥
	float dfKillOISellFactor;//ƽ�ֿյ�
	BYTE byReserved[32];
}GWBSTrendEx;

DECLARE_VECTOR(GWBSTrendEx, GWBSTRENDEX)
class GWBSTreendPack : public GWBasePack		// ��ճֲ�
{ 
public:
	GWBSTRENDEXVEC	vecBSTrendEx; //�°�汾 1����SLSP
	GWBSTRENDVEC	vecBSTrend;	//�ϰ汾 5����SLSP
	int nPeriod;	//����
	BOOL bNewVer;	//�Ƿ��°�
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
{//ETF�ֲ�
	enum
	{
		em_EFGGold = 1,	//�ƽ�
		em_EFGSliver = 2,	//����
	};
	int nType;		//���� or �ƽ� ETF
	int dwID;		//ID
	time_t dwDateTime;	//����
	double dfETFPosition;	//���ֲ�
	double dfAdd ;	//�ֲ�����
}GWETFPositon;

DECLARE_VECTOR(GWETFPositon, GWETFPOSTION)

class GWETFPositionPack : public GWBasePack		// ��ճֲ�
{
public:
	int nETFType;	//����
	GWETFPOSTIONVEC	vecETFPosition;
};


typedef struct tagExpertInfo	//��ע�б�
{ 
	enum EMFoucusType
	{
		em_Focus_On = 1,	//��ע
		em_Focus_Off = 3,	//ȡ����ע
	};
	DWORD		dwExpertID;		// ר��ID 
	TCHAR		szExpertName[32];			//ר������ 
	int  nStatus;				//״̬	1 ��ע    3 ȡ����ע
	time_t  dwUpdateTime;		//����ʱ��
	DWORD dwType;		//0 ��ʦ�б� 1 ֱ�����б�
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

class GWExpertPack : public GWBasePack	// ��ע�б�
{ 
public:
	GWEXPERTINFOVEC vecExpertInfo; 
};


typedef struct tagSymbolRight	//Ʒ��Ȩ���б�
{  
	TCHAR szMarket[32];		//�г�
	TCHAR szSymbol[32];		//Ʒ������  
	DWORD dwRight;		//Ȩ�� 
	tagSymbolRight()
	{ 
		ZeroMemory(this, sizeof(tagSymbolRight)); 
	}
} GWSymbolRight;

DECLARE_VECTOR(GWSymbolRight, GWSYMBOLRIGHT)

typedef struct tagTaiJiSignal
{//̫���ź�
	SymbolObj * pSymbol;	//Ʒ��
	time_t dwOccurTime;		//����ʱ��
	double dfPrice;		//��ʱ�۸�
	int nSignalType;	//�ź�
	int nKPeriode;	//����
	enum TaijiSignalType
	{
		OUT_KUN2KUN1_TO_UPPER = -2,		//��KUN2KUN1(KUN2_TO_KUN1) �������ϳ�ȥ
		INTO_KUN2KUN1_FROM_KUN1UPPER = -1,     	//����KUN2KUN1(KUN2_TO_KUN1) ����(����1���Ϸ�)
		INTO_QIAN1QIAN2_FROM_QIAN1LOWER = 1,  	//����QIAN1QIAN2(QIAN1_TO_QIAN2) ����(��Ǭ1���·�)
		OUT_QIAN1QIAN2_TO_LOWER = 2  		//��QIAN1QIAN2(QIAN1QIAN2) �������³�ȥ
	};
	tagTaiJiSignal()
	{
		ZeroMemory(this, sizeof(tagTaiJiSignal));
	}
	BOOL IsSignalOccur()
	{//�Ƿ񴥷����ź�
		if(INTO_KUN2KUN1_FROM_KUN1UPPER == nSignalType  || 
			INTO_QIAN1QIAN2_FROM_QIAN1LOWER == nSignalType)
		{
			return TRUE;
		}
		return FALSE;
	}
}GWTaiJiSignal;


DECLARE_VECTOR(GWTaiJiSignal, GWTAIJISIGNAL)
class GWTaijiSignalPack : public GWBasePack	// ̫���ź�����
{ 
public:
	GWTAIJISIGNALVEC	vecTaijiSignal;
};

//Ǭ����
typedef struct tagQKSignal
{
	enum EMQKKind
	{
		em_QK_None = 0,
		em_QK_Line = 1, //Ǭ����
		em_QK_Map = 2,	//Ǭ��ͼ
		em_BYJZ = 3,	//���ľ���
	};
	enum EMQKTrendType
	{
		em_Trend_Down = -1,	//��������
		em_Trend_Zero = 0,	//���Ʋ���
		em_Trend_Up = 1,	//��������	
	};
	SymbolObj * pSymbol;//Ʒ��,���ػ���������Ч
	int nPeriod;	//����
	int nBS;	//��ձ�־
	time_t dwTradeDay;	//������
	time_t dwLastOccurTime;//��һ���ź�ʱ��
	time_t dwOccurTime; //����ʱ��
	time_t dwExpireTime;	//��Чʱ��
	time_t  dwUpdateTime;	//����ʱ��
	int nState;		//״̬������ɾ
	int nSignalKind;	//�źŷ���
	DWORD dwResered[4];
	tagQKSignal()
	{
		ZeroMemory(this, sizeof(tagQKSignal));
	}
}GWQKSignal;
 

DECLARE_VECTOR(GWQKSignal, GWQKSIGNAL)
class GWQKSignalPack : public GWBasePack	// Ǭ�����ź�
{ 
public:
	int nPeriod;		//����
	int nSignalKind;	//�źŷ���
	GWQKSIGNALVEC	vecQKSignal;//�źż���
	GWQKSIGNALVEC	vecQKSignalLatest;	//���µ������ź�
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


struct GWBYJZItem  //���ľ���
{
	SymbolObj * pSymbol;
	int nPeriod;		//����
	int nSignalType;
	DWORD dwID;
	time_t dwUpdateTime; //ʱ��
	DWORD dwTradeDay;	//������ tradeday
	int  nBS;			//�����ź�
	double dfBound1;//�߽�1
	double dfBound2; //�߽�2
	double dfReserved[2]; 
	int dwKIndex;	//��ӦK�ߵ�����
	DWORD dwReserved[2];
	GWBYJZItem()
	{
		ZeroMemory(this, sizeof(GWBYJZItem));
		dwKIndex = -1;
	}
};




struct GWBYJZYZSItem  //����ֹӯֹ��
{
	GWBYJZItem tBYJZData;
	double dfZY; //ֹӯ
	double dfZS;//ֹ��
	double dfBSZS; //����ֹ��
	double dfJJZS;//����ֹ�� 
	GWBYJZYZSItem()
	{
		ZeroMemory(this, sizeof(GWBYJZYZSItem));
	}
};



DECLARE_VECTOR(GWBYJZItem, GWBYJW)
DECLARE_VECTOR(GWBYJZYZSItem, GWGWBYJZYZS) 
class GWBYJZPack : public GWBasePack	// ���ľ���
{ 
public:
	int nPeriod;		//����
	int nSignalKind;	//�źŷ���
	GWBYJWVEC	vecBYJZ;//���ݼ��� 
	GWBYJZYZSItem tBYZSZYData;//ֹ��ֹӯ
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