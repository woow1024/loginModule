# -*- coding:utf-8 -*-  

from RabbitConsumer import RabbitConsumer
from Redis import RedisDb
from Http import Http
from gevent import monkey

from public import redis
from public import producer
from public import consumer
from public import delete_redis
import os
import sys
import logging
import json
import threading
import time
monkey.patch_all()

MQ_VHOST = 'test'
WORKING_DIR = os.path.dirname(sys.argv[0])
WORKING_DIR = os.path.dirname(WORKING_DIR)
LOG_DIR = os.path.join(WORKING_DIR, "log")
CONF_DIR = os.path.join(WORKING_DIR, "conf")
LOG_FILENAME = os.path.join(LOG_DIR, "LoginModule.log")

def mq_loop(ch, method, properties, body):
    print " [x] %r:%r" % (method.routing_key, body,) 
    key = method.routing_key
    if('logout' == key):
        print body
        delete_redis(body)
        ch.basic_ack(delivery_tag=method.delivery_tag)
    
    if('answer' == key):
        try:
            http = Http()
            http.normal_method(body)
            ch.basic_ack(delivery_tag=method.delivery_tag)
        except Exception, e:
            print(e)    


def read_conf(path):
    try:
        fileObj = open(path,'r')
    except Exception,e:
        raise e
    try:
        readObj = fileObj.read()
    except Exception,e:
        fileObj.close()
    
    fileObj.close()
    return readObj


def func_producer():
    global aa
    producer.set_args('FSExchange1', 
                          'direct', 
                          'FSReplay', 
                          'answer',
                          on_msg_callcack=None)            
    try:  
        producer.run()
    except Exception, e:
        raise e

def func_consumer():
    serverLists = ['answer','logout'] 
    while True: 
        try:    
            consumer.connect()
            consumer.start_Consumer(exchange='FSExchange1', 
                                    queue='FSCenterBus123', 
                                    callback=mq_loop,
                                    serverLists=serverLists)            
        except Exception,e:
            logging.error("mq connect error %s" %e)
            logging.info('lose connection, reopen in 3 seconds...')
            time.sleep(5) 
            continue
        else:
            break
        
if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s : %(message)s',
                        datefmt='%Y-%m-%d %H:%M:%S',
                        stream=sys.stdout,
                        )

  
    if not os.path.exists(LOG_DIR):
            os.makedirs(LOG_DIR)
  
    console = logging.FileHandler(LOG_FILENAME, "a")
    
    console.setLevel(logging.INFO)
    formatter = logging.Formatter(fmt='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s : %(message)s',
                                      datefmt='%Y-%m-%d %H:%M:%S')
    console.setFormatter(formatter)
    logging.getLogger().addHandler(console)  

    logging.info("LoginModule is runing now, process id %d", os.getpid())
    
    
    try:
        redis.connect()
    except Exception, e:
        logging.info("connet redis %s " %e)
        print "make sure server is started"
       
    t1 = threading.Thread(target = func_producer)
    t2 = threading.Thread(target = func_consumer)
    
    t1.start()
    t2.start()
    
    t1.join()
    t2.join()
   
   
    
    
        
        
    
    
