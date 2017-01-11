# -*- coding:utf-8 -*-  
import urllib2
import datetime
import json
import sys
import pika
import redis
from gevent import monkey; monkey.patch_all()
from xml.etree import ElementTree
from gevent.pool import Pool 
from httpworker import my_http
from multiprocessing import Process


monkey.patch_all()
ini_url = 'http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3'
class my_mq():
    def __init__(self, host='localhost', port=5672, user='', pwd=''):
	self.host = host
	self.port = port
	self.user = user
	self.pwd  = pwd
	
	
    def initRedisPool(self, host='127.0.0.1', port = 6379, 
                      pwd=None, db=0,
                      socket_connect_timeout=None,
                      socket_keepalive=None, 
                      socket_keepalive_options=None,
                      encoding='utf-8'):
	try:
	    self.redis_pool = redis.ConnectionPool(host=host, port=port, db=0, password=pwd)
	    return True
	except Exception, e:
	    print "create redis pool err:"+e
	    return False
	
    
    def __connect_mq(self,queue_name='my_queue', vhost='/'):
	auth = pika.PlainCredentials(self.user, self.pwd, self.port)
	parameters = pika.ConnectionParameters(host=self.host, port=self.port,virtual_host=vhost, credentials=auth);
	try:
	    connection = pika.BlockingConnection(parameters)  
	    channel = connection.channel()  
	    channel.queue_declare(queue=queue_name, durable=True)
	except Exception, e:
	    print("error", ':',e)
	    self.channel=None
	    return None
	self.channel = channel
	return channel
    
    def star_consuming(self,queue_name='my_queue'):
	channel =  self.__connect_mq(queue_name)
	if(channel is not None):
	    print "connect to rabbitmq success..."
	    try:
		#设置最多分给worker2个任务，多余的分配给其他worker
		channel.basic_qos(prefetch_count=2)
		channel.basic_consume(self.__callback, queue = 'my_queue',
		                  no_ack=False)
		print "start recvive msg..."
		channel.start_consuming()
	    except Exception, e:
		print(e)   
	else:
	    print "connect to rabbitmq failed..."

	
    def stop_consuming(self):
	pass
          
	    
    def __callback(self,ch, method, properties, body):
	try:
	    url = self.__compos_url(body)
	    http = my_http(self.redis_pool)
	    http.pool_method
	    http.normal_method(url)
	    ch.basic_ack(delivery_tag=method.delivery_tag)
	except Exception, e:
	    print(e)
	
    #call_http_method('http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky')
    def __compos_url(self,jsondata):
	try:
	    json_data = json.loads(jsondata)
	    print json_data
	    new_url = ini_url + '&u=' + json_data['u'] + '&p=' + json_data['p']
	    return new_url
	except Exception, e:
	    print(e)      
	    
#start mutlti process
def start_multi_process(func, worknum):
    proc_record = []
    for i in range(worknum):
	p = Process(target = func)
	p.start()
	proc_record.append(p)
    for p in proc_record:
	p.join()    
    


if __name__ == '__main__':  
    try:
	mq = my_mq(host='192.168.94.230', 
	           port=5672, 
	           user='admin', 
	           pwd='000000')
	
	mq.initRedisPool(host='127.0.0.1', 
	                 port=6379, 
	                 db=0, 
	                 pwd=None)
	
	print 'init redis pool success..'

	mq.star_consuming(queue_name='my_queque')
	
    except Exception, error:
	print error  
    #start_multi_process(func=main, worknum=4)


