# -*- coding: utf-8 -*-

from celery import Celery
from config import config
from plugins.rabbitmq_consumer import *
import logging
import pika  
from plugins.Redis import RedisDb
from plugins.push_to_app import PushToAndroid
import gevent
import gevent.monkey
import json
gevent.monkey.patch_all()

content = {'title':u'早上好', 'text':u'天气不错'}
LOG_FORMAT = ('%(levelname) -10s %(asctime)s %(name) -30s %(funcName) '
              '-35s %(lineno) -5d: %(message)s')
LOGGER = logging.getLogger(__name__)

def make_celery(broker):
    try:
        celery = Celery(config['NAME'], broker=config['CELERY_BROKER_URL'])
        celery.conf.update(config)
    except Exception,e:
        print "celery" + e.message;


pushObject = PushToAndroid()
def handle_mq(body):
    pushObject.worker(clientId = config['CID'],title=content['title'],text=content['text'])  



if __name__ == "__main__":
    #make_celery(CELERY_BROKER_URL); 
    logging.basicConfig(level=logging.INFO, format=LOG_FORMAT)
    
    redis = RedisDb(host=config['REDIS_URL'])
    redisHandler = redis.connect()
    redis.get_redis_conn()
    aa =redisHandler.get('name')

    consumer = RabbitMqConsumer(config['AMQP__URL'], queue=config['AMQP_QUEUE'])
    try:
        consumer.run(handle_mq)
    except KeyboardInterrupt:
        consumer.stop();
        
   
    #pushMessageToList()
    