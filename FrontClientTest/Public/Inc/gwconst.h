#pragma once

#include <tchar.h>

#define DOUBLE_ZERO								0.00001				// 0ֵ
#define INVALID_UINT							((UINT)-1)

#define UNKNOWN_SYMBOLCODE		"UNKNOWCODE"		// δ֪����
#define UNKNOWN_SYMBOLNAME		"δ֪��Ʒ"			// δ֪����

#define UP_ARROW_CHAR			"��"				// �ϼ�ͷ
#define DOWN_ARROW_CHAR			"��"				// �¼�ͷ

#define UP_TRIANGLE				"��"				// ������
#define DOWN_TRIANGLE			"��"				// ������
#define EQU_TRIANGLE			"��"				// ƽ������


#define COLOR_BLACK			RGB(0, 0, 0)			// ��ɫ
#define COLOR_WHITE			RGB(255, 255, 255)		// ��ɫ

#define GW_BACKGROUND_CLR	RGB(5, 5, 5)			// ����ɫ
#define GW_TITLE_CLR		RGB(192, 192, 192)		// ����������ɫ
#define GW_SPLITLINE_CLR	RGB(99, 98, 98)			// һ��ָ���

#define GW_PRICEUP_CLR		RGB(255, 50, 50)		// ������ɫ
#define GW_PRICEDOWN_CLR	RGB(0, 230, 0)			// �µ���ɫ
#define GW_PRICEEQU_CLR		RGB(255, 255, 255)		// ƽ����ɫ

#define GW_SYMBOLNAME_CLR	RGB(192, 192, 192)		// ��Ʒ����

const CString HQCOMBINATION_LAYOUTNAME[] = { _T("MinLinView"), _T("KLineView"), _T("DealView"), _T("F10View"), _T("MultiPeriodView"),_T("PriceTabView") };
enum { LAYOUT_MINLINE = 0, LAYOUT_KLINE, LAYOUT_FULLDEAL, LAYOUT_F10, LAYOUT_MUTILPERIOD, LAYOUT_PRICETABLE, LAYOUT_UNKNOWN };

// �ֶζ���
#define MARKET_SGE							"SGE"									// �Ϻ��г�
#define MARKET_TPME							"TPME"									// ����г�
#define MARKET_PMEC							"PMEC"									// �����г�
#define MARKET_BPEX							_T("BPEX")								// ����ʯ��
#define MARKET_SZPEX							_T("SZPEX")							// ǰ��ʯ��
#define MARKET_SELFSTOCK					"SELFSTOCK"								// ��ѡ���г�
#define MARKET_TPMEALIAS					"JG"									// ����г�ժ��

#define GW_FIRSTMARKETKEY					_T("sys_firstmarket")					// ��ѡ�г�
#define GW_HQ_ALLMARKETS					_T("hq_allmarkets")						// ����������ҳ���г�
#define GW_OMITMARKETKEY					_T("sys_omitmarket")					// ���Ե��г�

#define SECURITYLIST_OBJ					"SECURITY"
#define DEAL_OBJ							"DEAL"
#define PUB_OBJ								"PUB"
#define SNAPSHOT_OBJ						"SNAPSHOT"
#define MULTISNAPSHOT_OBJ					"MULSNAPSHOT"							// ��Ͽ���
#define SORTDATA_OBJ						"SORT"
#define MARKET_OBJ							"MARKET"
#define KLINE_OBJ							"MINUTE"
#define SYS_MSG_OBJ							"SYS"
#define NEWS_OBJ							"NEWS"
#define POP_NEWS_OBJ						"POPNEWS"
#define INFOBOM_OBJ							"PIONTINFO"											// ��Ϣ����
#define INFOBOM_PUSH_OBJ					"PUSHINFO"								// ��Ϣ����(����)
#define SERVER_TA_OBJ						_T("BSSIGNAL")							//������ָ��
#define SERVER_BIGNEWS_OBJ					_T("NEWINFO")							//������Ѷ
#define ECONOMICDATA_OBJ				_T("ECONOMICDATA")					// ��Ҫ��������
#define HISECONOMICDATA_OBJ			_T("HISECONOMICDATA")				// ��ʷ��Ҫ������������
	
#define DAYKLINE_OBJ						"DAYMQUTE"
#define DAYMINKLINE_OBJ						"MINUTENEW"
#define HISDAYMINKLINE_OBJ					"HISMINUTE"
#define HIS15MINKLINE_OBJ					"HISFIFMINUTE"

#define PRICETABLE_OBJ						"PRICETABLE"
#define ORDERLIST_OBJ						"ORDERLIST"
#define SECURITYNEWS_OBJ					"SECURITYNEWS"


#define FILED_TA_ID							_T("OffSet")					//ָ��ID
#define FILED_TA_BB_VOL						_T("AlertType")					//���뿪����
#define FILED_TA_BK_VOL						_T("FxType")					//����ƽ����
#define FILED_TA_SB_VOL						_T("Begin")					//����������
#define FILED_TA_SK_VOL						_T("Size")					//����ƽ����
#define FILED_TA_TOTAL_VOL					_T("ChannelType")			//�ֲܳ���
#define FILED_TA_BUY_TOTAL_VOL				_T("InternalCode")			//�൥����
#define FILED_TA_BUY_AVG_PRICE				_T("PrevClosePx")			//�൥����
#define FILED_TA_SELL_TOTAL_VOL				_T("I32Field")			//�յ�����
#define FILED_TA_SELL_AVG_PRICE				_T("PrevWghtdPx")			//�յ�����

// ϵͳ�����ݰ�����
#define SYSMSG_KICK							"SYS.NOTICE.KICK"						// �ظ���¼��Ҫ������
#define SYSMSG_HEARTBEAT					"SYS.HEART.BEAT"						// ����
#define SYSMSG_MARKETINIT					"SYS.INIT.MARKET"						// �г���ʼ��
#define SYSMSG_CODEUPDATE					"SYS.NOTICE.SECURITYLIST"				// ������
#define SYSMSG_CONNECTDROP					"SYS.NOTICE.DISCONN"					// ������ʱ���µĶ�������

#define OBJ_PACK_ERROR						"SYS.RSP.ERROR"							// ���ݴ���֪ͨ
#define SYSMSG_CLEARLOCALDATA				_T("SYS.NOTICE.CLEARLOCALDATA")			// ������ػ���
#define SYSMSG_GETALLVERSION				_T("GET.ALL.VERSION")					// �������ݰ汾��Ϣ


#define FIELD_DATETIME						"DateTime"								// ʱ��
#define FIELD_LASTPX						"LatestPx"								// ���¼�
#define FIELD_OPENPX						"OpenPx"								// ����
#define FIELD_HIGHPX						"HighPx"								// ���
#define FIELD_LOWPX							"LowPx"									// ���
#define FIELD_CLOSEPX						"ClosePx"								// ����
#define FIELD_PREVCLOSEPX					"PrevClosePx"							// ����
#define FIELD_PRESETTLEMENTPX				"PreSettlementPx"						// �����
#define FIELD_AVERAGEPX						"AvgPx"									// ����
#define FIELD_VOLUME						"TotalTradeVolume"						// �ɽ���
#define FIELD_AMOUNT						"TotalTradeAmount"						// �ɽ���
#define FIELD_VOLUMENOW						"TradeVolume"							// ����
#define FIELD_AMOUNTNOW						"TradeAmount"							// �ֶ�
#define FIELD_TRADEGEARS					"ChannelType"							// ������λ

#define FIELD_BIDPX							"BidPx"									// ��
#define FIELD_ASKPX							"AskPx"									// ��

#define FIELD_BIDPX1						"BidPx1"								// ��һ
#define FIELD_BIDPX2						"BidPx2"								// ���
#define FIELD_BIDPX3						"BidPx3"								// ����
#define FIELD_BIDPX4						"BidPx4"								// ����
#define FIELD_BIDPX5						"BidPx5"								// ����

#define FIELD_ASKPX1						"AskPx1"								// ��һ
#define FIELD_ASKPX2						"AskPx2"								// ����
#define FIELD_ASKPX3						"AskPx3"								// ����
#define FIELD_ASKPX4						"AskPx4"								// ����
#define FIELD_ASKPX5						"AskPx5"								// ����

#define FIELD_BIDVOLUME1					"BidVolume1"							// ��һ��
#define FIELD_BIDVOLUME2					"BidVolume2"							// �����
#define FIELD_BIDVOLUME3					"BidVolume3"							// ������
#define FIELD_BIDVOLUME4					"BidVolume4"							// ������
#define FIELD_BIDVOLUME5					"BidVolume5"							// ������

#define FIELD_ASKVOLUME1					"AskVolume1"							// ��һ��
#define FIELD_ASKVOLUME2					"AskVolume2"							// ������
#define FIELD_ASKVOLUME3					"AskVolume3"							// ������
#define FIELD_ASKVOLUME4					"AskVolume4"							// ������
#define FIELD_ASKVOLUME5					"AskVolume5"							// ������

#define FIELD_POSITIONS						"PreOpenPosition"						// �ֲ���

#define FIELD_SECURITYID					"SecurityID"							// ����
#define FIELD_MARKET						"Market"								// �г�
#define FIELD_SECURITYTYPE					"SecurityType"							// ���ݰ�����
#define FIELD_ABBRNAME						"AbbrName"								// ��������
#define FIELD_KEYWORDS						"Keywords"								// �ؼ���
#define FIELD_CHANGE						"Change"								// �ǵ�
#define FIELD_CHANGERATE					"ChangeRate"							// �ǵ���
#define FIELD_SUBJECTNAME					"SubjectName"							// ��������		
#define FIELD_TRADECODE						"TradeCode"								// ��ǰ����ʱ��		
#define FIELD_CURRENCYTYPE					"OfferQuoteSide"						// ��������	
#define FIELD_QUOTEUNIT						"BidQuoteSide"							// ���۵�λ							
#define FIELD_DIGIT							"MinuteSpan"							// С��λ��
#define	FIELD_TRADESECTION					FIELD_KEYWORDS							// �г�����ʱ���
#define	FIELD_TRADESECTION_SHORT			_T("BondCategory")						// Ʒ�ֽ���ʱ���
#define	FIELD_TRADESECTION_EX				_T("AskMarketMaker")						// �г�����ʱ���
#define	FIELD_TRADESECTION_SYMBOL_FULL		_T("AskSettlType")						// Ʒ�ֽ���ʱ���
#define FIELD_KLINETYPE						"MinuteSpan"							// K������
#define FIELD_KLINEVERSION					"StringField"								// K�߰汾
#define FIELD_DEALCOUNT						"ListCount"								// �������

#define FIELD_COMPRESSEDDATA				"SecureData"							// ѹ������
#define FIELD_COMPRESSEDLEN					"Size"									// ѹ����ĳ���
#define FIELD_UNCOMPRESSLEN					"SecureDataLen"							// ��ѹ��ĳ���

#define  FIELD_KLINEDATE					"EndDateTime"							// K�߽�������

#define FILED_REQUESTID						"RequestID"
#define FILED_TOTALMESSAGE					"TotalMessage"
#define FILED_SEQUENCE						"Sequence"

#define FILED_NEWSTIME						"Headline"								// ����ʱ��
#define FILED_NEWSID						"NewsID"								// ����ID
#define FILED_NEWSTITLE						"Summary"								// ���ű���
#define FILED_NEWSLINK						"URLLink"								// ����
#define FILED_CLIENT_TYPE						_T("FilterExpr")					//�°��û�����ר���ֶ�


#define FIELD_DATA										"DATA"								//
#define FIELD_MATURITYDATE						"MaturityDate"
#define FIELD_REQUESTRESULTCODE			"RequestResultCode"
#define FIELD_BIDTIME									"BidTime"				//������
#define FIELD_UPDATETIME							"UpdateTime"				//����ʱ��
#define FIELD_CREATETIME							"BeginDateTime"
#define FIELD_SUMMARY								"Summary"
#define FIELD_FILTERRULEID							"FilterRuleID"
#define FIELD_FILTEREXPR								"FilterExpr"


#define FIELD_KEY_INDICATOR				_T("TA")			//����ָ��
#define FIELD_KEY_ASSINDICATOR		    _T("ASSINDICATOR")		// ��ͼָ��
#define FIELD_KEY_PERIOD				_T("PERIOD")		//K������
#define FIELD_KEY_EXTEND				_T("EXTEND")		//չ��
#define FIELD_KEY_VIEW					_T("VIEW")			//view
#define FIELD_KEY_MARKET				_T("MARKET")		//K������
#define FIELD_KEY_CODE					_T("SYMBOL")			//����
#define FIELD_KEY_TAB					_T("TAB")			//����
#define FIELD_KEY_FULLSCREEN			_T("FULLSCREEN")			//����
#define FIELD_KEY_ASSCHARTNUM		    _T("ASSCHARTNUM")	// ��ͼ����
#define FIELD_KEY_QKX        		    _T("QKX")			// Ǭ����

#define FIELD_KEY_CHART_RBMENU			_T("RBMENU")		// �Ҽ��˵�
#define FIELD_KEY_CHART_TOOLBAR		_T("TOOLBAR")	// ������
#define FIELD_KEY_CHART_INDICATOR		_T("INDICATOR")	// ָ����
#define FIELD_KEY_CHART_CROSS			_T("CROSS")		// ʮ�ֹ��
#define FIELD_KEY_CHAR_MINIMODE		_T("MINIMODE")	// ����ģʽ
#define FIELD_KEY_CHART_INDICATOR_WND_CLS	_T("INDICATOR_WND_CLS")	// ָ���Ӵ����еĹرհ�ť


#define FILED_POPWND_AUTODEL				_T("AUTODEL")
#define FIELD_POPWND_WNDWIDTH				_T("WNDWIDTH")
#define FIELD_POPWND_WNDHIGH				_T("WNDHIGH")
#define FIELD_POPWND_WNDTITLE				_T("WNDTITLE")
#define FIELD_POPWND_WNDBORDER			_T("WNDBORDER")
#define FIELD_POPWND_WNDCLOSEBTN		_T("WNDCLOSEBTN")
#define FIELD_POPWND_WNDMINBTN			_T("WNDMINBTN")
#define FIELD_POPWND_WNDMAXBTN			_T("WNDMAXBTN")
#define FIELD_POPWND_WNDRESIZE			_T("WNDRESIZE")
#define FIELD_POPWND_WEBURL					_T("WEBURL")
#define FIELD_USER_NAME						_T("Username")

const long DEAL_LOCALFILE_VISION				= 20121230;			// ��ʱ����ļ��汾��
const long KLINE_LOCALFILE_VISION				= 20121214;			// K�߱����ļ��汾��

// �µ�Э��ID				20130822  add by carrot	
#define PROTID_INFOBOM_PUSH 							38		// ��Ϣ��������
#define PROTID_INFOBOM_REQUEST 							39		// ��Ϣ��������


#define PROTID_LOGOUT									2		// �û��˳�
#define PROTID_SERVERTA_PUSH 							46		// ����һ����������
#define PROTID_SERVERTA_REQUEST 						47		// ����һ�Ŷ��շ�ʱ��������
#define PROTID_SERVERTA_HISTORY_REQUEST 				51		// ����һ����ʷ��������
#define PROTID_BIGNEWS_PUSH 							48		// ��Ϣ��������
#define PROTID_BIGNEWS_REQUEST 							49		// ��Ϣ��������
#define	PROTID_ECONOMICDATA_REQUEST						52		// ��Ҫ��������
#define	PROTID_ALLVERSION_REQUEST						58		// ���󱾵����ݰ汾��Ϣ
#define	PROTID_MYATTENTIONLIST_REQUEST						110		// �����ע�б�
#define	PROTID_MYATTENTIONLIST_PUSH							111		//  ��ע�б�����

#define  G_FREE_ONE_MEM(pMem) \
	if(pMem)\
{\
	delete pMem;\
}\
	pMem = NULL;


#define   G_FREE_ARRAY_MEM(pMemArr) \
	if(pMemArr)\
{\
	delete []pMemArr;\
}\
	pMemArr = NULL;



#define   G_FREE_ONE_WND( pCtrl)\
{\
	if(pCtrl && pCtrl->GetSafeHwnd())\
{\
	pCtrl->DestroyWindow();\
}\
	delete pCtrl;\
	pCtrl = NULL;\
}

#define G_FREE_OBJ(pObj)\
	if (pObj)\
	{\
		pObj->Close();\
		delete pObj;\
		pObj = NULL;\
	}

