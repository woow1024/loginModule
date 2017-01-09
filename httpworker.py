from gevent import monkey; monkey.patch_all()
import gevent
import urllib2
import urllib
import datetime
import json
from xml.etree import ElementTree
from gevent.pool import Pool
from gevent import pool
#login_url = 'http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky/"

n = 0
def f(url):
	try:
		value = {}
		data = urllib.urlencode(value)
		req=urllib2.Request(url)
		resp = urllib2.urlopen(req)
		res_data = resp.read()
		print(res_data)
		#print(resp)
		#return None
		root= ElementTree.fromstring(res_data)
		code = root.find('code')
		#print(code.text)
		msg_json = root.find('msg').text
		#print(res_data)
		result = json.loads(msg_json)
		# print('%d bytes received from %s.' % (len(res_data), url))
		print(result)
	except Exception, e:
		print(e)
	finally:
		global n
		n += 1
		print(n)		
		
    
i = 0
def setargs(url):
    #print "send http %s" %num_http
    global i
    g = pool.Pool()
    i += 1
    #if i < 100:   
    g.spawn(f, url)
    #g.add(f, url)
    #else:
    #    g.join()
    #    i = 0
    #pool = Pool(num_pool)
    #tasks = [gevent.spawn(f, 'https://www.baidu.com/') for i in xrange(1000)];
    #gevent.joinall(tasks)
    # urls =  url * num_http
    #pool.map(f, urls)    
    
if __name__ == '__main__': 
    begin = datetime.datetime.now()
    end = datetime.datetime.now()
    time = end-begin
    
    
    print time

