import urllib2
import redis
import logging
import os
import json

__all__ = {'call_http_method'}
def call_http_method(url, data=None, http_header={}):
    try:
        request = urllib2.Request(url, data, http_header)
        http_response = urllib2.urlopen(request)
    except (urllib2.URLError, ValueError, TypeError), error:
        logging.error("call http method error\nurl : %s\ndata : %r\nerror reason:%s",
                      url, data, error)
        return str(error), False

    try:
        content = http_response.read()
        code = http_response.getcode()

        http_response.close()
        print content
        return content, code

    except Exception, error:
        logging.error("call http method error\nurl : %s\ndata : %r\nerror reason:%s",
                      url, data, error)
        return str(error), False
    
    
class redisDb:
    def __init__(self):
        self.host = '127.0.0.1'
        self.port = 6379
        
    def get_redis_conn(self):
        try:
            redis_pool = redis.ConnectionPool(host=self.host,port=self.port)
            redis_conn = redis.Redis(connection_pool=redis_pool)      
            return redis_conn
        except Exception, error:
            logging.error("connect to redis failed : %s", str(error))
            return None        
        
    def user_exists(self,keyname):
        conn = redisDb.get_redis_conn(self)
        if conn is None:
            logging.error("connect to redis failed : %s", str(error))
            return False
        try:
            len = conn.hlen(keyname)
            if len == 0:
                return 0
            else:
                return 1
        except Exception,error:
            ogging.error("connect to redis failed : %s", str(error))
            return False       
        
    def write_redis(self, jsonData):
        conn = redisDb.get_redis_conn(self)
        if conn is None:
            logging.error("connect to redis failed : %s", str(error))
            return False    
        data = json.loads(jsonData, encoding=None, cls=None, object_hook=None, 
                         parse_float=None, 
                         parse_int=None, 
                         parse_constant=None, 
                         object_pairs_hook=None)
        
        
        


if __name__ == "__main__":
    #call_http_method('http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky')

    db = redisDb()
    db.user_exists('001')