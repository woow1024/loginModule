# -*- coding: utf-8 -*-
from plugins.push_getui import PushToAndroid
from plugins.push_apns import pushAPNs
from celery import Celery
from config import config


app = Celery(config['NAME'],  broker=config['CELERY_BROKER_URL']) #配置好celery的backend和broker
app.conf.update(config)

pushGeTui = PushToAndroid()
pushApns = pushAPNs(environment=config['PUSH_DEBUG'])

@app.task  #普通函数装饰为 celery task
def asynfunc(device, text):
	pushGeTui.worker(device,text)
		
@app.task 
def asynApple(device, text):
	pushApns.push(device,text)