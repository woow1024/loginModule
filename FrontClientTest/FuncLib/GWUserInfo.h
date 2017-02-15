#pragma once

#include <toolclass.h>
#include <gwtypes.h>

#undef GW_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define GW_EXT_CLASS _declspec(dllexport)
#else
#define GW_EXT_CLASS _declspec(dllimport)
#endif

#define VBS_STATE_TOP						0x00000010     //�ټҵ���ֱ������ʦ
#define VBS_STATE_ATTENTION				    0x00000100	   //�ټҵ��׹�ע
#define VBS_STATE_NOTATTENTION				0x00001000	   //�ټҵ���δ��ע

enum GWACCOUNTLEVEL
{
	VISITOR				= 0,	// �ο�
	REGISTEREDUSER		= 100,	// ע���û�

	ACTIVEUSER			= 300,	// �����û�
	//0��Ĭ�ϣ�,1(�Ͷ�),2���߼���

	VIPUSER				= 500,	// VIP

	//viplevel 0(Ĭ��),1,2,3(������ȷ����)

};

enum GWACCOUNTTYPE
{
	ATYPE_GW	= 0,		// Ĭ������
	ATYPE_TT	= 1,		// �ڽ����
	ATYPE_TD	= 2,		// ��߻ƽ�
	ATYPE_YG	= 3,		// ��������
};

enum GWServerType { Login_Server = 0, Trade_Server , Update_Server, Upload_Server ,HQ_Server,  HQ_Server1, HQ_Server2, MAX_SERVER=31 };

class GW_EXT_CLASS GWServerIno
{
public:
	GWServerIno() {}
	GWServerIno(LPCTSTR lpServerIP, USHORT uPort, LPCTSTR lpKey = NULL)
	{
		SetServerInfo(lpServerIP, uPort, lpKey);
	}

	void SetServerInfo(LPCTSTR lpServerIP, USHORT uPort, LPCTSTR lpKey = NULL)
	{
		m_strIP		= lpServerIP;
		m_uPort		= uPort;
		m_strKey	= lpKey;
	}

	void SetServerInfoEx(const CString& strUserName, const CString& strPasswd)
	{
		m_strUserName   = strUserName;
		m_strPasswd		= strPasswd; 
	}
	void SetNickName(const CString& strNickName )
	{
		m_strNickName = strNickName;
	}
	const CString& GetServerIP() const { return m_strIP; }
	const CString& GetUserName() const { return m_strUserName; }
	const CString& GetNickName() const { return m_strNickName; }
	const CString& GetPasswd() const { return m_strPasswd; }
	const CString& GetKey() const { return m_strKey; }
	USHORT GetPort() const { return m_uPort; }

private:
	CString		m_strIP;
	CString     m_strKey;			// ������Key
	CString		m_strUserName;		//�û���½��
	CString		m_strPasswd;
	CString		m_strNickName;	//�ǳ�/����
	USHORT		m_uPort;
};

DECLARE_MAP(CString,int,CSTRINGINT)
class GW_EXT_CLASS GWUserInfo : public static_singletonbase<GWUserInfo>
{
	IMPLEMENT_STATIC_SINGLETON_MODE(GWUserInfo)

		typedef struct LocalAdapterInfo
	{
		CString mac;	
		CString ip; 	

		bool operator < (const LocalAdapterInfo& adapter)
		{
			return mac < adapter.mac;
		}
	} LocalAdapterInfo;

#define  ACCOUNT_LEVEL_UNIT		100
	DECLARE_VECTOR(LocalAdapterInfo, ADAPTER)
	typedef struct
	{
		ADAPTERVEC adapters;				// ��������Ϣ

		struct
		{
			USHORT cx;						// �ֱ��� x
			USHORT cy;						// �ֱ��� y
		} resolution;						// �ֱ���

		char hostname[32];					// ������
		char brower[32];					// Ĭ�������

		SYSTEM_INFO	sysinfo;				// ϵͳ��Ϣ
		OSVERSIONINFOEX  osversion;			// ����ϵͳ��Ϣ

	} LocalHardwareInfo;
public:
	~GWUserInfo(void);

	void CollectLocalHardwareInfo();

	CString GetLocalMachineID();

	CString GetExtInfoString();
	BOOL IsLocalMacAddr(const CString& strMac);

	BOOL IsVisitor() const	// �Ƿ��ο�
	{ 
		return GetAccountLevel() == VISITOR; 
	}	
	DWORD GetTAPrivilege();
	BOOL IsRegisteredUser() const // �Ƿ�ע��ͻ�
	{
		return IsOnline() && (m_nAccountLevel >= REGISTEREDUSER && m_nAccountLevel < ACTIVEUSER); 
	}	

	BOOL IsActiveUser() const // �Ƿ񼤻��û�
	{
		return IsOnline() && (m_nAccountLevel  >= ACTIVEUSER && m_nAccountLevel < VIPUSER) ;
	}

	BOOL IsVIP() const	// �Ƿ�VIP
	{ 
		return IsOnline() && GetAccountLevel() >= VIPUSER; 
	}	

	BOOL IsInnerStaff() const
	{
		return IsOnline() && m_bInnerStaff;
	}

	BOOL IsLocalUser() const;

	GWACCOUNTLEVEL GetAccountLevel() const 
	{
		return m_nAccountLevel; 
	}
	DWORD GetMainAccountLevel() const
	{
		int dwMainLevel = (m_nAccountLevel / ACCOUNT_LEVEL_UNIT) * ACCOUNT_LEVEL_UNIT;
		return dwMainLevel;
	}
	DWORD GetSubAccountLevel() const 
	{	
		int dwSubLevel = m_nAccountLevel%ACCOUNT_LEVEL_UNIT;
		return dwSubLevel;
	}
	int AccountLevel2CurMode(BOOL bConnect)
	{
		int m_nCurMode;
		if(!bConnect)
		{
			m_nAccountLevel = VISITOR;
			m_nCurMode = 0;
		}
		else if(IsVIP())
		{
			m_nCurMode = GetMainAccountLevel();
		}
		else if(IsActiveUser())
		{
			m_nCurMode = GetAccountLevel();
			if((ACTIVEUSER + 1) == m_nCurMode)
				m_nCurMode = ACTIVEUSER;
		}
		else if(IsRegisteredUser())
		{
			m_nCurMode = GetMainAccountLevel();
		}
		else
		{
			m_nCurMode = 0;
		}
		return m_nCurMode;
	}
	BOOL IsOnline() const { return m_bOnLineState; }
	void SetOnline(BOOL bOnline) { m_bOnLineState = bOnline; }

	const CString& GetUserName() const { return m_svrArray[Login_Server].GetUserName(); }
	const CString& GetUserPWD() const { return m_svrArray[Login_Server].GetPasswd(); }
	const CString& GetNickName() const { return m_svrArray[Login_Server].GetNickName(); }
	const CString& GetUserID()const { return m_strUID; }
	CString& GetDefaultVerID(){return m_strDefaultVerID;}
	GWServerIno& GetServerInfo(GWServerType nType)  
	{
		return m_svrArray[nType];
	}

	const GWServerIno& GetTradeServerInfo() const
	{
		return m_svrArray[Trade_Server];
	}

	BOOL ParseTradeString(const string& strJson);

	void SetServerInfo(GWServerType nType, LPCTSTR lpServerIP, USHORT uPort, LPCTSTR lpKey = NULL);
	void SetServerInfo(GWServerType nType, LPCTSTR lpServerInfo);

	void SetAccountInfo(LPCTSTR lpUserName, LPCTSTR lpPasswd, BOOL bVisitor)
	{
		m_svrArray[Login_Server].SetServerInfoEx(lpUserName, lpPasswd);

		if (bVisitor)
		{
			m_nAccountLevel = VISITOR;
		}
		else
		{
			m_nAccountLevel = REGISTEREDUSER;
		}
	}

	void SetAccountInfoEx(LPCTSTR lpUID, GWACCOUNTTYPE nType);
	void SetIMMessageCount(int nMessageCount);
	int  GetIMMessageCount();
	void AddOneAttionExpert(GWExpertInfo & tExpert);//����һ����ע�б�
	GWExpertInfo * GetExpertByID(DWORD dwID, DWORD dwType = -1);
	BOOL IsExpertExist(DWORD dwID, DWORD dwType = -1);
	BOOL IsAttention();
	void ClearExpertList();
	enum EMLineState
	{
		em_Line_Off = 0,	//����
		em_Line_On,	//����
		em_line_Busy,	//æµ
		em_Line_Leave,//�뿪
	};

	BOOL IsHasIM();
	void SetIMState(int nIMState);
	int  GetIMState();
	BOOL IsNewsShow(tagInfoBombItem  tBombItem);
	GWSYMBOLRIGHTVEC & GetSymoblRightVec();

	void AddAccredit(CString strKey,int bValue);
	int GetAccredit(CString strKey,int nDefault = 0);
	BOOL HasAccredit(CString strKey);
	CString GetSLSPID(); //SLSP ϯλ��
	CString GetClientType();
private:
	BOOL HasTJAccredit();
	void CollectAdapterInfo();

	void InitInstance();

private:
	LocalHardwareInfo	m_cbHDInfo;

	GWACCOUNTLEVEL		m_nAccountLevel;			// �˻��ȼ�
	GWACCOUNTTYPE		m_nAccountType;				// �˻������г�

	BOOL				m_bOnLineState;				// �Ƿ�����
	CString				m_strUID;					// ��������ɵ��û���ʶ
	DWORD				m_dwTAPrivilege;			//ָ��Ȩ��
	DWORD				m_dwSLSPPrivilege;			//ͷ��Ȩ��
	CString				m_strDefaultVerID;
	int					m_nIMMessageCount;			//IM��Ϣ֪ͨ
	int					m_nIMState;			//IM����״̬  0�������� 1 ���� 2 æµ 
	GWServerIno m_svrArray[MAX_SERVER + 1];

	BOOL				m_bInnerStaff;
	CString				m_strSLSP;		//SLSP ϯλ��
	GWEXPERTINFOVEC  m_vectorExpert;		//��ע�б�
	GWSYMBOLRIGHTVEC  m_vecSymbolRight;	//Ʒ��Ȩ��
	GWTAIJISIGNALVEC  m_vecTaijiSignal;	//̫���źŴ����б�
	GWQKSIGNALVEC m_vecQKSignal;		//Ǭ��������
	CString m_strClientType;				//�û�����
#ifdef VERSION_TAG_TT
	int					m_nTmpLevel;				// TT����ʱЭ��
public:
	int GetTmpLevel() const { return m_nTmpLevel; }
#endif

	CSTRINGINTMAP m_mapAccredit; // ģ��ʹ��Ȩ��
public:
	GWTaiJiSignal * GetTaijiSignal(SymbolObj * pSymbol, int nPeriodType = 60); // nPeriodType  60  180  240 
	GWTAIJISIGNALVEC GetTaijiSignals(SymbolObj * pSymbol); 
	BOOL AddTaijiSignal(GWTaiJiSignal & tTaijiSignal);
	GWQKSignal* GetQKSignal(SymbolObj* pSymbol, int nPeriod, int nSignalKind);
	void AddQKSignal(GWQKSignal & tQKSignal);
	void AddQKSignal(GWQKSIGNALVEC & vecQKSignal);
	int  GetQKState(SymbolObj* pSymbol, int nPeriod, int nSignalKind,  time_t dwActiveTime = 0); 
};

GW_EXT_CLASS GWUserInfo* GetUserInfo();
GW_EXT_CLASS CString GetClientType();