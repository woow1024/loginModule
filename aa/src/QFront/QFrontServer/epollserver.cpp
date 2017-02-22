#include "epollserver.h"
#include "logwriter.h"
#include "net.h"
#include "autolock.h" 
#include <errno.h> 

#define MAX_RECV_BUFFER_SIZE	(1024 * 1024)	
static const int EPOLL_SIZE = 20000; 
CSafeQueue<int> epollserver::m_qLogout;
epollserver::epollserver(bool is_etmode)
{
	_slisten = INVALID_SOCKET;
	_is_running = false;
	_is_etmode = is_etmode; 
	_epoll_handle = epoll_create(EPOLL_SIZE+1);
	_epoll_listen_handle = epoll_create(2);
	INIT_LOCK(_client_vec_lock);
	//INIT_LOCKEX(_client_vec_lock, PTHREAD_MUTEX_ERRORCHECK_NP);

}

 epollserver::~epollserver()
 {
	 this->stop();
	CLOSE_SOCKET (_slisten);
	CLOSE_FILENO(_epoll_handle) 
	CLOSE_FILENO(_epoll_listen_handle) 

	CloseAllSocket(); 
	DEL_LOCK(_client_vec_lock);
 }


 void epollserver::CloseAllSocket()
 {
	 //auto_mutext_lock lock(&_client_vec_lock);
	 ENTER_LOCK(_client_vec_lock);

	 for (EPOLLDATAMAPITR itr = _epolldata_set.begin(); itr != _epolldata_set.end(); ++itr)
	 {
		 epoll_event_data* data = itr->second;

		 if (data->callback_event && data->callback_event->on_epolldata_destroy)
		 {
			 data->callback_event->on_epolldata_destroy(data);
		 }

		 CLOSE_SOCKET (data->fd);
		 DELETE_PTR(data);
	 }
 }


bool epollserver::listen_at(const char* ip, int port, net_event_callback* event, int server_identify)
{
	assert(_epoll_listen_handle != INVALID_FILENO);

	if (_epoll_listen_handle == INVALID_FILENO)
	{
		return false;
	}	

	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	socket_setsockintopt(s, SOL_SOCKET, SO_REUSEADDR, 1);
	socket_setsockblockmode(s, true);
	
	struct  sockaddr_in sd;
	SIMPLE_ZEROMEMORY (sd);
	
	sd.sin_family = AF_INET;
	sd.sin_port = htons(port);

	if (strcmp(ip, "*") == 0)
	{
		sd.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		inet_pton(AF_INET, ip, &sd.sin_addr);
	}

	if (::bind(s, SA_PTR(sd), sizeof(sd)) == SOCKET_ERROR)
	{

		WRITE_FORMAT_LOG ("bind [%s:%d] failed, error:[%d %s]", ip, port, errno, strerror(errno));
		CLOSE_SOCKET (s);
		return false;
	}
	else
	{

		WRITE_FORMAT_LOG ("bind [%s:%d] fd:%d OK", ip, port, s);
	}

	if (::listen(s, 5) == SOCKET_ERROR)
	{

		WRITE_FORMAT_LOG ("listen [%s:%d] failed, error:[%d %s]", ip, port, errno, strerror(errno));
		CLOSE_SOCKET (s);
		return false;
	}
	else
	{
		WRITE_FORMAT_LOG ("listen [%s:%d] fd:%d OK", ip, port, s);
	}

	ENTER_LOCK(_client_vec_lock);
	struct epoll_event ep_event; 

	struct epoll_event_data* data = new epoll_event_data;
	//memset(data, 0, sizeof(epoll_event_data));
	data->fd = s;
	data->type = ST_LISTEN;
	data->callback_event = event;
	data->identify = server_identify < 0 ? server_identify : -1; 
	data->id = 10;
	if (event->on_epolldata_create)
	{
		event->on_epolldata_create(data);
	}
	
	ep_event.events = EPOLLIN;

	if (_is_etmode)
	{
		ep_event.events |= EPOLLET;
	}
	
	ep_event.data.u32 =   data->id;

	_epolldata_set[data->id] = (data);
	
	if (epoll_ctl(_epoll_listen_handle, EPOLL_CTL_ADD, s, &ep_event) == -1)
	{
		FREE_PTR (data);
		WRITE_LOG_WITH_ERROR ("epoll_ctl failed, create server at [%s:%d] failed, error:[%d %s]", ip, port, errno, strerror(errno));
		return false;
	}
	//listen OK, star on thread;
	this->run();
	return true;
}

bool epollserver::run()
{
	if (_is_running)
	{
		WRITE_LOG ("server is already running");
		return true;
	}

	assert(_epoll_handle != INVALID_FILENO);

	if (_epoll_handle == INVALID_FILENO)
	{
		return false;
	}	

	_is_running = true;

	safe_start_thread(&epollserver::working_thread, NULL);
	safe_start_thread(&epollserver::listening_thread, 0, 1);
	
	return true;
}

void epollserver::listening_thread(void* param)
{
	WRITE_LOG ("epollserver::listening_thread is running now");
	struct epoll_event ep_event;
	ep_event.events = EPOLLIN | EPOLLRDHUP;

	struct epoll_event* events = new struct epoll_event[20];
	if (_is_etmode)
	{
		ep_event.events |= EPOLLET;
	}
	int nAcceptOKNum = 0;
	int nAcceptErrNum = 0;
	while (_is_running)
	{
		int nums = epoll_wait(_epoll_listen_handle, events, 20, -1/*1000*/);

		if (nums == 0)
		{
			continue; 
		}
		else if (nums == -1)
		{
			if( EINTR != errno)
			{
				WRITE_FORMAT_LOG ("listen epoll_wait failed,error:[%d %s]", errno, strerror(errno));
				break;
			}
			else
				continue;
		}

		for (int n = 0; n < nums; ++n)
		{
			int nSocketID =  events[n].data.u32;
			struct epoll_event_data* data = this->GetSocketByIdEx(nSocketID);

			if (data == 0)		// what's up??
			{
				continue;
			}

			int s = data->fd;

			if (data->type == ST_LISTEN)				// listen sock,call accept method
			{
				sockaddr_in ad;
				SIMPLE_ZEROMEMORY(ad);
				socklen_t len = sizeof(ad);

				int client = ::accept(s, SA_PTR(ad), &len);

				if (client == -1)		// maybe client disconnet right now
				{
					nAcceptErrNum ++;
					WRITE_FORMAT_LOG ("accept failed, return value is -1， error:[%d ]" , errno);
					continue;
				}
				
				if (data->callback_event && data->callback_event->on_connnected)
				{
					if (data->callback_event->on_connnected(client) == ES_CLOSE_SOCK)  // i don't like this connection
					{
						close(client);
						continue;
					}
				}
				static int nSocketID = 100;
				try
				{
					
					WRITE_FORMAT_LOG("accept begin" );
					struct epoll_event_data* client_data = new epoll_event_data;
					//memset(data, 0, sizeof(epoll_event_data));

					client_data->fd = client;
					client_data->type = ST_REMOTECLIENT;
					client_data->callback_event = data->callback_event;
					client_data->identify = data->identify; 
					client_data->id = nSocketID ++;
					client_data->tmStart = ::GetTickCount();

					if (client_data->callback_event && client_data->callback_event->on_epolldata_create)
					{
						client_data->callback_event->on_epolldata_create(data);
					}				
					ep_event.data.u32 =  client_data->id;

					{
						ENTER_LOCK(_client_vec_lock);	
						_epolldata_set[client_data->id] = client_data;
						epoll_ctl(_epoll_handle, EPOLL_CTL_ADD, client, &ep_event);
					}
					

					 
					
					nAcceptOKNum ++;
					char* pszIP=inet_ntoa(ad.sin_addr);
					WRITE_FORMAT_LOG("accept OK  [%s : %d] fd:%d id:%d OK:%d err:%d" , pszIP, ad.sin_port,
									client,client_data->id, nAcceptOKNum, nAcceptErrNum);
				} 
				catch(exception &e)
				{
					cout<<"accept error: "  << e.what()<<endl;
				}
				catch(...)
				{
					cout<<"accept error: unkown" <<endl;
				}
				 
			}
		}
	}

	DELETE_PTRA(events);
}


void epollserver::working_thread(void* param)
{
	WRITE_LOG ("epollserver::working_thread is running now");

	struct epoll_event ep_event;	
	ep_event.events = EPOLLIN | EPOLLRDHUP;

	if (_is_etmode)
	{
		ep_event.events |= EPOLLET;
	}

	struct epoll_event_data  dataStatic; 
	char * szRecvBuffer = new char[MAX_RECV_BUFFER_SIZE*2];;
	struct epoll_event * events = new epoll_event[EPOLL_SIZE+1];;
	while (_is_running)
	{
		int nums = epoll_wait(_epoll_handle, events, EPOLL_SIZE, -1/*1000*/);

		if (nums == 0)
		{
			continue; 
		}
		else if (nums == -1)
		{
			if( EINTR != errno)
			{
				WRITE_FORMAT_LOG ("epoll_wait failed,error:[%d %s]", errno, strerror(errno));
				break;
			}
			else
				continue;
		}
		for (int n = 0; n < nums; ++n)
		{

			if(events[n].events&EPOLLIN)
			{//有数据可读
				int nSocketID =   events[n].data.u32;
				bool bRet = this->GetSocketByIdEx2(nSocketID, dataStatic); 
				if(false == bRet)
				{
					continue;
				}  
				struct epoll_event_data* data = & dataStatic;
				int s = data->fd;
				// normal sock, call recv/close method
				{
					int nMaxSize = MAX_RECV_BUFFER_SIZE ;
					int len = ::recv(s, szRecvBuffer, nMaxSize, MSG_PEEK);
			
					if (len > 0)		// have data,can call recv
					{
						if (data->callback_event && data->callback_event->on_data_can_read)
						{
							int call_ret = data->callback_event->on_data_can_read(s);

							if (call_ret == ES_OK)
							{
								continue;
							}
							else if (call_ret == ES_CLOSE_SOCK)
							{ 
								CloseOneSocket(s, 1);
								//close_sock_by_epolldata(s, data);
								continue;
							}
						}
						
							
						if (data->callback_event && data->callback_event->on_data_recv)
						{

							len = ::recv(s, szRecvBuffer, nMaxSize, 0);
							if(len <= 0)
							{
								CloseSocketByID(nSocketID, 1);
								continue;
							}
							szRecvBuffer[len] = '\0';
							int call_ret = data->callback_event->on_data_recv(s, szRecvBuffer, len,data->id);

							if (call_ret == ES_CLOSE_SOCK)
							{
								CloseOneSocket(s, 2);
								//close_sock_by_epolldata(s, data);
							}
						}
					}
					else if(-1 == len)		// recv error, close and callback memory
					{
						//WRITE_ERROR_LOG(errno, "recv error 3  fd:%d" , s);
						CloseSocketByID(s,3);
						//close_sock_by_epolldata(s, data);
					}

				}
			}
			else if(events[n].events&EPOLLOUT) //如果有数据发送
			{//
#if 0
				events[n].events = EPOLLIN | EPOLLRDHUP;

				if (_is_etmode)
				{
					events[n].events |= EPOLLET;
				}

				WRITE_FORMAT_LOG ("epoll_wait [%d] ready write", n);
#endif
			}

		}
	}
	
	_is_running = false;

	DELETE_PTRA(events);
	DELETE_PTRA(szRecvBuffer);
	WRITE_LOG ("epollserver::working_thread quit");
}
 

static int nErrSocketNum = 0;
void epollserver::CloseOneSocket(int s, int nReaseon)
{
	nErrSocketNum ++;
	WRITE_LOG_WITH_ERROR("socket[%d] reason:%d errronum:%d" , s, nReaseon, nErrSocketNum); 
	ENTER_LOCK(_client_vec_lock);
	struct epoll_event ep_event;	
	epoll_ctl(_epoll_handle, EPOLL_CTL_DEL, s, &ep_event);
	close(s);
}

int epollserver::GetSocketById(int nSocketID)
{
	ENTER_LOCK(_client_vec_lock); 
	EPOLLDATAMAPITR iter = _epolldata_set.find(nSocketID);
	if( _epolldata_set.end() != iter )
	{
		return iter->second->fd;
	}
	return INVALID_SOCKET;
}

void epollserver::GetTimeoutSocket(unsigned int nTimoutMillonSeconds, std::vector<int>& vecSocketIDArr)
{
	int nTimeNow = ::GetTickCount();
	ENTER_LOCK(_client_vec_lock); 
	for (EPOLLDATAMAPITR itr = _epolldata_set.begin(); itr != _epolldata_set.end(); ++itr)
	{
		epoll_event_data* data = itr->second;
		if(nTimeNow - data->tmStart >= nTimoutMillonSeconds)
		{
			vecSocketIDArr.push_back(data->id);
		}
	}
}

epoll_event_data* epollserver::GetSocketByIdEx(int nSocketID)
{
	ENTER_LOCK(_client_vec_lock); 
	EPOLLDATAMAPITR iter = _epolldata_set.find(nSocketID);

	if( _epolldata_set.end() != iter )
	{
		return iter->second;
	}
	return 0;
}


bool epollserver::GetSocketByIdEx2(int nSocketID, epoll_event_data& tEventData)
{
	ENTER_LOCK(_client_vec_lock); 
	EPOLLDATAMAPITR iter = _epolldata_set.find(nSocketID);

	if( _epolldata_set.end() != iter )
	{
		tEventData = * iter->second;
		return true;
	}
	return false;

}

bool epollserver::CloseSocketByID(int nSocketID, int nReaseon)
{ 
	static int nCloseSocketNum = 0;
	ENTER_LOCK(_client_vec_lock); 
	nCloseSocketNum++;
	WRITE_FORMAT_LOG("close socket id:%d  reason:%d total:%d" , nSocketID, nReaseon, nCloseSocketNum);
	m_qLogout.push(nSocketID);

	EPOLLDATAMAPITR iter = _epolldata_set.find(nSocketID);

	if( _epolldata_set.end() != iter )
	{ 
		try
		{
			epoll_event_data* data = iter->second;
			int fd = data->fd;

			WRITE_FORMAT_LOG("close socket id:%d [%d] reason:%d begin" ,nSocketID, fd, nReaseon);
			if (data->callback_event && data->callback_event->on_disconnnected)
			{
				data->callback_event->on_disconnnected(fd); 
			}

           		
			struct epoll_event ep_event;	
			epoll_ctl(_epoll_handle, EPOLL_CTL_DEL, fd, &ep_event);

			if (data->callback_event && data->callback_event->on_epolldata_destroy)
			{
				data->callback_event->on_epolldata_destroy(data);
			}
			_epolldata_set.erase(iter);
			DELETE_PTR(data);
			close(fd);
			
			WRITE_FORMAT_LOG("close socket id:%d [%d] reason:%d end" ,nSocketID,  fd, nReaseon);
		} 
		catch(exception &e)
		{
			cout<<"close_sock_by_epolldata error: "  << e.what()<<endl;
		}
		catch(...)
		{
			cout<<"close_sock_by_epolldata error: unkown" <<endl;
		}
		return true;
	}

	//WRITE_FORMAT_LOG("close socket id[%d] fail,can not find" , nSocketID);
	return false;
 
}
 

void epollserver::stop()
{
	_is_running = false;
	join_all_thread();
}

bool epollserver::send_data_to_peer(int s, const char* data, size_t len)
{
	CLIENTVEC client_vec; 

	ENTER_LOCK(_client_vec_lock); 
	if (s < 0)		// broadcast, use indentify 
	{

		for (EPOLLDATAMAPITR itr = _epolldata_set.begin(); itr != _epolldata_set.end(); ++itr)
		{
			epoll_event_data* data = itr->second;
			
			if(data && data->type == ST_REMOTECLIENT && data->identify == s)
			{
				client_vec.push_back(data->fd);
			}
		}
	}
	else
	{
		client_vec.push_back(s);
	}

	bool send_ret = true;
	
	for (CLIENTVECITR itr = client_vec.begin(); itr != client_vec.end(); ++itr)
	{
		send_ret &= socket_sendbuffer(*itr, data, len);
	}

	return send_ret;
}


