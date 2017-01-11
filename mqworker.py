# -*- coding:utf-8 -*-  
import urllib2
import json
import sys
import pika
import logging
import redis
from gevent import monkey; monkey.patch_all()
from xml.etree import ElementTree
from gevent.pool import Pool 
from httpworker import my_http
from multiprocessing import Process
from Queue import Queue

monkey.patch_all()
ini_url = 'http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3'
class my_mq():
    def __init__(self, host='localhost', port=5672, user='', pwd='', queue=None):
	self.host = host
	self.port = port
	self.user = user
	self.pwd  = pwd
	self.queue = queue
	
	
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
	    print "create redis pool err: %s" %e
	    return False
	
    
    def connect_mq(self,queue_name='my_queue', vhost='/'):
	auth = pika.PlainCredentials(self.user, self.pwd, self.port)
	parameters = pika.ConnectionParameters(host=self.host, port=self.port,virtual_host=vhost, credentials='');
	try:
	    connection = pika.BlockingConnection(parameters)  
	    self.channel = connection.channel()  
	    self.channel.queue_declare(queue=queue_name, durable=True)
	except Exception, e:
	    print("error", ':',e)
	    self.channel=None
	    return None
	#self.channel = channel
	#return channel
    
    def start_produce(self,msg,queue_name='my_queue1'):
	try:
	    json_str = json.dumps(msg)
	    self.channel.basic_publish(exchange='',
	                        routing_key='my_queue1',
		                 body=json_str,
		                 properties=pika.BasicProperties(
	                            #delivery_mode = 2
	                        ))	
	except Exception, e:
	    print e
	    #print("%s %s",__file__,e)   	    
	
    def star_consuming(self,queue_name='my_queue'):
	#channel =  self.__connect_mq(queue_name)
	if(self.channel is not None):
	    print "connect to rabbitmq success..."
	    try:
		#设置最多分给worker2个任务，多余的分配给其他worker
		self.channel.basic_qos(prefetch_count=2)
		self.channel.basic_consume(self.__callback, queue = queue_name,
		                  no_ack=False)
		print "start recvive msg..."
		self.channel.start_consuming()
	    except Exception, e:
		print(e)   
	else:
	    print "connect to rabbitmq failed..."

	
    def stop_consuming(self):
	pass
          
	    
    def __callback(self,ch, method, properties, body):
	try:
	    url = self.__compos_url(body)
	    http = my_http(self.redis_pool,queue=self.queue)
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
    




