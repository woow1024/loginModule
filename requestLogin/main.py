# -*- coding:utf-8 -*-  

from RabbitMq import RabbitMQ
from Redis import RedisDb
from Http import Http
from gevent import monkey

from public import redis
from public import producer
from public import consumer
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

        
if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s : %(message)s',
                        datefmt='%Y-%m-%d %H:%M:%S',
                        stream=sys.stdout,
                        )

  
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
    
    try:    
        #producer.connect_mq()
        consumer.connect_mq()
    except Exception,e:
        logging.error("mq connect error %s" %e)
    
    consumer.start_Consumer(exchange='FSExchange1', 
                            queue='FSCenterBus123', 
                            callback=mq_loop,
                            routingKey='answer')
    
    
