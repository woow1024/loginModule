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
	// д����־
#ifdef _DEBUG
	GW_FUNCLIB_API GWVOID WriteDebugMessageImp(LPCTSTR lpFile, int nLine, GWLogLevel nLevel, LPCTSTR lpMessage, ...);
#else
	GW_FUNCLIB_API GWVOID WriteDebugMessageImp(GWLogLevel nLevel, LPCTSTR lpMessage, ...);
#endif
	// ��ȡ��ǰ��־����
	GW_FUNCLIB_API const CString& GetLastLogFileName();
}

namespace GW_Monitor
{
	enum MonitorRectType { EntireMonitor = 0, WorkAreaOnly };

	// ��ȡ��ǰ����������ʾ��Rect
	GW_FUNCLIB_API GWBOOL GetCurrentMonitorRect(CWnd* pWnd, CRect& rcMonitor, MonitorRectType eType = WorkAreaOnly);
}

namespace GW_PathFile
{
	// Ŀ¼���ļ��Ƿ����
	GW_FUNCLIB_API GWBOOL IsPathFileExist(const CString& strPathFile);					
	
	// �ж�·���Ƿ����·��
	GW_FUNCLIB_API GWBOOL IsRelativePath(const CString& strPath);	

	// �ж�·���Ƿ�Ϊ����·��
	GW_FUNCLIB_API GWBOOL IsNetPath(const CString& strPath);								

	// ��ȡ�ļ���·����Ŀ¼
	GW_FUNCLIB_API const CString GetParentPath(const CString& strFileName);					


	GW_FUNCLIB_API  CString FullPath2ShortPath(const CString& strFullPahtFileName);	
	// ����Ŀ¼
	GW_FUNCLIB_API GWBOOL CreateDir(const CString& strPath);

	// ȷ��Ŀ¼����
	GW_FUNCLIB_API GWBOOL EnsureDirExist(const CString& strPath);	

	// ���ļ�����
	GW_FUNCLIB_API GWUINT FindFiles(const CString& strDir, CSTRINGVEC& vecFile, const CString& strExtName = _T("*.*"));

	// Ŀ¼ɾ��
	GW_FUNCLIB_API GWBOOL DeleteDir(const CString&  strDir);

	// �ļ�ɾ��
	GW_FUNCLIB_API GWBOOL DeleteFile(const CString&  strFile);

	// ������xml�ļ�
	GW_FUNCLIB_API GWBOOL CreateEmptyXMLFile(const CString& strFileName);
}

namespace GW_Version
{
	// �Ƿ��ο�
	GW_FUNCLIB_API GWBOOL IsVisitor();

	// �Ƿ�ע���û�
	GW_FUNCLIB_API GWBOOL IsRegisteredUser();

	// �Ƿ񼤻�
	GW_FUNCLIB_API GWBOOL IsActiveUser();

	// �Ƿ�VIP
	GW_FUNCLIB_API GWBOOL IsVip();

	// �Ƿ��ڲ�Ա��
	GW_FUNCLIB_API GWBOOL IsInnerStaff();
}

#define GetUIColor(id, default_color)		GW_UI::GetUIColorImpl(id,  default_color)
namespace GW_UI
{
	// ��ȡָ����С��������
	GW_FUNCLIB_API CFont* GetChineseFont(int nSize, BOOL bBold = FALSE);

	// ��ȡָ����С��������
	GW_FUNCLIB_API CFont* GetNumberFont(int nSize, BOOL bBold = FALSE);

	// ��ȡָ����СӢ������
	GW_FUNCLIB_API CFont* GetEnglishFont(int nSize, BOOL bBold = FALSE);

	// ��ȡ�۸����ɫ
	GW_FUNCLIB_API COLORREF GetPriceColor(double dbPreClose, double dbPirce, int nDigit = 3);

	// ��ȡ�ָ���pen
	GW_FUNCLIB_API CPen* GetSplitLine(int nWidth, COLORREF clrLine= RGB(99, 98, 98), int nStyle = PS_SOLID);

	// ����仭��,Ϊ�����Ϻ������º�����ֱ�ӵ���GDILib��
	GW_FUNCLIB_API void LinePoint(CDC* pDC, CPoint pt1, CPoint pt2, CPen* pPen);

	// ����UIģʽ
	GW_FUNCLIB_API void SetUIMode(const CString& strMode);
	
	GW_FUNCLIB_API COLORREF GetUIColorImpl(UINT uKeyID, COLORREF clrDefault = RGB(0, 0, 0));
}

namespace GW_Common
{
	// ��ȡָ��������㸸����
	GW_FUNCLIB_API HWND GetTopLevelParent(HWND hWnd);

	// ���õ�ǰ�FrameWnd
	GW_FUNCLIB_API GWVOID SetActiveFrameWnd(CWnd* pFrameWnd);

	// ��ȡ��ǰ�FrameWnd
	GW_FUNCLIB_API CWnd*	GetActiveFrameWnd();

	// ��ȡ��ǰ�Symbol
	GW_FUNCLIB_API SymbolObj* GetActiveSymbolObj();

	// �������ƴ�����ͼ
	GW_FUNCLIB_API CView*	CreateViewByName(const CString& strViewName, CWnd * pParent, BOOL bInit, SymbolObj * pSymbolObj = NULL, const CString& strParam = _T(""));
	
	// ����һ��GUID,FullMode��ʾ�Ƿ���Ҫ�����������
	GW_FUNCLIB_API const CString CreateGUID(BOOL bFullMode = TRUE);

	// ���ַ����������
	GW_FUNCLIB_API GWBOOL PutStringToClipboard(const CString& strText);

	// ʹ��ϵͳĬ��������������ļ��е�url
	GW_FUNCLIB_API GWBOOL QuickOpenUrl(LPCTSTR lpApp, LPCTSTR lpSection);

	// ����double�Ƿ����
	GW_FUNCLIB_API GWBOOL IsDoubleEqual(double dfVal1, double dfVal2);

	// ���ýű��ļ�
	GW_FUNCLIB_API GWBOOL CallScriptFunction(const char*filename, const char* func = NULL);
	
	GW_FUNCLIB_API void BroadcastMessage(HWND hParent, UINT msg, WPARAM wp, LPARAM lp, BOOL bPostMessage = TRUE);

	enum EMViewClassType
	{
		em_View_FST = 0,	//��ʱͼ
		em_View_KXT,	//K��ͼ
		em_View_ZBT,	//���ͼ
		em_View_F10,	//F10
		em_View_DZQ,	//������ͬ��
		em_View_FJB,	//�ּ۱�
		em_View_HQ,	//������ҳ
		em_View_DPZ,	//��Ʒ��ͬ��
		em_View_SY,	//��ҳ
		em_View_LOGIN,	//��¼
		em_View_TOOLBAR,	//������
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
	
	// ���ַ����򵥽���MD5����
	GW_FUNCLIB_API CString SimpleMD5Key(const CString& strText);

	//BASE64 ����
	GW_FUNCLIB_API CString BASE64Encrypt(const CString &strCleartext,long lLen);

	//BASE64 ����
	GW_FUNCLIB_API CString BASE64Decode(const CString &strCiphertext,long lLen);
}

namespace Json
{
	class Value;
}

namespace GW_Json
{
	class Value;
	
	// ��ӡJson�ַ���
	GW_FUNCLIB_API GWVOID PrintJsonString(Json::Value& jsValue);
	
	// ��Json�ṹ��ȡ�ַ��� 
	GW_FUNCLIB_API CString StringFromJsonValue(Json::Value& jsValue);

}

namespace GW_Time
{
	enum TIME_FORMAT
	{
		TIME_YMD = 0,		// ���� �����գ�20121214��
		TIME_MD,			// ���� ���� ��1214��
		TIME_HMS,			// ʱ�� ʱ���루131415��
		TIME_HM,			// ʱ�� ʱ�֣�1314��
		TIME_FULL			// ������ʱ���루20121214131415��
	};

	// ����time_t����ʱ��
	GW_FUNCLIB_API GWLONG TimeFromTimeT(time_t t, TIME_FORMAT fmt);

	// ������������time_t
	GW_FUNCLIB_API time_t TimeTFromDate(long nDate);

	// ����time_t�����ַ�ʱ�䴮
	GW_FUNCLIB_API CString StringFromTimeT(time_t t, TIME_FORMAT fmt, const CString& strDateSp = _T("-"), const CString& strTimeSp = _T(":"), const CString& strDateTimeSp = _T(" "));

	// ���ݽ����������ַ�ʱ�䴮
	GW_FUNCLIB_API CString StringFromDate(long nDate, TIME_FORMAT fmt, const CString& strDateSp = _T("-"), const CString& strTimeSp = _T(":"), const CString& strDateTimeSp = _T(" "));
}

namespace GW_HQ
{
	// ����һ���г�����
	GW_FUNCLIB_API GWVOID AddMarketToMarketMap(MarketInfo* pMarket);
	
	// ����һ���г�����
	GW_FUNCLIB_API MarketInfo* FindMarket(const CString& strMarket);
	
	// ������������
	GW_FUNCLIB_API GWVOID AddSymbolToCodeList(SymbolObj* pSymbolObj);
	
	// ��ȡ�����г�
	GW_FUNCLIB_API GWVOID GetAllMarketNames(CSTRINGVEC& vecName);
	
	// ��ȡ�����г�
	GW_FUNCLIB_API GWVOID GetAllMarket(MRKETVEC& vecMarket);	

	// ��ȡָ���г������д���
	GW_FUNCLIB_API GWVOID GetMarketSymbols(const CString& strMarket, SYMBOLVEC& vecSymbol, BOOL bOnlyTopLevelSymbol = FALSE);		
	
	// ��ȡ�����г������д���
	GW_FUNCLIB_API GWVOID GetAllMarketSymbols(SYMBOLVEC& vecAllSymbol);		
	
	// ��ȡsymbol�����ӽڵ�
	GW_FUNCLIB_API GWVOID GetMarketSymbols(SymbolObj& pParentSymbol, SYMBOLVEC& vecChildrenSymbol);

	// ȫ�г����Ҵ���
	GW_FUNCLIB_API SymbolObj* FindSymbolFromAllMarket(const CString &strCode);	

	// ���Ҵ���
	GW_FUNCLIB_API SymbolObj* FindSymbolObj(const CString& strMarket, const CString &strCode);	

	// ���Ҵ���
	GW_FUNCLIB_API SymbolObj* FindSymbolObjEx(const CString& strObj);

	// ��ȡ��һֻ��Ʊ
	GW_FUNCLIB_API SymbolObj*	GetNextSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket);				

	// ��ȡ��һֻ��Ʊ
	GW_FUNCLIB_API SymbolObj*	GetPreviousSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket);			

	// ���õ�ǰѭ����symbols
	GW_FUNCLIB_API void SetActiveSymbols(SYMBOLVEC& vecSymbols);

	// ��ȡѡ���г��Ĺ�Ʊѭ������
	GW_FUNCLIB_API GWVOID GetMarketSymbolLoopVector(SYMBOLVEC& vecMarketSymbol, const CString& strMarket, SymbolObj* pStartSymbolObj = NULL, UINT nMaxCount = 16, BOOL bLoop = TRUE);
	GW_FUNCLIB_API GWBOOL IsVolMarket(SymbolObj* pSymbolObj);
	GW_FUNCLIB_API GWBOOL IsVolMarket(MarketInfo* pMarket);

	GW_FUNCLIB_API int PeriodMin2KType(int nPeriodMin);
	GW_FUNCLIB_API int KType2PeriodMin(int nKType);
	GW_FUNCLIB_API CString GetTJXName(int nKType);		//̫��
	GW_FUNCLIB_API CString GetQKXName(int nKType, int nQKX = tagQKSignal::em_QK_Line);		//Ǭ��
	GW_FUNCLIB_API CString GetTJXNameEx(int nPeriodMin);		//̫��
	GW_FUNCLIB_API CString GetQKXNameEx(int nPeriodMin, int nQKX);		//Ǭ��
	GW_FUNCLIB_API CString GetBYJZName(int nPeriodMin);//���ľ���
}

namespace GW_Cfg
{
	// �������ļ�Indicator.xml��ȡ���м�������ָ��
	GW_FUNCLIB_API GWVOID ReadIndicatorXML(CIndicatorArr & CTAArr, const CString & strKey);

	// �������ļ�urlconfig.ini�ж�ȡ��ҳ��Ϣ
	GW_FUNCLIB_API CString GetUrlFromCfg(LPCTSTR lpApp, LPCTSTR lpSection);

	// ��ȡINI�����ļ�
	GW_FUNCLIB_API CString GetRegKeyString(const CString& strFile, const CString& strSection, const CString& strKey, const CString& strDefault = _T(""));
	GW_FUNCLIB_API int GetRegKeyInt(const CString& strFile, const CString& strSection, const CString& strKey, int lValDefalt = 0);

	// д��INI�����ļ�
	GW_FUNCLIB_API GWBOOL WriteRegKeyString( const CString& strFile,  const CString& strSection,  const CString& strKey, const CString& strVal);
	GW_FUNCLIB_API GWBOOL WriteRegKeyInt( const CString& strFile,  const CString& strSection,  const CString& strKey, int lVal);
}

namespace GW_InfoBomb
{
	GW_FUNCLIB_API 	void ExtractDelInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecDel);//��ȡ��Ҫɾ������Ϣ����
	GW_FUNCLIB_API 	void RemoveUnusedInfoBomb(IN OUT GWINFOBOMVEC& vecOrg, IN const GWINFOBOMVEC & vecDel);	//ɾ�����ڵ���Ϣ����

	GW_FUNCLIB_API void SortAndUniqueInfoBombArr(IN OUT GWINFOBOMVEC& vecOrg, BOOL bKMode = FALSE);					//������Ψһ��

	GW_FUNCLIB_API 	void ExtractUpdateInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecUpdate);//��ǰ���µ���Ϣ����
	GW_FUNCLIB_API 	void MergeUpdateInfoBomb(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecUpdate);	//�ϲ����µ���Ϣ���ף��������µ�һ��
	GW_FUNCLIB_API  void RemovInfoBombByType(IN OUT GWINFOBOMVEC& vecOrg, IN int nType);	//ɾ��ָ�����͵���Ϣ����

	GW_FUNCLIB_API  void CheckUpdateInfoBombArr(IN OUT GWINFOBOMVEC& vecOrg);//�����Ϣ���׶��У�ǿ�Ƹ��±�־
	
	GW_FUNCLIB_API 	void MergeTwoInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecNew, BOOL bNeedSortAndUnique, BOOL bKMode = FALSE);//�ϲ�2����Ϣ���׶���
	GW_FUNCLIB_API 	void TrimeInfoBombArr(IN GWINFOBOMVEC& vecOrg,  BOOL bNeedSortAndUnique, BOOL bKMode = FALSE);

	GW_FUNCLIB_API CString CreateInfoBomLink(const SymbolObj * pSymbol, const GWInfoBom & tGWInfoBomb, const CString& strFixLink);		// ������Ϣ��������
}

namespace GW_Multimedia
{
	GW_FUNCLIB_API BOOL  PlaySoundEx(LPCTSTR pszSound,BOOL bBeep = TRUE);
	
}
namespace GW_BigNews
{
	GW_FUNCLIB_API 	void ExtractDelNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecDel);//��ȡ��Ҫɾ����������Ѷ
	GW_FUNCLIB_API 	void RemoveUnusedNews(IN OUT GWBIGNEWSVEC& vecOrg, IN const GWBIGNEWSVEC & vecDel);	//ɾ�����ڵ�������Ѷ

	GW_FUNCLIB_API void SortAndUniqueNewsArr(IN OUT GWBIGNEWSVEC& vecOrg);					//������Ψһ��

	GW_FUNCLIB_API 	void ExtractUpdateNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecUpdate);//��ǰ���µ�������Ѷ
	GW_FUNCLIB_API 	void MergeUpdateNews(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecUpdate);	//�ϲ����µ���Ϣ����������Ѷ���������µ�һ��
	GW_FUNCLIB_API  void RemovNewsByType(IN OUT GWBIGNEWSVEC& vecOrg, IN int nType);	//ɾ��ָ�����͵�������Ѷ
	
	GW_FUNCLIB_API 	void MergeTwoNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecNew, BOOL bNeedSortAndUnique);//�ϲ�2����Ϣ���׶���
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

	static void UTF_8ToGB2312(CString &pOut, char *pText, int pLen);//utf_8תΪgb2312  
	static void GB2312ToUTF_8(CString& pOut,char *pText, int pLen); //gb2312 תutf_8  
	static CString UrlGB2312(char * str);                           //urlgb2312����  
	static CString UrlUTF8(char * str);                             //urlutf8 ����  
	static CString UrlUTF8Decode(const CString& strURL);                  //urlutf8����  
	static CString UrlGB2312Decode(const CString& strURL);                //urlgb2312����  

private:  
	static void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);  
	static void UTF_8ToUnicode(WCHAR* pOut,char *pText);  
	static void UnicodeToUTF_8(char* pOut,WCHAR* pText);  
	static void UnicodeToGB2312(char* pOut,WCHAR uData);  
	static char  CharToInt(char ch);  
	static char StrToBin(char *str);  

};  