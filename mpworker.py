# -*- coding:utf-8 -*-  
import gevent
import urllib2
import urllib
import datetime
import json
import sys
import urllib2
import pika
from gevent import monkey; monkey.patch_all()
from xml.etree import ElementTree
from gevent.pool import Pool
import multiprocessing  
from httpworker import pool_method
from httpworker import normal_method
from multiprocessing import Process

monkey.patch_all()

def connect_mq():
    auth = pika.PlainCredentials('admin', '000000')
    parameters = pika.ConnectionParameters('192.168.94.230', 5672, '/',auth);
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

def callback(ch, method, properties, body):
    print("receive %r" %(body,))
    try:
	normal_method(url=body)
	ch.basic_ack(delivery_tag=method.delivery_tag)
    except Exception, e:
	print(e)
    finally:
	global count
	count += 1
	#print(count)


def main():
    try:
	channel =  connect_mq()
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


