# -*- coding:utf-8 -*-  
import redis
import os
import logging

__all__ = ['get_redis_conn','redis']



class RedisDb(object):
    def __init__(self, 
                 host='localhost',
                 port=6379,
                 pwd='',
                 db = 0):
        self.host = host
        self.port = port
        self.pwd = pwd
        self.db = db
        self._conn = None
        self.logger = logging.getLogger(self.__module__)
        
    def connect(self):
        try:
            self.pool = redis.ConnectionPool(max_connections=5,
                                         host=self.host, 
                                         port= self.port, 
                                         db = self.db, 
                                         password =self.pwd)
            self._conn = redis.Redis(connection_pool=self.pool)
            return self._conn
        except Exception, e:
            self.logger.info('Connecting to redis error %s', str(e))
            return False
    

    def get_redis_state(self):
        try:
            return self._conn.ping()
        except Exception, e:
            return False
        
    def get_redis_conn(self):
        if self.get_redis_state() == True:
            return self._conn
        else:
            self.logger.warn('redis disconnected')
            return False
        
    def user_exists(self,keyname):
        conn = self.get_redis_conn()
        try:
            len = conn.hlen(keyname)
            if len == 0:
                return 'not exists'
            else:
                return 'exists'
        except Exception,error:
            logging.error("check user_exists failed : %s", str(error))
            return False           
    

