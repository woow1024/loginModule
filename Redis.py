# -*- coding:utf-8 -*-  
import urllib2
import redis
import os
import json

class RedisDb:
    def __init__(self, 
                 host='localhost',
                 port=5676,
                 pwd='',
                 db = ''):
        self.host = host
        self.port = port
        self.pwd = pwd
        self.db = db
        
    def connect(self):
        self.pool = redis.ConnectionPool(host=self.host, 
                                         port= self.port, 
                                         db = self.db, 
                                         password =self.pwd)
        self.conn = redis.Redis(connection_pool=self.pool)
        
    def get_res_conn(self):
        return self.conn
              
    
###############################################################################################华丽的分割线#################################
    
    


