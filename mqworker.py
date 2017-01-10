# -*- coding:utf-8 -*-  
import urllib2
import datetime
import json
import sys
import pika
from gevent import monkey; monkey.patch_all()
from xml.etree import ElementTree
from gevent.pool import Pool 
from httpworker import normal_method
from multiprocessing import Process

monkey.patch_all()


def connect_mq(queue,username='',pwd='',ip='localhost',port=5672):
    auth = pika.PlainCredentials(username, pwd,port)
    parameters = pika.ConnectionParameters(ip, port, '/');
    try:
        #host_url = '192.168.1.23'
        connection = pika.BlockingConnection(parameters)  
        channel = connection.channel()  
        channel.queue_declare(queue='my_queue', durable=True)
    except Exception, e:
        print("error", ':',e)
        return None
    return channel

    
count = 0
#g = pool.Pool()

#call_http_method('http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky')

#read file 
ini_url = 'http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3'
def compos_url(jsondata):
    try:
	json_data = json.loads(jsondata)
	print json_data
	new_url = ini_url + '&u=' + json_data['u'] + '&p=' + json_data['p']
	return new_url
    except Exception, e:
	    print(e)        
    
def callback(ch, method, properties, body):
    try:
	url = compos_url(body)
	normal_method(url)
	ch.basic_ack(delivery_tag=method.delivery_tag)
    except Exception, e:
	print(e)


def main():
    try:
	channel =  connect_mq('my_queue')
	#设置最多分给worker2个任务，多余的分配给其他worker
	channel.basic_qos(prefetch_count=2)
	channel.basic_consume(callback, queue = 'my_queue',
	                      no_ack=False)
	print("start recvive msg...")
	channel.start_consuming()
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
    main()
    #start_multi_process(func=main, worknum=4)


