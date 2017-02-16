#pragma once

#include <tchar.h>

#define DOUBLE_ZERO								0.00001				// 0值
#define INVALID_UINT							((UINT)-1)

#define UNKNOWN_SYMBOLCODE		"UNKNOWCODE"		// 未知代码
#define UNKNOWN_SYMBOLNAME		"未知商品"			// 未知名称

#define UP_ARROW_CHAR			"↑"				// 上箭头
#define DOWN_ARROW_CHAR			"↓"				// 下箭头

#define UP_TRIANGLE				"▲"				// 上三角
#define DOWN_TRIANGLE			"▼"				// 下三角
#define EQU_TRIANGLE			"◆"				// 平盘菱形


#define COLOR_BLACK			RGB(0, 0, 0)			// 黑色
#define COLOR_WHITE			RGB(255, 255, 255)		// 白色

#define GW_BACKGROUND_CLR	RGB(5, 5, 5)			// 背景色
#define GW_TITLE_CLR		RGB(192, 192, 192)		// 标题字体颜色
#define GW_SPLITLINE_CLR	RGB(99, 98, 98)			// 一般分割线

#define GW_PRICEUP_CLR		RGB(255, 50, 50)		// 上涨颜色
#define GW_PRICEDOWN_CLR	RGB(0, 230, 0)			// 下跌颜色
#define GW_PRICEEQU_CLR		RGB(255, 255, 255)		// 平盘颜色

#define GW_SYMBOLNAME_CLR	RGB(192, 192, 192)		// 商品名称

const CString HQCOMBINATION_LAYOUTNAME[] = { _T("MinLinView"), _T("KLineView"), _T("DealView"), _T("F10View"), _T("MultiPeriodView"),_T("PriceTabView") };
enum { LAYOUT_MINLINE = 0, LAYOUT_KLINE, LAYOUT_FULLDEAL, LAYOUT_F10, LAYOUT_MUTILPERIOD, LAYOUT_PRICETABLE, LAYOUT_UNKNOWN };

// 字段定义
#define MARKET_SGE							"SGE"									// 上海市场
#define MARKET_TPME							"TPME"									// 天津市场
#define MARKET_PMEC							"PMEC"									// 广州市场
#define MARKET_BPEX							_T("BPEX")								// 北京石油
#define MARKET_SZPEX							_T("SZPEX")							// 前海石油
#define MARKET_SELFSTOCK					"SELFSTOCK"								// 自选股市场
#define MARKET_TPMEALIAS					"JG"									// 天津市场摘牌

#define GW_FIRSTMARKETKEY					_T("sys_firstmarket")					// 首选市场
#define GW_HQ_ALLMARKETS					_T("hq_allmarkets")						// 行情中心首页多市场
#define GW_OMITMARKETKEY					_T("sys_omitmarket")					// 忽略的市场

#define SECURITYLIST_OBJ					"SECURITY"
#define DEAL_OBJ							"DEAL"
#define PUB_OBJ								"PUB"
#define SNAPSHOT_OBJ						"SNAPSHOT"
#define MULTISNAPSHOT_OBJ					"MULSNAPSHOT"							// 组合快照
#define SORTDATA_OBJ						"SORT"
#define MARKET_OBJ							"MARKET"
#define KLINE_OBJ							"MINUTE"
#define SYS_MSG_OBJ							"SYS"
#define NEWS_OBJ							"NEWS"
#define POP_NEWS_OBJ						"POPNEWS"
#define INFOBOM_OBJ							"PIONTINFO"											// 信息地雷
#define INFOBOM_PUSH_OBJ					"PUSHINFO"								// 信息地雷(推送)
#define SERVER_TA_OBJ						_T("BSSIGNAL")							//服务器指标
#define SERVER_BIGNEWS_OBJ					_T("NEWINFO")							//新闻资讯
#define ECONOMICDATA_OBJ				_T("ECONOMICDATA")					// 重要经济数据
#define HISECONOMICDATA_OBJ			_T("HISECONOMICDATA")				// 历史重要经济数据数量
	
#define DAYKLINE_OBJ						"DAYMQUTE"
#define DAYMINKLINE_OBJ						"MINUTENEW"
#define HISDAYMINKLINE_OBJ					"HISMINUTE"
#define HIS15MINKLINE_OBJ					"HISFIFMINUTE"

#define PRICETABLE_OBJ						"PRICETABLE"
#define ORDERLIST_OBJ						"ORDERLIST"
#define SECURITYNEWS_OBJ					"SECURITYNEWS"


#define FILED_TA_ID							_T("OffSet")					//指标ID
#define FILED_TA_BB_VOL						_T("AlertType")					//买入开仓量
#define FILED_TA_BK_VOL						_T("FxType")					//买入平仓量
#define FILED_TA_SB_VOL						_T("Begin")					//卖出开仓量
#define FILED_TA_SK_VOL						_T("Size")					//卖出平仓量
#define FILED_TA_TOTAL_VOL					_T("ChannelType")			//总持仓量
#define FILED_TA_BUY_TOTAL_VOL				_T("InternalCode")			//多单总量
#define FILED_TA_BUY_AVG_PRICE				_T("PrevClosePx")			//多单均价
#define FILED_TA_SELL_TOTAL_VOL				_T("I32Field")			//空单总量
#define FILED_TA_SELL_AVG_PRICE				_T("PrevWghtdPx")			//空单均价

// 系统级数据包主题
#define SYSMSG_KICK							"SYS.NOTICE.KICK"						// 重复登录，要求下线
#define SYSMSG_HEARTBEAT					"SYS.HEART.BEAT"						// 心跳
#define SYSMSG_MARKETINIT					"SYS.INIT.MARKET"						// 市场初始化
#define SYSMSG_CODEUPDATE					"SYS.NOTICE.SECURITYLIST"				// 码表更新
#define SYSMSG_CONNECTDROP					"SYS.NOTICE.DISCONN"					// 心跳超时导致的断线请求

#define OBJ_PACK_ERROR						"SYS.RSP.ERROR"							// 数据错误通知
#define SYSMSG_CLEARLOCALDATA				_T("SYS.NOTICE.CLEARLOCALDATA")			// 清除本地缓存
#define SYSMSG_GETALLVERSION				_T("GET.ALL.VERSION")					// 本地数据版本信息


#define FIELD_DATETIME						"DateTime"								// 时间
#define FIELD_LASTPX						"LatestPx"								// 最新价
#define FIELD_OPENPX						"OpenPx"								// 开盘
#define FIELD_HIGHPX						"HighPx"								// 最高
#define FIELD_LOWPX							"LowPx"									// 最低
#define FIELD_CLOSEPX						"ClosePx"								// 收盘
#define FIELD_PREVCLOSEPX					"PrevClosePx"							// 昨收
#define FIELD_PRESETTLEMENTPX				"PreSettlementPx"						// 昨结算
#define FIELD_AVERAGEPX						"AvgPx"									// 均价
#define FIELD_VOLUME						"TotalTradeVolume"						// 成交量
#define FIELD_AMOUNT						"TotalTradeAmount"						// 成交额
#define FIELD_VOLUMENOW						"TradeVolume"							// 现量
#define FIELD_AMOUNTNOW						"TradeAmount"							// 现额
#define FIELD_TRADEGEARS					"ChannelType"							// 买卖档位

#define FIELD_BIDPX							"BidPx"									// 买
#define FIELD_ASKPX							"AskPx"									// 卖

#define FIELD_BIDPX1						"BidPx1"								// 买一
#define FIELD_BIDPX2						"BidPx2"								// 买二
#define FIELD_BIDPX3						"BidPx3"								// 买三
#define FIELD_BIDPX4						"BidPx4"								// 买四
#define FIELD_BIDPX5						"BidPx5"								// 买五

#define FIELD_ASKPX1						"AskPx1"								// 卖一
#define FIELD_ASKPX2						"AskPx2"								// 卖二
#define FIELD_ASKPX3						"AskPx3"								// 卖三
#define FIELD_ASKPX4						"AskPx4"								// 卖四
#define FIELD_ASKPX5						"AskPx5"								// 卖五

#define FIELD_BIDVOLUME1					"BidVolume1"							// 买一量
#define FIELD_BIDVOLUME2					"BidVolume2"							// 买二量
#define FIELD_BIDVOLUME3					"BidVolume3"							// 买三量
#define FIELD_BIDVOLUME4					"BidVolume4"							// 买四量
#define FIELD_BIDVOLUME5					"BidVolume5"							// 买五量

#define FIELD_ASKVOLUME1					"AskVolume1"							// 卖一量
#define FIELD_ASKVOLUME2					"AskVolume2"							// 卖二量
#define FIELD_ASKVOLUME3					"AskVolume3"							// 卖三量
#define FIELD_ASKVOLUME4					"AskVolume4"							// 卖四量
#define FIELD_ASKVOLUME5					"AskVolume5"							// 卖五量

#define FIELD_POSITIONS						"PreOpenPosition"						// 持仓量

#define FIELD_SECURITYID					"SecurityID"							// 代码
#define FIELD_MARKET						"Market"								// 市场
#define FIELD_SECURITYTYPE					"SecurityType"							// 数据包类型
#define FIELD_ABBRNAME						"AbbrName"								// 中文名称
#define FIELD_KEYWORDS						"Keywords"								// 关键字
#define FIELD_CHANGE						"Change"								// 涨跌
#define FIELD_CHANGERATE					"ChangeRate"							// 涨跌幅
#define FIELD_SUBJECTNAME					"SubjectName"							// 对象名称		
#define FIELD_TRADECODE						"TradeCode"								// 当前交易时间		
#define FIELD_CURRENCYTYPE					"OfferQuoteSide"						// 货币类型	
#define FIELD_QUOTEUNIT						"BidQuoteSide"							// 报价单位							
#define FIELD_DIGIT							"MinuteSpan"							// 小数位数
#define	FIELD_TRADESECTION					FIELD_KEYWORDS							// 市场交易时间段
#define	FIELD_TRADESECTION_SHORT			_T("BondCategory")						// 品种交易时间段
#define	FIELD_TRADESECTION_EX				_T("AskMarketMaker")						// 市场交易时间段
#define	FIELD_TRADESECTION_SYMBOL_FULL		_T("AskSettlType")						// 品种交易时间段
#define FIELD_KLINETYPE						"MinuteSpan"							// K线类型
#define FIELD_KLINEVERSION					"StringField"								// K线版本
#define FIELD_DEALCOUNT						"ListCount"								// 逐笔总数

#define FIELD_COMPRESSEDDATA				"SecureData"							// 压缩数据
#define FIELD_COMPRESSEDLEN					"Size"									// 压缩后的长度
#define FIELD_UNCOMPRESSLEN					"SecureDataLen"							// 解压后的长度

#define  FIELD_KLINEDATE					"EndDateTime"							// K线交易日期

#define FILED_REQUESTID						"RequestID"
#define FILED_TOTALMESSAGE					"TotalMessage"
#define FILED_SEQUENCE						"Sequence"

#define FILED_NEWSTIME						"Headline"								// 新闻时间
#define FILED_NEWSID						"NewsID"								// 新闻ID
#define FILED_NEWSTITLE						"Summary"								// 新闻标题
#define FILED_NEWSLINK						"URLLink"								// 链接
#define FILED_CLIENT_TYPE						_T("FilterExpr")					//新版用户类型专用字段


#define FIELD_DATA										"DATA"								//
#define FIELD_MATURITYDATE						"MaturityDate"
#define FIELD_REQUESTRESULTCODE			"RequestResultCode"
#define FIELD_BIDTIME									"BidTime"				//交易日
#define FIELD_UPDATETIME							"UpdateTime"				//更新时间
#define FIELD_CREATETIME							"BeginDateTime"
#define FIELD_SUMMARY								"Summary"
#define FIELD_FILTERRULEID							"FilterRuleID"
#define FIELD_FILTEREXPR								"FilterExpr"


#define FIELD_KEY_INDICATOR				_T("TA")			//技术指标
#define FIELD_KEY_ASSINDICATOR		    _T("ASSINDICATOR")		// 辅图指标
#define FIELD_KEY_PERIOD				_T("PERIOD")		//K线周期
#define FIELD_KEY_EXTEND				_T("EXTEND")		//展开
#define FIELD_KEY_VIEW					_T("VIEW")			//view
#define FIELD_KEY_MARKET				_T("MARKET")		//K线周期
#define FIELD_KEY_CODE					_T("SYMBOL")			//代码
#define FIELD_KEY_TAB					_T("TAB")			//代码
#define FIELD_KEY_FULLSCREEN			_T("FULLSCREEN")			//代码
#define FIELD_KEY_ASSCHARTNUM		    _T("ASSCHARTNUM")	// 辅图数量
#define FIELD_KEY_QKX        		    _T("QKX")			// 乾坤线

#define FIELD_KEY_CHART_RBMENU			_T("RBMENU")		// 右键菜单
#define FIELD_KEY_CHART_TOOLBAR		_T("TOOLBAR")	// 工具栏
#define FIELD_KEY_CHART_INDICATOR		_T("INDICATOR")	// 指标栏
#define FIELD_KEY_CHART_CROSS			_T("CROSS")		// 十字光标
#define FIELD_KEY_CHAR_MINIMODE		_T("MINIMODE")	// 迷你模式
#define FIELD_KEY_CHART_INDICATOR_WND_CLS	_T("INDICATOR_WND_CLS")	// 指标子窗口中的关闭按钮


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

const long DEAL_LOCALFILE_VISION				= 20121230;			// 逐笔本地文件版本号
const long KLINE_LOCALFILE_VISION				= 20121214;			// K线本地文件版本号

// 新的协议ID				20130822  add by carrot	
#define PROTID_INFOBOM_PUSH 							38		// 信息地雷推送
#define PROTID_INFOBOM_REQUEST 							39		// 信息地雷请求


#define PROTID_LOGOUT									2		// 用户退出
#define PROTID_SERVERTA_PUSH 							46		// 银利一号数据推送
#define PROTID_SERVERTA_REQUEST 						47		// 银利一号多日分时数据请求
#define PROTID_SERVERTA_HISTORY_REQUEST 				51		// 银利一号历史数据请求
#define PROTID_BIGNEWS_PUSH 							48		// 信息地雷推送
#define PROTID_BIGNEWS_REQUEST 							49		// 信息地雷请求
#define	PROTID_ECONOMICDATA_REQUEST						52		// 重要经济数据
#define	PROTID_ALLVERSION_REQUEST						58		// 请求本地数据版本信息
#define	PROTID_MYATTENTIONLIST_REQUEST						110		// 请求关注列表
#define	PROTID_MYATTENTIONLIST_PUSH							111		//  关注列表推送

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

