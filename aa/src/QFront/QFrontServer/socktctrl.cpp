#include <easywork.h>
#include "net.h"
#include "logwriter.h"


int socket_setsockintopt(int s, int level, int optname, int value)

{
	int ret = setsockopt(s, level, optname, (const char*)&value, sizeof(value));
	
	if (ret != 0)
	{
		WRITE_LOG_WITH_ERROR ("tcp_server::setsockintopt failed");
	}
	 
	return ret;
}

int socket_getsockintopt(int s, int level, int optname)
{
	int value = 0;
	socklen_t len = sizeof(value);
		
	int ret = getsockopt(s, level, optname, (char*)&value, &len);
		
	if (ret != 0)
	{
		WRITE_LOG_WITH_ERROR ("tcp_server::getsockintopt failed");
		return -1;
	}
		
	return value;
}

bool socket_readall(int s, std::string& buffer)
{
	static const int buffer_size = 1024 * 4;
	char recv_buffer[buffer_size];
	
	int recv_len = ::recv(s, recv_buffer, buffer_size, 0);
	
	while (recv_len > 0)
	{
		buffer.append(recv_buffer, recv_len);

		if (recv_len < buffer_size)
		{
			break;
		}
		
		recv_len = ::recv(s, recv_buffer, buffer_size, 0);
	}

	return true;
}

bool socket_readn(int s, void* buffer, size_t buffer_len)
{
	size_t left_len = buffer_len;
	int recv_len = 0;
	
	char* pos = (char*)buffer;
	
	while (left_len > 0)
	{
		recv_len = ::recv(s, pos, left_len, 0);

		if (recv_len == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				continue;
			}
		}
		
		if (recv_len <= 0)
		{
			return false;
		}
			
		pos += recv_len;
		left_len -= recv_len;
	}
	
	return true;
}

bool socket_sendbuffer(int s, const void* buffer, size_t buffer_len)
{
	if(s == INVALID_SOCKET || 0 == buffer || 0 == buffer_len)
	{
		return true;
	}
	size_t left_len = buffer_len;
	int send_len = 0;
	
	const char* pos = (const char*)buffer;
	
	while (left_len > 0)
	{
		send_len = ::send(s, pos, left_len, 0);

		if (send_len == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				continue;
			}
		}
	
		if (send_len <= 0)
		{
			return false;
		}
	
		pos += send_len;
		left_len -= send_len;
	}
	
	return true;
}

bool socket_sendstring(int s, const char* buffer)
{
	return socket_sendbuffer(s, buffer, strlen(buffer));
}

static std::string get_sock_info(int s, bool get_peerinfo)
{
	if (s == INVALID_SOCKET)
	{
		return "invalid:invalid";
	}

	sockaddr_in sd;
	SIMPLE_ZEROMEMORY (sd);
	
	socklen_t len = sizeof(sd);

	if (get_peerinfo)
	{
		::getpeername(s, SA_PTR(sd), &len);
	}
	else
	{
		::getsockname(s, SA_PTR(sd), &len);
	}
	
	char buffer[256], addr[128];
	inet_ntop(AF_INET, &sd.sin_addr, addr, sizeof(addr));

	snprintf(buffer, sizeof(buffer), "%s:%d", addr, ntohs(sd.sin_port));
	return buffer;
}

std::string socket_getsockname(int s)
{
	return get_sock_info(s, false);
}

std::string socket_getpeername(int s)
{
	return get_sock_info(s, true);
}

bool socket_setsockblockmode(int s, bool noblock)
{
	int opts = fcntl(s, F_GETFL);

	if (opts < 0)
    {
		return false;
	}

	if (noblock)
    {
		opts = opts | O_NONBLOCK;
	}
	else
	{
		opts &= (~O_NONBLOCK);
	}
		 
    if (fcntl(s, F_SETFL, opts) < 0)
    {
		return false;			
    }

	return true;
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifndef _WIN32
#include <time.h>

unsigned int GetTickCount()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}


unsigned int GetCurrentTime()
{
	time_t tmCur = time(0);
	return tmCur;
}

#endif