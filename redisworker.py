import urllib2
import redis
import logging
import os
import json

att_dict =['func1']


class redisDb:
    def __init__(self, pool):
        #self.host = '192.168.1.23'
        #self.port = 6379
        self.redis_conn = redis.Redis(connection_pool=pool)
    def get_redis_conn(self):
        return self.redis_conn
    
    def user_exists(self,keyname):
        if self.redis_conn is None:
            logging.error("connect to redis failed : %s", str(error))
            return False
        try:
            len = self.redis_conn.hlen(keyname)
            if len == 0:
                return 'not exists'
            else:
                return 'exists'
        except Exception,error:
            logging.error("connect to redis failed : %s", str(error))
            return False       
        
    def write_redis(self, keyname,jsonAttr,jsonData):
        res = self.user_exists
        if not res:
            return False
        elif res == 'not exists':
            self.redis_conn.hset(keyname,jsonAttr, jsonData)
            return 'success'
        elif res == 'exits':
            return 'kickout'
       
        
    
    
    def delete_redis(self,keyname):
        
        if self.redis_conn is None:
            logging.error("connect to redis failed : %s", str(error))
            return False    
        for i in att_dict:
            self.redis_conn.hdel(keyname,i)
        h_data = self.redis_conn.hgetall(keyname)
        #print h_data        
                
    
        


if __name__ == "__main__":
    #call_http_method('http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky')

    db = redisDb()
    db.user_exists('001')
   