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
	GDT_CFG	     = 0,		// �����ļ�Ŀ¼
	GDT_USERDATA = 1,		// �û��ļ�
	GDT_UPDATE   = 2,		// �����ļ�
	GDT_DATA	 = 3,		// ��������
	GDT_RES		 = 4,		// ��ԴĿ¼
	GDT_LOG		 = 5,		// ϵͳ��־
	GDT_BAKUP	 = 6,		// ����Ŀ¼
	GDT_PLUGIN	 = 7,		// ���Ŀ¼
	GDT_TACFG	 = 8,		// ��������ʹ������Ŀ¼
	GDT_TMP		 = 9,		// ��ʱĿ¼
	GDT_SCRIPT	 = 10,		// �ű��ļ�Ŀ¼
} GF_DIR_TYPE;

DECLARE_MAP(CString, CString, CSTRING)


enum EM_NewType
{
//	em_Scroll_RSS = 461,	//������Ѷ
	em_News_Common = 527,	//��������
	em_News_TT = 528,	//TT����
	em_News_TD = 529,	//TD����
	em_News_YG = 530,	//YG����
	em_News_ZY = 9999, //ZY����
	em_FastNews = 988,	//��Ѷ
	em_StatusBar_DX = 9998, //����汾�ʻ�
	em_StatusBar_Common = 461, //������Ѷ

	em_Today_Headline = 585,	//���ձض�
	em_Central_Bank_Dynamic = 586, //���ж�̬
	em_Goverment_Voice = 587, //��Ҫ����

	em_News_Other_Left = 757,			// ��ҵ�������������Ѷ
	em_News_Other_Right = 758,			// ��ҵ�����Ҳ�������Ѷ
	em_News_Adv = 10000,	//�������
	em_Baijia_News = 10001,	//�ټ�����
	em_BaiJia_Adv_TT = 768,	//��ҳ���   TT
	em_BaiJia_Adv_SY = 809,	//��ҳ���   SY
	em_News_Oill_Headline = 806,	//ԭ��Ҫ��
	em_News_Oill_Dynamic = 807,  //���ж�̬
	em_News_Oill_Market_Dynamic = 808, //��������̬
	//��߰�����
	em_News_TD_Left=527,		//797,
	em_News_TD_Right=529,		//798,

	em_Baijia_YG = 9997,	//�ټ�����
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
	const CString GetMainExeName();								// ��ȡִ��exe����

	const CString GetMainDir();									// ��ȡ��������Ŀ¼
	const CString GetGFSubDir(GF_DIR_TYPE type);				// ��ȡָ�����͵�Ŀ¼

	const CString GetDealFileName(SymbolObj* pSymbol, long lDate);			// ��ȡ�������ļ�����
	const CString GetMinuteFileName(SymbolObj* pSymbol, long lDate);		// ��ȡ����ͼ����ļ�����
	const CString GetKLineFileName(SymbolObj* pSymbol, GWKLINE_TYPE nType);	// ��ȡK������ļ�����

	const CString GetMinMineFileName( SymbolObj* pSymbol, long lDate);						// ������Ϣ��������ļ�����
	const CString GetHisMineFileName(SymbolObj* pSymbol);									// ��ʷ��Ϣ��������ļ�����
 
	const CString GetMinServerTAFileName( SymbolObj* pSymbol, long lDate);						// ������ָ���ʱ����ļ�����
	const CString GetHisServerTAFileName(SymbolObj* pSymbol);									// ��ʷ������ָ������ļ�����
	const CString GetNewsFileName(SymbolObj* pSymbol, long lDate, int nNewsType);				//������ ��������ļ�����
	const CString GeSLSPFileName(SymbolObj* pSymbol, long lDate, int nPeriod);				//������ SLSP �ļ�����
	const CString GeQKSignalFileName(SymbolObj* pSymbol, int nPeriodType, int nSignalKind);				//������ ��������ļ�����
	const CString GeBYJZFileName(SymbolObj* pSymbol, int nPeriodType, int nSignalKind);	
	const CString GetSystemConfigFileName();					// ��ȡϵͳ�����ļ�����
	const CString GetUserConfigFileName();						// ��ȡ�û������ļ�����
	const CString GetResSubDir(CString& strSubDir);	//��ԴĿ¼�µ���Ŀ¼

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
	//�������ݰ汾��
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

	void LoadLocalConfig();										// ���ر�������
	
	void SaveSettings();
	void ReadSettings();
	
	void ReadVerFile();		//��ȡ�ļ��汾��Ϣ
	BOOL ReadSkinFile();	//��ȡ��ɫ����
	void FreeAllSkin();		//�ͷ�����Ƥ���ڴ�
public:
	void SaveVerFile();		//��ȡ�ļ��汾��Ϣ
private:
	struct tagSkinCell
	{
		DWORD dwID;		//ID
		COLORREF clColor;	//������ɫ
		BOOL bInited;		//�Ƿ��ʼ��
		tagSkinCell()
		{
			ZeroMemory(this, sizeof(tagSkinCell));
		}
	};
	struct tagSkinSerial	
	{
		DWORD dwID;		//ID
		TCHAR szSerialName[32];	//Ƥ������
		TCHAR szResPath[64];	//��ԴͼƬ·��
		int nMaxCellNum;	//�����ɫ��
		int nRealCellNum;	//ʵ����ɫ��
		tagSkinCell * pSkinCell;	//ϸ����ɫ
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
	SKINSERIALVEC m_cSkinVec;		//Ƥ������
	int m_nCurrentSkinIndex;	//��ǰƤ������

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