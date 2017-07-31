# -*- coding: utf-8 -*-
from asyntask import asynfunc
from asyntask import pushObject
from celery import Celery
from config import config
from plugins.rabbitmq_consumer import *
import logging
import pika  
from plugins.Redis import RedisDb
from plugins.push_to_Android import PushToAndroid
import gevent
import gevent.monkey
import json
import time
from  config import *
gevent.monkey.patch_all()


LOG_FORMAT = ('%(levelname) -10s %(asctime)s %(name) -30s %(funcName) '
              '-35s %(lineno) -5d: %(message)s')
LOGGER = logging.getLogger(__name__)

pushObj = PushToAndroid()
	
def handle_mq(body):
	jstr = json.loads(body)
	pushObj.worker(config['CID'],jstr['title'],jstr['text'])
	#result = asynfunc.delay(config['CID'],content['title'],content['text'])
	
def make_celery(broker):
	try:
		celery = Celery(config['NAME'], broker=config['CELERY_BROKER_URL'])
		celery.conf.update(config)
	except Exception,e:
		print "celery" + e.message;




if __name__ == "__main__":
    #make_celery(CELERY_BROKER_URL); 
	logging.basicConfig(level=logging.INFO, format=LOG_FORMAT)
	
	redisHandler = redis.connect()
	redis.get_redis_conn()
	aa =redisHandler.get('name')
	
	consumer = RabbitMqConsumer(config['AMQP__URL'], queue=config['AMQP_QUEUE'])
	try:
		consumer.run(handle_mq)
	except KeyboardInterrupt:
		consumer.stop();
        
   
    #pushMessageToList()
    