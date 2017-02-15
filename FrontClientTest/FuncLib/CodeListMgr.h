#pragma once

#include <gwtypes.h>
#include <toolclass.h>

#undef _MX_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define _MX_EXT_CLASS _declspec(dllexport)
#else
#define _MX_EXT_CLASS _declspec(dllimport)
#endif

class _MX_EXT_CLASS CodeListMgr : public static_singletonbase<CodeListMgr>
{
	IMPLEMENT_STATIC_SINGLETON_MODE(CodeListMgr)
public:
	~CodeListMgr(void);
	
	void SetActiveSymbolObj(SymbolObj* pSymbolObj);
	void SetActiveMarket(MarketInfo* pMarket);
	void SetActiveMarket(const CString& strMarket);

	void SetActiveSymbolsVec(SYMBOLVEC& vecSymbols);

	SymbolObj*	GetActiveSymbolObj();
	MarketInfo* GetActiveMarket();

	void		AddSymbol(SymbolObj* pSymbolObj);												// ��Ӵ���
	void		UpdateSymbol(SymbolObj* pSymbolObj);											// ���´���
	
	void		MakeRelationship();																// �������ӹ�ϵ

	SymbolObj*	FindSymbolObj(const CString& strMarket, const CString &strCode);				// ���Ҵ���
	
	void		AddMarket(MarketInfo* pMarket);													// ����г�
	MarketInfo* FindMarket(const CString& strMarket);											// �����г�
	
	void		DoMarketAdjust();																// ά��һ���г�˳��

	void		GetAllMarket(MRKETVEC& vecName);												// ��ȡ�����г�����
	void		GetAllMarketNames(CSTRINGVEC& vecName);											// ��ȡ�����г�
	void		GetMarketSymbols(const CString& strMarket, SYMBOLVEC& vecSymbol, BOOL bOnlyTopLevelSymbol);				// ��ȡָ���г������д���

	void		KeyworkFilter(const CString& strText, WIZZARDOBJLST& lstMatch, DWORD dwMask);	// ��ȡƥ�����

	BOOL		LoadLocalConfig();																// ���ر��������ļ�

	BOOL		AddSelfStock(SymbolObj* pSymbolObj);											// ������ѡ��
	BOOL        ReplaceSelfStock(SYMBOLVEC vecSymbol);                                          // �滻����ǰ����ѡ��
	BOOL		RemoveSelfStock(SymbolObj* pSymbolObj);											// ɾ����ѡ��
	BOOL		MoveSelfStockPostion(SymbolObj* pSymbolObj, BOOL bMoveDown);					// �ƶ���ѡ��λ�ã����ƻ�����һλ 
	void		SaveSelfStockToFile();															// ������ѡ���ļ��������ļ�

	SymbolObj*	GetNextSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket);				// ��ȡ��һֻ��Ʊ
	SymbolObj*	GetPreviousSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket);			// ��ȡ��һֻ��Ʊ
	
	void MakeSymbolRight(GWSYMBOLRIGHTVEC& vecSymbolRight);		//���� SLSPȨ��Ʒ��
	void GetMarketSymbolLoopVector(SYMBOLVEC& vecMarketSymbol, const CString& strMarket, SymbolObj* pStartSymbolObj, UINT nMaxCount, BOOL bLoop);		 // ��ȡѡ���г��Ĺ�Ʊѭ������
private:
	const CString GetSysShortcutFileName();														// ��ȡϵͳ��ݼ������ļ�����
	const CString GetUserShortcutFileName();													// ��ȡϵͳ��ݼ������ļ�����

	const CString GetSelfStockFileName();														// ��ȡ��ѡ���ļ�����

	SYMBOLVEC&  GetMarketSymbolVec(const CString& strMarketName);								// ��ȡ�г���Ʒ����

	void		CreateSelfStockMarket();														// ������ѡ���г�
		
	BOOL		LoadSelfStockFromFile();														// ���ļ���ѡ��
	BOOL		LoadShortcutFromFile(const CString& strFileName);								// ���ļ�����

	BOOL		CheckMarketValid(MarketInfo* pMarket);											// ����г��Ƿ���Ч

	void		InitInstance();
private:
	MARKETMAP			m_mapMarket;				// �г������

	WIZZARDOBJLST		m_lstShortcutObj;			// ��ݼ������б�
	WIZZARDOBJLST		m_lstSymbolObj;				// �������б�
	MARKETLST			m_lstMarket;				// �г������б�����һ��ԭʼ˳����г�

	SymbolObj*			m_pActiveSymbol;			// ��ǰsymbol
	MarketInfo*			m_pActiveMarket;			// ����Market

	CSTRINGVEC			m_vecOmitMarket;			// ��Ҫ���Ե��г�

	SYMBOLVEC			m_vecActiveSymbols;
};

_MX_EXT_CLASS CodeListMgr* GetCodeListMgr();