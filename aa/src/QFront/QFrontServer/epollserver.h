#include <easywork.h>
#include <sys/epoll.h>
#include "safethread.h"
#include "safequeue.h"


struct net_event_callback;

enum SOCK_TYPE
{
	ST_NONE = -1,
	ST_LISTEN 		= 0,
	ST_REMOTECLIENT,
	ST_LOCALCLIENT
};

enum EMServerType
{
	em_authorWeb = 100,	 //ÊÚÈ¨web ·þÎñ
};

struct epoll_event_data
{
	int 			fd;
	SOCK_TYPE 		type;
	int 			identify;
	int 			id;
	net_event_callback* callback_event;
	unsigned int	tmStart;
	void* data_ex;
	epoll_event_data()
	{
		memset(this, 0, sizeof(epoll_event_data));
		fd = -1;
		type = ST_NONE;

	}
};

#define ES_CLOSE_SOCK			(-1)
#define ES_OK					(0)
#define ES_NOT_INTEREST			(1)

typedef int _net_event_on_data_can_read(int s);
typedef int _net_event_on_data_recv(int s, char* data, size_t len, unsigned int dwAppendData);
typedef int _net_event_on_connnected(int s);
typedef void _net_event_on_disconnnected(int s);
typedef int _net_event_on_data_send(int s, const char* data, size_t len);
typedef void _net_event_on_epolldata_create(epoll_event_data* data);
typedef void _net_event_on_epolldata_destroy(epoll_event_data* data);
	
struct net_event_callback
{
	_net_event_on_data_can_read		*on_data_can_read;
	_net_event_on_data_recv			*on_data_recv;
	_net_event_on_connnected 		*on_connnected;
	_net_event_on_disconnnected 	*on_disconnnected; 
	_net_event_on_data_send 		*on_data_send;
	_net_event_on_epolldata_create  *on_epolldata_create;
	_net_event_on_epolldata_destroy *on_epolldata_destroy;
};


class epollserver : public safe_thread_base<epollserver, 2>
{
	DECLARE_VECTOR (int, CLIENTVEC)
	DECLARE_MAP (int ,epoll_event_data*, EPOLLDATAMAP)
public:
	epollserver(bool is_etmode = true);
	~epollserver();

	static CSafeQueue<int> m_qLogout;
	bool send_data_to_peer(int s, const char* data, size_t len);

	bool listen_at(const char* ip, int port, net_event_callback* event, int server_identify = -1);		// server_identify must be negative
	
	bool run();
	void stop();
	bool CloseSocketByID(int nSocketID, int nReaseon);
	int GetSocketById(int nSocketID); 

	epoll_event_data* GetSocketByIdEx(int nSocketID); 
	bool GetSocketByIdEx2(int nSocketID, epoll_event_data& tEventData); 
	void GetTimeoutSocket(unsigned int nTimoutMillonSeconds, std::vector<int>& vecSocketIDArr);
private:
	void working_thread(void* param);
	void listening_thread(void* param); 
	 
	void CloseAllSocket();
	void CloseOneSocket(int s, int nReaseon);
private:
	int					_epoll_handle;
	int					_epoll_listen_handle;
	int 				_slisten;
	bool				_is_running;
	//CLIENTVEC 			_client_vec;
	bool 				_is_etmode;

	pthread_mutex_t		_client_vec_lock;
	EPOLLDATAMAP		_epolldata_set; 

};

