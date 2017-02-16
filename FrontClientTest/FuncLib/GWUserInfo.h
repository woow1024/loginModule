#pragma once

#include <toolclass.h>
#include <gwtypes.h>

#undef GW_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define GW_EXT_CLASS _declspec(dllexport)
#else
#define GW_EXT_CLASS _declspec(dllimport)
#endif

#define VBS_STATE_TOP						0x00000010     //百家地雷直播室老师
#define VBS_STATE_ATTENTION				    0x00000100	   //百家地雷关注
#define VBS_STATE_NOTATTENTION				0x00001000	   //百家地雷未关注

enum GWACCOUNTLEVEL
{
	VISITOR				= 0,	// 游客
	REGISTEREDUSER		= 100,	// 注册用户

	ACTIVEUSER			= 300,	// 激活用户
	//0（默认）,1(低端),2（高级）

	VIPUSER				= 500,	// VIP

	//viplevel 0(默认),1,2,3(暂无明确含义)

};

enum GWACCOUNTTYPE
{
	ATYPE_GW	= 0,		// 默认类型
	ATYPE_TT	= 1,		// 融金汇银
	ATYPE_TD	= 2,		// 金策黄金
	ATYPE_YG	= 3,		// 金祥银瑞
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
	CString     m_strKey;			// 服务器Key
	CString		m_strUserName;		//用户登陆名
	CString		m_strPasswd;
	CString		m_strNickName;	//昵称/别名
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
		ADAPTERVEC adapters;				// 适配器信息

		struct
		{
			USHORT cx;						// 分辨率 x
			USHORT cy;						// 分辨率 y
		} resolution;						// 分辨率

		char hostname[32];					// 机器名
		char brower[32];					// 默认浏览器

		SYSTEM_INFO	sysinfo;				// 系统信息
		OSVERSIONINFOEX  osversion;			// 操作系统信息

	} LocalHardwareInfo;
public:
	~GWUserInfo(void);

	void CollectLocalHardwareInfo();

	CString GetLocalMachineID();

	CString GetExtInfoString();
	BOOL IsLocalMacAddr(const CString& strMac);

	BOOL IsVisitor() const	// 是否游客
	{ 
		return GetAccountLevel() == VISITOR; 
	}	
	DWORD GetTAPrivilege();
	BOOL IsRegisteredUser() const // 是否注册客户
	{
		return IsOnline() && (m_nAccountLevel >= REGISTEREDUSER && m_nAccountLevel < ACTIVEUSER); 
	}	

	BOOL IsActiveUser() const // 是否激活用户
	{
		return IsOnline() && (m_nAccountLevel  >= ACTIVEUSER && m_nAccountLevel < VIPUSER) ;
	}

	BOOL IsVIP() const	// 是否VIP
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
	void AddOneAttionExpert(GWExpertInfo & tExpert);//增加一个关注列表
	GWExpertInfo * GetExpertByID(DWORD dwID, DWORD dwType = -1);
	BOOL IsExpertExist(DWORD dwID, DWORD dwType = -1);
	BOOL IsAttention();
	void ClearExpertList();
	enum EMLineState
	{
		em_Line_Off = 0,	//离线
		em_Line_On,	//在线
		em_line_Busy,	//忙碌
		em_Line_Leave,//离开
	};

	BOOL IsHasIM();
	void SetIMState(int nIMState);
	int  GetIMState();
	BOOL IsNewsShow(tagInfoBombItem  tBombItem);
	GWSYMBOLRIGHTVEC & GetSymoblRightVec();

	void AddAccredit(CString strKey,int bValue);
	int GetAccredit(CString strKey,int nDefault = 0);
	BOOL HasAccredit(CString strKey);
	CString GetSLSPID(); //SLSP 席位号
	CString GetClientType();
private:
	BOOL HasTJAccredit();
	void CollectAdapterInfo();

	void InitInstance();

private:
	LocalHardwareInfo	m_cbHDInfo;

	GWACCOUNTLEVEL		m_nAccountLevel;			// 账户等级
	GWACCOUNTTYPE		m_nAccountType;				// 账户所属市场

	BOOL				m_bOnLineState;				// 是否在线
	CString				m_strUID;					// 服务端生成的用户标识
	DWORD				m_dwTAPrivilege;			//指标权限
	DWORD				m_dwSLSPPrivilege;			//头寸权限
	CString				m_strDefaultVerID;
	int					m_nIMMessageCount;			//IM消息通知
	int					m_nIMState;			//IM在线状态  0：不在线 1 在线 2 忙碌 
	GWServerIno m_svrArray[MAX_SERVER + 1];

	BOOL				m_bInnerStaff;
	CString				m_strSLSP;		//SLSP 席位号
	GWEXPERTINFOVEC  m_vectorExpert;		//关注列表
	GWSYMBOLRIGHTVEC  m_vecSymbolRight;	//品种权限
	GWTAIJISIGNALVEC  m_vecTaijiSignal;	//太极信号触发列表
	GWQKSIGNALVEC m_vecQKSignal;		//乾坤线数据
	CString m_strClientType;				//用户类型
#ifdef VERSION_TAG_TT
	int					m_nTmpLevel;				// TT版临时协议
public:
	int GetTmpLevel() const { return m_nTmpLevel; }
#endif

	CSTRINGINTMAP m_mapAccredit; // 模块使用权限
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