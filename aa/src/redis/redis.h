#ifndef _REDIS_WRAP_H
#define _REDIS_WRAP_H

#include <hiredis/hiredis.h>
#include <std_macro.h>

class redis
{
	typedef struct REDIS_SERVER
	{
		std::string ip;
		int port;
		int index;
	} REDIS_SERVER;

	DECLARE_VECTOR (REDIS_SERVER, SERVERVEC)
public:
    redis();
    ~redis();

	void add_server(const char* ip, int port, int index = 0);

	bool connect();
	
  	bool connect(const char* ip, int port, int index);

	bool redis_excute(const char* format, ...);
	redisReply* redis_command(const char* format, ...);
	redisReply* redis_vcommand(const char* format, va_list ap);

	bool set_string_value(const char* key, const char* value);
	const std::string get_string_value(const char* key);

private:
	SERVERVEC	_server_vec;
    redisContext* _connect;
};











#endif

