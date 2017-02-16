#pragma once

#include <gwtypes.h>
#include <trade\nt_protocol.h>

typedef struct						// ��Լ��Ϣ
{
	SymbolObj* pSymbolObj;			// ��Լ��Ӧ��Ʒ

	int id;							// ��ԼID
	CString symbol;					// ��Լ����
	CString name;					// ��Լ����

	int gpid;						// ��Լ���
	int dic;						// ��Լλ��
	//int aeunit;						// ��Լ��λ
	double aeunit;						// ��Լ��λ
	//double newaeunit;				// ��Լ��λ
	int unitdic;							// ��Լ��λС��λ��
	double minunitnum;				// ��Լ��С��λ����

	CString unitname;				// ��Լ��λ���ĺ���

	GWFloat	spunit;					// ��Լ������λ decimal(16, 2)

#define DIRECTCONTRACT				 '1'		// ֱ�Ӻ�Լ
#define INDIRDIRCONTRACT			 '2'		// ����Լ
#define INTERSECTCONTRACT			 '3'		// �����Լ
#define NOTTRADECONTRACT			 '4'		// �ǽ��׺�Լ
	char type;						// ��Լ����

#define CONTRACT_OPEN				'0'			// ����
#define CONTRACT_CLOSE				'C'			// ������
#define CONTRACT_DISABLE			'D'			// �ѹ��ڵĺ�Լ,����Ҫ��ʾ
	char testatus;								// ����״̬

	int sworder;								// ��ʾ˳��
	
	GWFloat sellvalue;							// �����	decimal(12, 4)
	GWFloat byvalue;							// ����	decimal(12, 4)
	GWFloat slvalue;							// ֹ����	decimal(12, 4)
	GWFloat spvalue;							// ֹӯ���	decimal(12, 4)
	GWFloat ltvalue;							// �޼۵�� decimal(12, 4)

#define PTTYPE_FIXED				'F'			// �̶�
#define PTTYPE_PERSENT				'P'			// ����
	char pttype;

	GWFloat mnvalue;							// ��С�������� decimal(16, 2)
	GWFloat mxvalue;							// ��������� decimal(16, 2)
	GWFloat bymxvalue;							// �����ֲ��� decimal(16, 2)
	GWFloat slmxvalue;							// ������ֲ��� decimal(16, 2)

	int qeceunit;								// ������С�䶯��λ


	GWFloat chargevalue;						// ��������
	GWFloat usedvalue;							// ��֤�����
} ContractDetailInfo;

struct FundInfoBase								// �ʽ���Ϣ
{
	int uid;									// �ͻ�ID
	int fid;									// �ʽ�ID

	GWFloat fmg;								// ���ɱ�֤�� decimal(18, 2)
	GWFloat umg;								// ռ�ñ�֤�� decimal(18, 2)
	GWFloat fzmg;								// ���ᱣ֤�� decimal(18, 2)
	GWFloat atin;								// ����ܼ� decimal(18, 2)
	GWFloat atout;								// �����ܼ� decimal(18, 2)
	GWFloat cha;								// ������ decimal(18, 2)
	GWFloat pl;									// ƽ��ӯ�� decimal(18, 2)
	GWFloat _int;								// ���ڷ� decimal(18, 2)
	GWFloat pvbe;								// ������� decimal(18, 2)
} ;

struct FundInfo : public FundInfoBase			// �ʽ���Ϣ
{
	GWFloat rlbe;								// �ɳ���� decimal(18, 2)

	CString d_rate;								// ��������
	
	CString wrate;								// ��������
	CString dayrank;							// ������
	CString than10days;							// �����ʴ���10����
	CString than10rank;							// �����ʴ���10��������
	CString weekrank;							// ������
	CString windays;							// ����ӯ������
	CString winrank;							// ����ӯ����������

	CString dchange;							// �������仯
	CString wchange;							// �������仯
	CString tenchange;							// �����ʴ���10���������仯
	CString profitchange;						// ����ӯ�����������仯

	CString amount;								// ��ʼ�ʽ�
} ;

struct HisFundInfo : public FundInfoBase		// ��ʷ�ʽ���Ϣ
{
	CString tedate;								// ��������
	GWFloat rnpl;								// ����ӯ�� decimal(18, 2)
};

typedef struct	PostOrderInfo					// �ύ����
{
//	int uid;									// �ͻ�ID
//	int oruid;									// ����ԱID	
	int id;										// ��ԼID	
	int orid;									// ����ID

//	1 : �м�ί��
//	2 : �޼�ί��
	char ortype;								// ί������

//	1 : ��
//	2 : ����ֹ��
//	3 : ����ֹӯ
//	4 : ����ֹ��ֹӯ
//	5 : ƽ
//	6 : ƽ��ֹ��
//	7 : ƽ��ֹӯ
//	8 : ƽ��ֹ��ֹӯ
	char bltype;								// ��������

//	B : ����
//	S : ����
	char bsflag;								// ������־

	int orcrprice;								// ί�е�ǰ�۸�
	int orprice;								// ί�м۸�
	int slprice;								// ֹ���
	int spprice;								// ֹӯ��

	GWFloat qty;								// ί������	decimal(16, 2)

	int rttime;									// ����ί��ʱ��
	int pt;										// ����ɽ����
	
	GWFloat bkhdqty;							// ���ֽ�����	decimal(16, 2)

	PostOrderInfo()
	{
//		oruid = 0;									
		id = 0;									
		orid = 0;									

		ortype = '0';								
		bltype = '0';								
		bsflag = '0';								

		orcrprice = 0;								
		orprice = 0;								
		slprice = 0;								
		spprice = 0;															

		rttime = 0;									
		pt = 0;																
	}
} PostOrderInfo;

struct HoldOrderInfoBase					// �ֲֵ�����
{
	int hdorid;								// �ֲֵ���
	int uid;								// �ͻ�ID
	int fid;								// �ʽ�ID
	int id;									// ��ԼID

	// B : ����
	// S : ����
	char bsflag;							// ������־

	GWFloat qty;							// �ֲ����� decimal(16, 2)

	int opprice;							// ���ּ۸�
	int hdprice;							// �ֲּ۸�
	int slprice;							// ֹ���
	int spprice;							// ֹӯ��

	GWFloat _int;							// ���ڷ� decimal(18, 2)

	CString ontime;							// ����ʱ��

	const int GetKeyID() const
	{
		return hdorid;
	}
};

struct HoldOrderInfoCount : public HoldOrderInfoBase	// ����
{
	int totalhdprice;								// �ܼƳֲּ�
	int totalopprice;								// �ܼƽ��ּ�

	int countid;									// ����ID

	HoldOrderInfoCount(HoldOrderInfoBase* pObj)
	{
		uid = pObj->uid;
		fid = pObj->fid;
		id = pObj->id;

		bsflag = pObj->bsflag;
		qty = pObj->qty;
		opprice = pObj->opprice;
		hdprice = pObj->hdprice;
		_int = pObj->_int;

		totalopprice = (int)(opprice * qty.GetValue());
		totalhdprice = (int)(hdprice* qty.GetValue());
	}
};

struct HoldOrderInfo : public HoldOrderInfoBase	 // �ֲֵ�
{
// Y : ��
// N : ��
	char istrhold;							// �Ƿ�ָʱ����
	
// Y : ��
// N : ��
	char splp;								// �Ƿ�����ֹ��ֹӯ
};

struct CloseOrderInfoBase					// ƽ�ֵ�����
{
	int hdorid;								// �ֲֵ���
	int ceorid;								// ƽ�ֵ���
	int uid;								// �ͻ�ID
	int fid;								// �ʽ�ID
	int id;									// ��ԼID

	// B : ����
	// S : ����
	char bsflag;							// ������־

	GWFloat qty;							// ƽ������	decimal(16, 2)

	int opprice;							// ���ּ۸�
	int hdprice;							// �ֲּ۸�
	int ceprice;							// ƽ�ּ۸�

	GWFloat pl;								// ӯ��	decimal(18, 2)
	GWFloat ce;								// ������ decimal(18, 2)

	CString ontime;							// ����ʱ�� 
	CString cetime;							// ƽ��ʱ�� 

	//	1 : �û��µ�
	//	2 : �����µ�
	//	3 : ϵͳ�µ�
	//	4 : ��ϳɽ�
	char oetype;							// ��������

	const int GetKeyID() const
	{
		return ceorid;
	}
};
struct CloseOrderInfo : public CloseOrderInfoBase							// ƽ�ֵ�
{
	int orderprice;							// ί�м۸�
};

struct OpenOrderInfoBase					// ���ֵ�����
{
	int hdorid;								// �ֲֵ���
	int onorid;								// ���ֵ���
	int uid;								// �ͻ�ID
	int fid;								// �ʽ�ID
	int id;									// ��ԼID

	// B : ����
	// S : ����
	char bsflag;							// ������־

	GWFloat qty;							// ��������	decimal(16, 2)

	int opprice;							// ���ּ۸�

	GWFloat ce;								// ������ decimal(18, 2)

	CString ontime;							// ����ʱ�� 

	//	1 : �û��µ�
	//	2 : �����µ�
	//	3 : ϵͳ�µ�
	//	4 : ��ϳɽ�
	char oetype;							// ��������

	//int orderprice;							// ί�м۸�

	const int GetKeyID() const
	{
		return onorid;
	}
};

struct OpenOrderInfo : public	OpenOrderInfoBase						// ���ֵ�
{
	
} ;

struct LimitOrderInfoBase					// �޼۵�����
{
	int ltorid;								// �޼۵���
	int uid;								// �ͻ�ID
	int fid;								// �ʽ�ID
	int id;									// ��ԼID

	// B : ����
	// S : ����
	char bsflag;							// ������־

	GWFloat qty;							// ����	decimal(16, 2)

	int ltprice;							// �޼�
	int slprice;							// ֹ���
	int spprice;							// ֹӯ��

	CString ontime;							// ����ʱ�� 

	// 1 : δ�ɽ�
	// 2 : �ѳ���
	// 3 : �ѳɽ�
	char orstaus;							// ί��״̬

	const int GetKeyID() const
	{
		return ltorid;
	}
};

struct LimitOrderInfo : public LimitOrderInfoBase	// �޼۵�
{
	
} ;

struct  FundIOInfo					// �����
{
	int uid;				// �ͻ�id

	// 'I' ���
	// 'O' ����
	char type;			// ��������

	GWFloat amount;		// ���
	
	// �ʽ����� ��ʱд��"d89267ba6e888426c8f798a04f2fb874"
	CString mypwd;
};

struct FundFlowInfoBase						// �ʽ���ˮ����	
{
	int uid;								// �ͻ�ID
	int fid;								// �ʽ�ID
	int flowid;								// �ʽ���ˮ��

// '1' : ���
// '2' : ������
// '3' : ����
// '4' : �������
// '5' : ������
// '6' : ƽ��ӯ��
// '7' : �ֲ�ӯ��
// '8' : ���ڷ�
// '9' : �ͻ����Ա�Ľ���������
// 'A' : �ͻ����Ա�Ľ���ӯ��
// 'B' : �ͻ����Ա�Ľ������ڷ�
	char type;								// ��ˮ����

	GWFloat amount;							// �䶯���	decimal(18, 2)
	GWFloat balance;						// �˻����	decimal(18, 2)

	int rnno;								// ��������
	int orid;								// ����ԱID
	
	CString descript;						// ����
	CString cedate;							// ����ʱ��
} ;

struct FundFlowInfo : public FundFlowInfoBase	// �ʽ���ˮ
{

};

struct HisFundFlowInfo : public FundFlowInfoBase
{
	CString tedate;							// ��������
};

struct HisCloseOrderInfo : public CloseOrderInfoBase								// ��ʷƽ�ֵ�
{
	CString tedate;							// ��������
};

struct HisOpenOrderInfo : public OpenOrderInfoBase								// ��ʷ���ֵ�
{
	CString tedate;							// ��������
};

struct HisLimitOrderInfo : public LimitOrderInfoBase								// ��ʷ�޼۵�
{
	CString tedate;							// ��������
};

struct HisHoldOrderInfo : public HoldOrderInfoBase	 // ��ʷ�ֲֵ�
{
	CString tedate;							// ��������

	int rnpe;								// �����

	GWFloat rnpl;							// ����ӯ��
};

enum CANCEL_LO_TYPE
{ 
	LO_SL_CLOSE		= 0,	//ֹ��ƽ�ֵ�
	LO_SP_CLOSE		= 1,	//ֹӯƽ�ֵ�
	LO_LIMIT_OPEN	= 2,	//�޼۽��ֵ�
	LO_TIME_ASIGN	= 3		//ָʱ��
};

struct OrderDealFinishInfo
{
	int uid;							// �û�ID
	int orid;							// ���ݺ�

// ��1���޼۽��ֳɽ�
// ��2��ֹ��ɽ�
// ��3��ֹӯ�ɽ�
// ��4���޼۽��ֳ���
// ��5��ָʱ���ֳɽ�
// ��6��ָʱƽ�ֳɽ�
// ��7��ָʱ����ʱ����
	int dltype;						// �ɽ�����

	int dlprice;						// �ɽ��۸�
	
	int nDecimalNum;					//С����λ��
	CString dltime;						// �ɽ�ʱ��
};

struct MaketStatusInfo					// �г�״̬
{
	CString	tedate;						// �г�����

// 1 : ����
// 4 : ����
// 6 : �������
	int mtstatus;						// ״̬
};

struct ExpertOperation					// ���ֲ���
{
	CString user;						// �û���
	CString nick;						// �ǳ�
	int	    uid;						// �û���ʶ
	int		nOp;						// ����
	time_t	time;						// ������ʱ��
	UINT	nOpCount;					// ��������

#ifdef _DEBUG
	~ExpertOperation()
	{
		OutputDebugString("~ExpertOperation\n");
	}
#endif
};

struct jsRet
{
	int ret;
	jsRet()
	{
		ret = 0;
	}
};

struct DelayJsHead
{
	CString bf;
	int rc;
	CString sn;

	DelayJsHead()
	{
		rc = 0;
	}
};

#define WM_TRADE_CMD_CLICK					(WM_USER + 800)
#define WM_TAB_SWTICH_NOTIFY				(WM_USER + 4)			//window stack �л�
#define WM_MENU_NAVIGATE_SWITCH_NOTIFY		(WM_USER + 801)			//�������л� �л�
#define WM_BROADCAST_QUOTE_NOTIFY			(WM_USER + 802)			//���������Ϣ�� LPARAM Ϊ SymbolObj*

#define WM_QUERY_HISTORY_NOTIFY				(WM_USER + 803)			//���������Ϣ�� LPARAM Ϊ SymbolObj*
#define WM_FORCE_REFRESH_ALL				(WM_USER + 804)			//ǿ��ˢ��
#define WM_SET_OPTIONAL				        (WM_USER + 805)			//������ѡ

#define em_Trade_View						(WM_USER + 806)
#define em_Login_View						(WM_USER + 807)
#define em_Lock_View						(WM_USER + 808)
#define em_Vistor_View						(WM_USER + 809)

enum EMTraderNodeMenu
{
	em_Menu_Node_Parent = 0,
	em_Menu_BuildOI = 200,		//����
	em_Menu_KilledOI,			//ƽ��
	em_Menu_LockTrade,			//�������׽���
	em_Menu_ForgotPwd,			//��������

	em_Menu_AccountInfo = 1000,	//�˺���Ϣ
	em_Menu_MoneySeq,		//�ʽ���ˮ

	em_Menu_OIDetail = 2000, //�ֲ���ϸ
	em_Menu_OITotal,		//�ֲֻ���
	em_Menu_FundManage,		//�ʽ����
	em_Menu_Deposit,		// ���
	em_Menu_TodayPriceList,	//����ָ�۵�
	em_Menu_TodayCommit,	//����ί��
	em_Menu_TodayTrade,		//���ճɽ�
	em_Menu_HistoryCommit,	//��ʷί��
	em_Menu_HistoryTrade,

	em_Menu_HistoryBuildOI,	//��ʷ����
	em_Menu_HistoryCloseOI,	//��ʷƽ��
	em_Menu_HistorySPOI,	//��ʷָ�۵�

	em_Menu_TradeTip = 3000,		//�ɽ���ʾ
	em_Menu_BlackBoarTip,		//������ʾ
	em_Menu_HelperManual,		//����˵��
	em_Menu_Close,				//�˳�

	em_Menu_Node_Max,

	em_Menu_Node_Wnd_Close,		//����׷��
};	

typedef struct tagNtPkgHeadEX : public NtPkgHead
{
	int nFlag;								// �Ƿ�ѹ��
	int lUnCompressLen;				// ѹ������
	tagNtPkgHeadEX()
	{
		nFlag = 0;
		lUnCompressLen = 0;
	}
} NtPkgHeadEX; //Э���ͷ

