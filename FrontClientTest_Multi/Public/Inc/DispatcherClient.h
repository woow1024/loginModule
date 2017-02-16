
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

const int CLIENT_EVENT_CONNECT			=	1 ;//���ӳɹ�
const int CLIENT_EVENT_DISCONNECT		=	2 ;//���ӶϿ�
const int CLIENT_EVENT_RECONNECT		=	3 ;//�����ɹ�

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

	ProxyType		Type ;							//��������
	std::string		Host ;							//�����������ַ
	int				Port ;							//����������˿�
	std::string		UserName ;						//�����������֤
	std::string		Password ;						//���������	

	ProxyInfo() ;
} ;
class DISPATCHERCLIENT_API LoginInfo
{
public:
	LoginInfo():client_type(0),loginport(0),hqport(0),downloadport(0){}
	//~LoginInfo(){}
	//����Ϊ�������
	int client_type;//�ͻ�������
	std::string client_version;//�ͻ��˰汾
	std::string loginip;//��½��������ַ
	int loginport;//��½�������˿�
	std::string username;//��½���û����ƻ��û��ֻ���
	std::string passwd;//��½����
	std::string jsonin;//���͸���¼��������json��Ϣ
	//����Ϊ��½����������ֵ
	std::string key;//md5(�û����ֻ���+����+���������key)
	std::string hqip;//��½���������ݸ����Զ�����������������ַ
	int hqport;//��½���������ݸ����Զ����������������˿�
	std::string errstr;//��½���������ص���֤ʧ����Ϣ����
	std::string phonenum;//��½�û�������Ҫ���û������͸�������������е�½
	std::string userid;//��½�û��ֻ���
	std::string downloadip;//���ط��������ݵĵ�ַ
	int downloadport;//���ط��������ݵĶ˿�
	std::string downloaduser;//���ط��������ݵķ�������½�û���
	std::string downloadpassword;//���ط��������ݵķ������ĵ�½����
	int usertype;//�û�����
	std::string jsoninfo;//json��ʽ����չ��Ϣ(ģ�⽻�׷�������ַ����Ϣ)
};
class DISPATCHERCLIENT_API DispatcherClient{
private:
	DispatcherNetIO *netio_ ;
public:
//-----------------------xww add start 20121105----------------------
	//LoginManager m_LoginManager;
public:
	/*
	����ֵ������Ϣ��LoginInfo::errstr��ȡ
	0:��½�ɹ�
	1:���ӳ���
	2:���緢�ͳ�ʱ
	3;������ճ�ʱ
	4:�������ӳ����쳣
	���·���ֵ�ݶ�����
	-1:��֤���벻��ȷ
	-2:�˺Ų�����Ч��
	-3:�˺Ų�����
	-99:��������
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
		����K�ߣ������5���ӣ���ôinterval == 5
		2012-02-07
		��ʱֻ֧��1����K��
	*/
	int32 MinuteQuote(const char * subject , time_t start_time , int interval = 1) ;  

	int32 DealQuote(const char * subject , long npos = 0  , int size = 0 );

	//**********************************************************************
	/*�·���k�ߡ��ֱʽӿ�*/
	/*
	* @param subject ���� (Market.Security)
	* @param starttime ��ʼʱ��� (0:��ǰ��)
	* @param endtime ����ʱ��� (0:��ʼ��)
	* @param count ���� (0:ȫ��, >0 ָ��������)
	* @param dir ���� (0:��->ǰ,1:ǰ->��)
	* @param type ���� (0:��,1:��,2:��,3:��)
	* @param interval k������ (0:K1,6:Kday)
	* @param offset ƫ���� (Ĭ��Ϊ0,��ƫ��)
	*/
	int32 MinuteQuote(const char * subject , time_t start_time, int count, int dir , int type, int interval = 0, int offset = 0);
	int32 DealQuote(const char * subject , time_t start_time, time_t end_time, int count, int dir , int type);

	int32 DayQuote( const char * subject , time_t start_time, int count, int dir);//���߽ӿ�TodayMinQuote
	int32 TodayMinQuote( const char * subject , time_t start_time, int count = 0, int dir = 0);//���շ����߽ӿ�
	int32 HisMinQuote( const char * subject , time_t start_time,time_t end_time, int count, int dir,int type);//��ʷ1������
	int32 HisFifMinQuote( const char * subject , time_t start_time,time_t end_time, int count, int dir,int type);//��ʷ15����
	int32 NewsQuote( const char * subject , int  id /*= 0 */  , int count = 10,int type =0 /*= 0 */);
	int32 PriceTable( const char * subject);//�ּ�����
	int32 OrderList( const char * subject);
	int32 CSecurityNews( const char * subject ,  std::map<int,int> data );



	//**********************************************************************
	int32 SecurityList(const char * subject );

	int32 MarketInfo(const char * subject );

	/*
		2011-04-20
		group_id �����ǿͻ���Ψһ�����򽫱��滻��
	*/
	int32 RequestSort(int group_id , const char * subjects[] , int subject_count , const char * field_name , 
			int begin = 0 , int size = -1 , bool asc = true) ;
	int32 RequestSort(const char * group_expr , const char * field_name , int begin = 0 , int size = -1) ;

	/*
		2011-10-25
		����ɸѡ
	*/
	int32 RequestFilter(const char * filter_expr , const char * field_name , int begin = 0 , int size = -1) ;

	/*
		2011-05-11
		���գ�������һ�����壬�Լ���һ������
	*/
	int32 Snapshot(const char * subjects[] , int subject_count) ;
	int32 Snapshot(const char * subject) ;

	/*
		2011-05-11
		���ű��ۣ�������һ�Լ���һ
	*/
	int32 BestQuote(const char * subject) ;
	int32 BestQuote(const char * subjects[] , int subject_count) ;

	/*
		2011-05-11
		���±��ۣ�ֻ�������һ����ۺ�����
	*/
	int32 LatestQuote(const char * subject) ;
	int32 LatestQuote(const char * subjects[] , int subject_count) ;

	/*
		2011-05-11
		������־����
	*/
	void SetLogLevel(int level) ;

	/*
		2011-08-30
		���ÿͻ����¼��ص�����
	*/
	void SetClientEvent(DispatcherClientEvent * client_event) ;

	/*
		2011-10-10
		���ô���
	*/
	void SetProxyInfo(const ProxyInfo& info) ;
	/*
		���ݷ��ͽӿ�
	*/
	
	int SendDispatcherMsg(DispatcherMessage *& p);
	static DispatcherClient * Create() ;
	void Destory();
} ;

#endif /** __DISPATCHERCLIENT_H */
