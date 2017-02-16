#ifndef NET_H
#define NET_H

#include <mem_macro.h>

#define SOCKET_ERROR			(-1)
#define INVALID_SOCKET			(-1)
#define INVALID_FILENO			(-1)

#define SA_CPTR(addr) ((const sockaddr *)&(addr))
#define SA_PTR(addr) ((sockaddr *)&(addr))

#define INIT_SOCKET_ADDR(addr, ip, port)\
SIMPLE_ZEROMEMORY(addr);\
addr.sin_family = AF_INET;\
addr.sin_port = htons(port);\
inet_pton(AF_INET, ip, &addr.sin_addr)

#define CLOSE_SOCKET(s)\
	if (s != SOCKET_ERROR) {\
		::close(s);\
		s = SOCKET_ERROR; }

#define CLOSE_FILENO(fileno)\
	if (fileno != INVALID_FILENO) {\
		::close(fileno);\
		fileno = INVALID_FILENO; }

#define BIND(s, sd)				\
	if (::bind(s, SA_PTR(sd), sizeof(sd)) == SOCKET_ERROR)	{	\
		WRITE_ERROR_LOG (errno, "bind failed");\
		exit(0);}	


#define LISTEN(s, sd)				\
	if (::listen(s, 5) == SOCKET_ERROR)	{	\
		WRITE_ERROR_LOG(errno, "listen failed");\
		exit(0);}	



int socket_setsockintopt(int s, int level, int optname, int value);
int socket_getsockintopt(int s, int level, int optname); 
bool socket_setsockblockmode(int s, bool noblock);

bool socket_readn(int s, void* buffer, size_t buffer_len);
bool socket_readall(int s, std::string& buffer);
bool socket_sendbuffer(int s, const void* buffer, size_t buffer_len);
bool socket_sendstring(int s, const char* buffer);

std::string socket_getsockname(int s);
std::string socket_getpeername(int s);

#ifndef _WIN32
unsigned int GetTickCount();
unsigned int GetCurrentTime();
#endif

#endif
