# -*- coding:utf-8 -*-  

from RabbitMq import RabbitMQ
from Redis import RedisDb
from Http import Http
from gevent import monkey

from public import redis
from public import producer
from public import consumer
import logging

monkey.patch_all()
    
def mq_loop(ch, method, properties, body):
    try:
        http = Http()
        http.normal_method(body)
        ch.basic_ack(delivery_tag=method.delivery_tag)
    except Exception, e:
        print(e)    


if __name__ == '__main__':
    try:
        redis.connect()
    except Exception, e:
        logging.Logger.info(e)
        print "connet redis " %e
    
    res = producer.connect_mq(queue='my_queue1')
    if res is None:
        print "process error for mq connection"
        exit() 
    consumer.connect_mq(queue='my_queue')
    consumer.start_Consumer(mq_loop)
    
    
