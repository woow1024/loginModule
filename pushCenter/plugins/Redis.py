# -*- coding:utf-8 -*-  
import redis
import os
import logging

__all__ = ['get_redis_conn']
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
        self._conn = False
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
    

    def get_redis_conn(self):
        try:
            return self._conn.ping()
        except Exception, e:
            self.logger.info('redis disconnected')
            return False
    
              
    

