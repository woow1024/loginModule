from gevent import monkey; monkey.patch_all()
import gevent
import urllib2
import urllib
import json
import logging
from  redisworker import  redisDb
from xml.etree import ElementTree
from gevent.pool import Pool
from gevent import pool
import redis
from Queue import Queue
#login_url = 'http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky/'

#b = redisDb()

class my_http:
    def __init__(self, pool,queue):
        self.redisdb=redisDb(pool)
        self.queue=queue
    def decode_write_login_response(self,response):
        try:
            #decode xml
            root= ElementTree.fromstring(response)
    
            #decode json
            msg_json = root.find('msg').text
            result = json.loads(msg_json)
    
            if( result['ret'] != 0 ):
                return  False
            keyname = result['user']['username']
            res = self.redisdb.write_redis( keyname,'func',result['r'][0]['func'])
            
        except Exception, e:
            print(e)
    
    def send_login_request(self, url):
    
        value = {}
        data = urllib.urlencode(value)
        req=urllib2.Request(url)
        
        try:
            resp = urllib2.urlopen(req)
        except Exception, e:
            logging.info("send login request error[%s]"%e)	
            self.queue.put(e)
            return False
        res_data = resp.read()
        print(res_data)
        self.decode_write_login_response(res_data)
    
    def pool_method(self, url):
        try:
            g = pool.Pool()
            g.spawn(self.send_login_request, url)
            #time.sleep(1)
        except Exception,e:
            print(e)	
        return None
    
    
    def normal_method(self, url):
        try:
            gevent.spawn(self.send_login_request, url)
            
        except Exception,e:
            print(e)	
        return None   
    
    
    def test(self):
        for i in range(1000):
            pool_method('http://httpbin.org/get')
        return None    


if __name__ == '__main__': 
    send_login_request('http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky/')

