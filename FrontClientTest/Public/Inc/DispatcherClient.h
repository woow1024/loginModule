
#ifndef __DISPATCHERCLIENT_H
#define __DISPATCHERCLIENT_H 1

#include "DispatcherInt.h"
#include "DispatcherMessage.h"
#include <WinSock2.h>
#include <map>
#pragma comment(lib,"WS2_32.lib")
class DISPATCHERCLIENT_API DispatcherClientEvent{
public:
	DispatcherClientEvent() ;
	virtual ~DispatcherClientEvent() ;
	virtual void ProcessEvent(int EventCode , const std::string& EventMsg) ;
} ;

const int CLIENT_EVENT_CONNECT			=	1 ;//连接成功
const int CLIENT_EVENT_DISCONNECT		=	2 ;//连接断开
const int CLIENT_EVENT_RECONNECT		=	3 ;//重连成功

class DispatcherNetIO ;

class DISPATCHERCLIENT_API ProxyInfo{
public:
	enum ProxyType
	{
		NoProxy,
		HttpProxy,
		Socks4proxy,
		Socks4Aproxy,
		Socks5proxy
	};

	ProxyType		Type ;							//代理类型
	std::string		Host ;							//代理服务器地址
	int				Port ;							//代理服务器端口
	std::string		UserName ;						//代理服务器认证
	std::string		Password ;						//代理服务器	

	ProxyInfo() ;
} ;
class DISPATCHERCLIENT_API LoginInfo
{
public:
	LoginInfo():client_type(0),loginport(0),hqport(0),downloadport(0){}
	//~LoginInfo(){}
	//以下为输入参数
	int client_type;//客户端类型
	std::string client_version;//客户端版本
	std::string loginip;//登陆服务器地址
	int loginport;//登陆服务器端口
	std::string username;//登陆的用户名称或用户手机号
	std::string passwd;//登陆密码
	std::string jsonin;//发送给登录服务器的json信息
	//以下为登陆服务器返回值
	std::string key;//md5(用户的手机号+密码+行情服务器key)
	std::string hqip;//登陆服务器根据负载自动分配的行情服务器地址
	int hqport;//登陆服务器根据负载自动分配的行情服务器端口
	std::string errstr;//登陆服务器返回的验证失败信息描述
	std::string phonenum;//登陆用户名，需要把用户名发送给行情服务器进行登陆
	std::string userid;//登陆用户手机号
	std::string downloadip;//下载非行情数据的地址
	int downloadport;//下载非行情数据的端口
	std::string downloaduser;//下载非行情数据的服务器登陆用户名
	std::string downloadpassword;//下载非行情数据的服务器的登陆密码
	int usertype;//用户类型
	std::string jsoninfo;//json格式的扩展信息(模拟交易服务器地址等信息)
};
class DISPATCHERCLIENT_API DispatcherClient{
private:
	DispatcherNetIO *netio_ ;
public:
//-----------------------xww add start 20121105----------------------
	//LoginManager m_LoginManager;
public:
	/*
	返回值错误信息从LoginInfo::errstr获取
	0:登陆成功
	1:连接出错
	2:网络发送超时
	3;网络接收超时
	4:网络连接出现异常
	以下返回值暂定如下
	-1:验证密码不正确
	-2:账号不在有效期
	-3:账号不存在
	-99:其他错误
	*/
	int Login(LoginInfo& mLoginInfo);
	int Logon(int encrypt_method , int heart_beat_interval , const char *default_appl_verid ,LoginInfo& mLoginInfo) ;
private:
	bool SetSocketBlock(SOCKET& sockfd,std::string& err,bool bBlock = false);
	void CloseSocket(SOCKET& sockfd);
	int SendProxyRequest(SOCKET& sockfd,std::string& loginip,int loginport,const ProxyInfo& info_,std::string& strerr);
	int SendHttpProxyRequest(SOCKET& sockfd,std::string& loginip,int loginport,const ProxyInfo& info_,std::string& strerr);
	int SendSock4ProxyRequest(SOCKET& sockfd,std::string& loginip,int loginport,const ProxyInfo& info_,std::string& strerr);
	int SendSock5ProxyRequest(SOCKET& sockfd,std::string& loginip,int loginport,const ProxyInfo& info_,std::string& strerr);

	std::string Base64Encode(const std::string& str);
	char Base2Chr(unsigned char n);
	int ConnectLoginServ(SOCKET& sockfd,const std::string& loginip,int loginport,std::string& strerr,int timeout_second);
	int SendTologinServ(const std::string sendbuf_str,SOCKET& sockfd,int timeout_send);
	int RecvDataFromLoginServ(SOCKET& sockfd,int timeout_recv,std::string& recvbuf,size_t& len);
	int RecvFromLoginServ(std::string& recvbuf,SOCKET& sockfd,int timeout_recv);
	int DealReturnLoginPack(const std::string& recvbuf_str,LoginInfo& mLoginInfo);
	int MakeSendLoginPack(std::string& sendbuf_str,const LoginInfo& mLoginInfo);
//-----------------------xww add end 20121105----------------------	
public:
	DispatcherClient() ;
	~DispatcherClient() ;

	uint32 Subscribe(const char *subject , bool snapshot = true) ;
	int Unsubscribe(const char *subject) ;
	int ActivateQuotes(const char *subjects[] , int count) ;

	int Unsubscribe(uint32 guid) ;
	int ActivateQuotes(uint32 *guids , int count) ;

	int WaitForPublish(DispatcherMessage *messages[] , int count , int timeout) ;

	int Logon(const char *ip , int port , 
		int encrypt_method , int heart_beat_interval , const char *default_appl_verid , const char *token) ;

	int Logout() ;

	void Manage(const char *command , const char *token) ;

	void SetParseFlag(int flag = 1) ;

	void SetHeartBeatFlag(int flag = 1);
	int GetRequstId();

	/*
		分钟K线，如果是5分钟，那么interval == 5
		2012-02-07
		暂时只支持1分钟K线
	*/
	int32 MinuteQuote(const char * subject , time_t start_time , int interval = 1) ;  

	int32 DealQuote(const char * subject , long npos = 0  , int size = 0 );

	//**********************************************************************
	/*新分钟k线、分笔接口*/
	/*
	* @param subject 主题 (Market.Security)
	* @param starttime 开始时间点 (0:当前点)
	* @param endtime 结束时间点 (0:起始点)
	* @param count 数量 (0:全部, >0 指定数量的)
	* @param dir 方向 (0:后->前,1:前->后)
	* @param type 类型 (0:根,1:日,2:月,3:年)
	* @param interval k线类型 (0:K1,6:Kday)
	* @param offset 偏移量 (默认为0,不偏移)
	*/
	int32 MinuteQuote(const char * subject , time_t start_time, int count, int dir , int type, int interval = 0, int offset = 0);
	int32 DealQuote(const char * subject , time_t start_time, time_t end_time, int count, int dir , int type);

	int32 DayQuote( const char * subject , time_t start_time, int count, int dir);//日线接口TodayMinQuote
	int32 TodayMinQuote( const char * subject , time_t start_time, int count = 0, int dir = 0);//当日分钟线接口
	int32 HisMinQuote( const char * subject , time_t start_time,time_t end_time, int count, int dir,int type);//历史1分钟线
	int32 HisFifMinQuote( const char * subject , time_t start_time,time_t end_time, int count, int dir,int type);//历史15分钟
	int32 NewsQuote( const char * subject , int  id /*= 0 */  , int count = 10,int type =0 /*= 0 */);
	int32 PriceTable( const char * subject);//分价请求
	int32 OrderList( const char * subject);
	int32 CSecurityNews( const char * subject ,  std::map<int,int> data );



	//**********************************************************************
	int32 SecurityList(const char * subject );

	int32 MarketInfo(const char * subject );

	/*
		2011-04-20
		group_id 必须是客户端唯一，否则将被替换掉
	*/
	int32 RequestSort(int group_id , const char * subjects[] , int subject_count , const char * field_name , 
			int begin = 0 , int size = -1 , bool asc = true) ;
	int32 RequestSort(const char * group_expr , const char * field_name , int begin = 0 , int size = -1) ;

	/*
		2011-10-25
		请求筛选
	*/
	int32 RequestFilter(const char * filter_expr , const char * field_name , int begin = 0 , int size = -1) ;

	/*
		2011-05-11
		快照，包含买一到买五，以及卖一到卖五
	*/
	int32 Snapshot(const char * subjects[] , int subject_count) ;
	int32 Snapshot(const char * subject) ;

	/*
		2011-05-11
		最优报价，包含买一以及卖一
	*/
	int32 BestQuote(const char * subject) ;
	int32 BestQuote(const char * subjects[] , int subject_count) ;

	/*
		2011-05-11
		最新报价，只包含最后一笔买价和卖价
	*/
	int32 LatestQuote(const char * subject) ;
	int32 LatestQuote(const char * subjects[] , int subject_count) ;

	/*
		2011-05-11
		设置日志级别
	*/
	void SetLogLevel(int level) ;

	/*
		2011-08-30
		设置客户端事件回调函数
	*/
	void SetClientEvent(DispatcherClientEvent * client_event) ;

	/*
		2011-10-10
		设置代理
	*/
	void SetProxyInfo(const ProxyInfo& info) ;
	/*
		数据发送接口
	*/
	
	int SendDispatcherMsg(DispatcherMessage *& p);
	static DispatcherClient * Create() ;
	void Destory();
} ;

#endif /** __DISPATCHERCLIENT_H */
