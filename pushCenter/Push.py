# -*- coding: utf-8 -*-
from asyntask import *
from plugins.rabbitmq_consumer import *
import logging
import pika  
from plugins.Redis import RedisDb
from plugins.push_getui import PushToAndroid
from plugins.push_apns import pushAPNs
import json
import time
from  config import * 
import logging.config
from celery import Celery

logging.config.fileConfig('logging.conf')
logger=logging.getLogger('Push')
if config['DEBUG'] is True:
	logger.setLevel(logging.INFO)
else:
	logger.setLevel(logging.WARN)


redisHandler= redis.connect()

	 

#can this place use gevent???
def get_user_info(data):
	json_data = json.loads(data)
	user = json_data['username']
	res = redis.user_exists(user,'plat')
	if res != False:
		device = redis.get_value(user,'device')
		platform = redis.get_value(user,'plat')
		return user,device, platform,json_data['text']
	
pushGeTui = PushToAndroid()
pushApns = pushAPNs(environment=config['PUSH_DEBUG'])
def handle_mq(body):
	user,device, platform,text = get_user_info(body)
	if platform == "android":
		#pushGeTui.worker(device,text)
		asynfunc.delay(device, text)
	elif platform == "ios":
		asynApple.delay(device, text)
	
	

if __name__ == "__main__":
	i
	consumer = RabbitMqConsumer(config['AMQP__URL'], queue=config['AMQP_QUEUE'])
	try:
		consumer.run(handle_mq)
	except KeyboardInterrupt:
		consumer.stop();
	
        
   
    #pushMessageToList()
	
	
	
	
    
