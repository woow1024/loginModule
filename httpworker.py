from gevent import monkey; monkey.patch_all()
import gevent
import urllib2
import urllib
import datetime
import json
import time
from  redisworker import  redisDb
from xml.etree import ElementTree
from gevent.pool import Pool
from gevent import pool
#login_url = 'http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky/'


def decode_login_response(response):
    try:
	#decode xml
	root= ElementTree.fromstring(response)
	
	#decode json
	msg_json = root.find('msg').text
	result = json.loads(msg_json)
	
	if( result['ret'] != 0 ):
	    return  False
	
	b = redisDb()
	print result['user']['username']
	b.write_redis( result['user']['username'],'func',result['r'][0]['func'])
    except Exception, e:
	print(e)


def send_login_request(url):
  
	value = {}
	data = urllib.urlencode(value)
	req=urllib2.Request(url)
	try:
	    resp = urllib2.urlopen(req)
	except Exception, e:
	    print("send login request error[%s]"%e)	
	    return False
	res_data = resp.read()
	print(res_data)
	decode_login_response(res_data)
   
	    
	    
		
def pool_method(url):
    try:
	g = pool.Pool()
	g.spawn(send_login_request, url)
	#time.sleep(1)
    except Exception,e:
	print(e)	
    return None


def normal_method(url):
    try:
	gevent.spawn(send_login_request, url)
	#time.sleep(1)
    except Exception,e:
	print(e)	
    return None   
    
    
def main():
    for i in range(1000):
	pool_method('http://httpbin.org/get')
    return None

if __name__ == '__main__': 
   # begin = datetime.datetime.now()
    #main()
   # end = datetime.datetime.now()
   # print(end-begin)
    send_login_request('http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky/')

