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
from httpworker import setargs
monkey.patch_all()
from gevent.hub import get_hub, iwait, wait
from gevent.hub import get_hub, iwat, wait
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



def worker(body):
    print(body)
    
count = 0
#g = pool.Pool()

def callback(ch, method, properties, body):
    print("receive %r" %(body,))
    #print("recv count", )
    global count
    count += 1
    try:
	print(count)
	#tasks = [gevent.spawn(worker, body)]
	#gevent.joinall(tasks) 
	setargs(url=body)
	ch.basic_ack(delivery_tag=method.delivery_tag)
    except Exception, e:
	print(e)

try:
	channel =  connect_mq()
#设置最多分给worker2个任务，多余的分配给其他worker
	channel.basic_qos(prefetch_count=2)

	channel.basic_consume(callback, queue = 'my_queue',
                      no_ack=False)
	channel.start_consuming()
	print("recv message:")
except Exception, e:
	print(e)

#
##    end = datetime.datetime.now()
    #time = end-begin
 #   print time

