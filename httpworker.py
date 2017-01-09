from gevent import monkey; monkey.patch_all()
import gevent
import urllib2
import urllib
import datetime
import json
import time
from xml.etree import ElementTree
from gevent.pool import Pool
from gevent import pool
#login_url = 'http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky/"

count = 0
start_time = 0
def decode_login_response(response):
    try:
	#decode xml
	root= ElementTree.fromstring(response)
	code = root.find('code')
	#print(code.text)
    
	#decode json
	msg_json = root.find('msg').text
	result = json.loads(msg_json)
	
	print(result)    
    except Exception, e:
	print(e)


def send_login_request(url):
    try:
	value = {}
	data = urllib.urlencode(value)
	req=urllib2.Request(url)
	resp = urllib2.urlopen(req)
	res_data = resp.read()
	print(res_data)
	decode_login_response(res_data)
    except Exception, e:
	print("send login request error[%s]"%e)
    finally:
	    global count
	    global start_time
	    if 0 == count:
		start_time = time.time()
	    count += 1
	    print(count)
	    if 1000 == count:
		print(time.time() - start_time)	    
	    
	    
		
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
    begin = datetime.datetime.now()
    main()
    end = datetime.datetime.now()
    print(end-begin)

