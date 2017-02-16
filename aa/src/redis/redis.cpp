#include "redis.h"
#include "../logctrl/logwriter.h"

redis::redis()
{
	_connect = NULL;
}

redis::~redis()
 {
 	if (_connect)
 	{
		redisFree(_connect);
		_connect = NULL;
 	}
}

void redis::add_server(const char* ip, int port, int index)
{
	REDIS_SERVER server;
	server.ip = ip;
	server.port = port;
	server.index = index;

	_server_vec.push_back(server);
}

bool redis::connect(const char* ip, int port, int index)
{
	_connect = redisConnect(ip, port);

  	if(_connect == NULL)
  	{
		WRITE_FORMAT_LOG ("redisConnect [%s:%d] failed, return value is null", ip, port);
  	}
	else if (_connect->err) 
	{
		WRITE_FORMAT_LOG ("redis connect to [%s:%d] failed : %s", ip, port, _connect->errstr);
		redisFree(_connect);
		_connect = NULL;
	}
	else
	{
		WRITE_FORMAT_LOG ("redis connect to [%s:%d] succeed", ip, port);
		
		if (index != 0 && !redis_excute("select %d", index))
		{
			WRITE_FORMAT_LOG ("can't execut command [select %d] at redis server[%s:%d]", index, ip, port);
		}
	}
	
	return _connect != NULL;
}

bool redis::connect()
{
	if (_connect != NULL)
	{
		redisFree(_connect);
		_connect = NULL;
	}
	
	if (_server_vec.empty())
	{
		WRITE_LOG ("redis connect failed, no server is asigned");
		return false;
	}

	for (SERVERVECITR itr = _server_vec.begin(); itr != _server_vec.end(); ++itr)
	{
		if (connect(itr->ip.c_str(), itr->port, itr->index))
		{
			return true;
		}		
	}

	return false;
}


bool redis::redis_excute(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	
	redisReply* reply = (redisReply*)redisvCommand(_connect, format, ap);

	va_end(ap);

	bool ret_value = (reply != NULL);
	
	freeReplyObject(reply);
	return ret_value;
}

redisReply* redis::redis_vcommand(const char* format, va_list ap)
{
	if (_connect == NULL && !connect ())
	{
		return false;
	}

	va_list ap_bak;
	va_copy(ap_bak, ap);
	
	redisReply* reply = (redisReply*)redisvCommand(_connect, format, ap);
	
	if (reply == NULL && connect())
	{
		reply = (redisReply*)redisCommand(_connect, format, ap_bak);		// once again
	}

	return reply;
}

redisReply* redis::redis_command(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	redisReply* reply = redis_vcommand(format, ap);
	va_end(ap);
	return reply;
}
	
bool redis::set_string_value(const char* key, const char* value)
{
	return redis_excute("SET %s %s", key, value);
}

const std::string redis::get_string_value(const char* key)
{
    redisReply * reply = redis_command("GET %s", key);

	if (reply == NULL)
	{
		static std::string empty_str;
		return empty_str;
	}
	
	std::string str;

	if (reply->str)
	{
		str = reply->str;
	}

	freeReplyObject(reply);

	return str;
}


