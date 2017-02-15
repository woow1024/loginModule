#pragma once

#include <gwtypes.h>
#include <trade\nt_protocol.h>

typedef struct						// 合约信息
{
	SymbolObj* pSymbolObj;			// 合约对应商品

	int id;							// 合约ID
	CString symbol;					// 合约代码
	CString name;					// 合约名称

	int gpid;						// 合约组别
	int dic;						// 合约位数
	//int aeunit;						// 合约单位
	double aeunit;						// 合约单位
	//double newaeunit;				// 合约单位
	int unitdic;							// 合约单位小数位数
	double minunitnum;				// 合约最小单位重量

	CString unitname;				// 合约单位中文含义

	GWFloat	spunit;					// 合约步进单位 decimal(16, 2)

#define DIRECTCONTRACT				 '1'		// 直接合约
#define INDIRDIRCONTRACT			 '2'		// 简介合约
#define INTERSECTCONTRACT			 '3'		// 交叉合约
#define NOTTRADECONTRACT			 '4'		// 非交易合约
	char type;						// 合约类型

#define CONTRACT_OPEN				'0'			// 开市
#define CONTRACT_CLOSE				'C'			// 休市市
#define CONTRACT_DISABLE			'D'			// 已过期的合约,不需要显示
	char testatus;								// 交易状态

	int sworder;								// 显示顺序
	
	GWFloat sellvalue;							// 卖点差	decimal(12, 4)
	GWFloat byvalue;							// 买点差	decimal(12, 4)
	GWFloat slvalue;							// 止损点差	decimal(12, 4)
	GWFloat spvalue;							// 止盈点差	decimal(12, 4)
	GWFloat ltvalue;							// 限价点差 decimal(12, 4)

#define PTTYPE_FIXED				'F'			// 固定
#define PTTYPE_PERSENT				'P'			// 比例
	char pttype;

	GWFloat mnvalue;							// 最小交易数量 decimal(16, 2)
	GWFloat mxvalue;							// 最大交易数量 decimal(16, 2)
	GWFloat bymxvalue;							// 最大买持仓量 decimal(16, 2)
	GWFloat slmxvalue;							// 最大卖持仓量 decimal(16, 2)

	int qeceunit;								// 行情最小变动单位


	GWFloat chargevalue;						// 手续费率
	GWFloat usedvalue;							// 保证金比例
} ContractDetailInfo;

struct FundInfoBase								// 资金信息
{
	int uid;									// 客户ID
	int fid;									// 资金ID

	GWFloat fmg;								// 自由保证金 decimal(18, 2)
	GWFloat umg;								// 占用保证金 decimal(18, 2)
	GWFloat fzmg;								// 冻结保证金 decimal(18, 2)
	GWFloat atin;								// 入金总计 decimal(18, 2)
	GWFloat atout;								// 出金总计 decimal(18, 2)
	GWFloat cha;								// 手续费 decimal(18, 2)
	GWFloat pl;									// 平仓盈亏 decimal(18, 2)
	GWFloat _int;								// 延期费 decimal(18, 2)
	GWFloat pvbe;								// 上日余额 decimal(18, 2)
} ;

struct FundInfo : public FundInfoBase			// 资金信息
{
	GWFloat rlbe;								// 可出金额 decimal(18, 2)

	CString d_rate;								// 日收益率
	
	CString wrate;								// 周收益率
	CString dayrank;							// 日排名
	CString than10days;							// 收益率大于10天数
	CString than10rank;							// 收益率大于10天数排名
	CString weekrank;							// 周排名
	CString windays;							// 连续盈利天数
	CString winrank;							// 连续盈利天数排名

	CString dchange;							// 日排名变化
	CString wchange;							// 周排名变化
	CString tenchange;							// 收益率大于10天数排名变化
	CString profitchange;						// 连续盈利天数排名变化

	CString amount;								// 初始资金
} ;

struct HisFundInfo : public FundInfoBase		// 历史资金信息
{
	CString tedate;								// 交易日期
	GWFloat rnpl;								// 结算盈余 decimal(18, 2)
};

typedef struct	PostOrderInfo					// 提交单据
{
//	int uid;									// 客户ID
//	int oruid;									// 操作员ID	
	int id;										// 合约ID	
	int orid;									// 单据ID

//	1 : 市价委托
//	2 : 限价委托
	char ortype;								// 委托类型

//	1 : 建
//	2 : 建带止损
//	3 : 建带止盈
//	4 : 建带止损止盈
//	5 : 平
//	6 : 平带止损
//	7 : 平带止盈
//	8 : 平带止损止盈
	char bltype;								// 单据类型

//	B : 买入
//	S : 卖出
	char bsflag;								// 买卖标志

	int orcrprice;								// 委托当前价格
	int orprice;								// 委托价格
	int slprice;								// 止损价
	int spprice;								// 止盈价

	GWFloat qty;								// 委托数量	decimal(16, 2)

	int rttime;									// 请求委托时间
	int pt;										// 允许成交点差
	
	GWFloat bkhdqty;							// 反手建仓量	decimal(16, 2)

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

struct HoldOrderInfoBase					// 持仓单基类
{
	int hdorid;								// 持仓单号
	int uid;								// 客户ID
	int fid;								// 资金ID
	int id;									// 合约ID

	// B : 买入
	// S : 卖出
	char bsflag;							// 买卖标志

	GWFloat qty;							// 持仓数量 decimal(16, 2)

	int opprice;							// 建仓价格
	int hdprice;							// 持仓价格
	int slprice;							// 止损价
	int spprice;							// 止盈价

	GWFloat _int;							// 延期费 decimal(18, 2)

	CString ontime;							// 建仓时间

	const int GetKeyID() const
	{
		return hdorid;
	}
};

struct HoldOrderInfoCount : public HoldOrderInfoBase	// 汇总
{
	int totalhdprice;								// 总计持仓价
	int totalopprice;								// 总计建仓价

	int countid;									// 汇总ID

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

struct HoldOrderInfo : public HoldOrderInfoBase	 // 持仓单
{
// Y : 是
// N : 否
	char istrhold;							// 是否指时建仓
	
// Y : 是
// N : 否
	char splp;								// 是否允许止损止盈
};

struct CloseOrderInfoBase					// 平仓单基类
{
	int hdorid;								// 持仓单号
	int ceorid;								// 平仓单号
	int uid;								// 客户ID
	int fid;								// 资金ID
	int id;									// 合约ID

	// B : 买入
	// S : 卖出
	char bsflag;							// 买卖标志

	GWFloat qty;							// 平仓数量	decimal(16, 2)

	int opprice;							// 建仓价格
	int hdprice;							// 持仓价格
	int ceprice;							// 平仓价格

	GWFloat pl;								// 盈亏	decimal(18, 2)
	GWFloat ce;								// 手续费 decimal(18, 2)

	CString ontime;							// 建仓时间 
	CString cetime;							// 平仓时间 

	//	1 : 用户下单
	//	2 : 代客下单
	//	3 : 系统下单
	//	4 : 撮合成交
	char oetype;							// 操作类型

	const int GetKeyID() const
	{
		return ceorid;
	}
};
struct CloseOrderInfo : public CloseOrderInfoBase							// 平仓单
{
	int orderprice;							// 委托价格
};

struct OpenOrderInfoBase					// 建仓单基类
{
	int hdorid;								// 持仓单号
	int onorid;								// 建仓单号
	int uid;								// 客户ID
	int fid;								// 资金ID
	int id;									// 合约ID

	// B : 买入
	// S : 卖出
	char bsflag;							// 买卖标志

	GWFloat qty;							// 建仓数量	decimal(16, 2)

	int opprice;							// 建仓价格

	GWFloat ce;								// 手续费 decimal(18, 2)

	CString ontime;							// 建仓时间 

	//	1 : 用户下单
	//	2 : 代客下单
	//	3 : 系统下单
	//	4 : 撮合成交
	char oetype;							// 操作类型

	//int orderprice;							// 委托价格

	const int GetKeyID() const
	{
		return onorid;
	}
};

struct OpenOrderInfo : public	OpenOrderInfoBase						// 建仓单
{
	
} ;

struct LimitOrderInfoBase					// 限价单基类
{
	int ltorid;								// 限价单号
	int uid;								// 客户ID
	int fid;								// 资金ID
	int id;									// 合约ID

	// B : 买入
	// S : 卖出
	char bsflag;							// 买卖标志

	GWFloat qty;							// 数量	decimal(16, 2)

	int ltprice;							// 限价
	int slprice;							// 止损价
	int spprice;							// 止盈价

	CString ontime;							// 建单时间 

	// 1 : 未成交
	// 2 : 已撤单
	// 3 : 已成交
	char orstaus;							// 委托状态

	const int GetKeyID() const
	{
		return ltorid;
	}
};

struct LimitOrderInfo : public LimitOrderInfoBase	// 限价单
{
	
} ;

struct  FundIOInfo					// 出入金
{
	int uid;				// 客户id

	// 'I' 入金
	// 'O' 出金
	char type;			// 操作类型

	GWFloat amount;		// 金额
	
	// 资金密码 暂时写死"d89267ba6e888426c8f798a04f2fb874"
	CString mypwd;
};

struct FundFlowInfoBase						// 资金流水基类	
{
	int uid;								// 客户ID
	int fid;								// 资金ID
	int flowid;								// 资金流水号

// '1' : 入金
// '2' : 入金调整
// '3' : 出金
// '4' : 出金调整
// '5' : 手续费
// '6' : 平仓盈亏
// '7' : 持仓盈亏
// '8' : 延期费
// '9' : 客户与会员的结算手续费
// 'A' : 客户与会员的结算盈亏
// 'B' : 客户与会员的结算延期费
	char type;								// 流水类型

	GWFloat amount;							// 变动金额	decimal(18, 2)
	GWFloat balance;						// 账户余额	decimal(18, 2)

	int rnno;								// 关联单号
	int orid;								// 操作员ID
	
	CString descript;						// 描述
	CString cedate;							// 发生时间
} ;

struct FundFlowInfo : public FundFlowInfoBase	// 资金流水
{

};

struct HisFundFlowInfo : public FundFlowInfoBase
{
	CString tedate;							// 交易日期
};

struct HisCloseOrderInfo : public CloseOrderInfoBase								// 历史平仓单
{
	CString tedate;							// 交易日期
};

struct HisOpenOrderInfo : public OpenOrderInfoBase								// 历史建仓单
{
	CString tedate;							// 交易日期
};

struct HisLimitOrderInfo : public LimitOrderInfoBase								// 历史限价单
{
	CString tedate;							// 交易日期
};

struct HisHoldOrderInfo : public HoldOrderInfoBase	 // 历史持仓单
{
	CString tedate;							// 交易日期

	int rnpe;								// 结算价

	GWFloat rnpl;							// 结算盈亏
};

enum CANCEL_LO_TYPE
{ 
	LO_SL_CLOSE		= 0,	//止损平仓单
	LO_SP_CLOSE		= 1,	//止盈平仓单
	LO_LIMIT_OPEN	= 2,	//限价建仓单
	LO_TIME_ASIGN	= 3		//指时单
};

struct OrderDealFinishInfo
{
	int uid;							// 用户ID
	int orid;							// 单据号

// ‘1’限价建仓成交
// ‘2’止损成交
// ‘3’止盈成交
// ‘4’限价建仓撤消
// ‘5’指时建仓成交
// ‘6’指时平仓成交
// ‘7’指时建仓时撤消
	int dltype;						// 成交类型

	int dlprice;						// 成交价格
	
	int nDecimalNum;					//小数点位数
	CString dltime;						// 成交时间
};

struct MaketStatusInfo					// 市场状态
{
	CString	tedate;						// 市场日期

// 1 : 开市
// 4 : 休市
// 6 : 结算完成
	int mtstatus;						// 状态
};

struct ExpertOperation					// 高手操作
{
	CString user;						// 用户名
	CString nick;						// 昵称
	int	    uid;						// 用户标识
	int		nOp;						// 操作
	time_t	time;						// 最后操作时间
	UINT	nOpCount;					// 操作次数

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
#define WM_TAB_SWTICH_NOTIFY				(WM_USER + 4)			//window stack 切换
#define WM_MENU_NAVIGATE_SWITCH_NOTIFY		(WM_USER + 801)			//导航栏切换 切换
#define WM_BROADCAST_QUOTE_NOTIFY			(WM_USER + 802)			//行情接收消息， LPARAM 为 SymbolObj*

#define WM_QUERY_HISTORY_NOTIFY				(WM_USER + 803)			//行情接收消息， LPARAM 为 SymbolObj*
#define WM_FORCE_REFRESH_ALL				(WM_USER + 804)			//强制刷新
#define WM_SET_OPTIONAL				        (WM_USER + 805)			//设置自选

#define em_Trade_View						(WM_USER + 806)
#define em_Login_View						(WM_USER + 807)
#define em_Lock_View						(WM_USER + 808)
#define em_Vistor_View						(WM_USER + 809)

enum EMTraderNodeMenu
{
	em_Menu_Node_Parent = 0,
	em_Menu_BuildOI = 200,		//建仓
	em_Menu_KilledOI,			//平仓
	em_Menu_LockTrade,			//锁定交易界面
	em_Menu_ForgotPwd,			//忘记密码

	em_Menu_AccountInfo = 1000,	//账号信息
	em_Menu_MoneySeq,		//资金流水

	em_Menu_OIDetail = 2000, //持仓明细
	em_Menu_OITotal,		//持仓汇总
	em_Menu_FundManage,		//资金出入
	em_Menu_Deposit,		// 入金
	em_Menu_TodayPriceList,	//当日指价单
	em_Menu_TodayCommit,	//当日委托
	em_Menu_TodayTrade,		//当日成交
	em_Menu_HistoryCommit,	//历史委托
	em_Menu_HistoryTrade,

	em_Menu_HistoryBuildOI,	//历史建仓
	em_Menu_HistoryCloseOI,	//历史平仓
	em_Menu_HistorySPOI,	//历史指价单

	em_Menu_TradeTip = 3000,		//成交提示
	em_Menu_BlackBoarTip,		//公告提示
	em_Menu_HelperManual,		//帮助说明
	em_Menu_Close,				//退出

	em_Menu_Node_Max,

	em_Menu_Node_Wnd_Close,		//高手追踪
};	

typedef struct tagNtPkgHeadEX : public NtPkgHead
{
	int nFlag;								// 是否压缩
	int lUnCompressLen;				// 压缩长度
	tagNtPkgHeadEX()
	{
		nFlag = 0;
		lUnCompressLen = 0;
	}
} NtPkgHeadEX; //协议包头

