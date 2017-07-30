# -*- coding: utf-8 -*-
from plugins.push_to_app import PushToAndroid
from celery import Celery

pushObject = PushToAndroid() 
app = Celery('asyntask',  backend='redis://localhost:6379/0', broker='redis://localhost:6379/0') #配置好celery的backend和broker

@app.task  #普通函数装饰为 celery task
def asynfunc(clientID, title, text):
	print clientID
	pushObject.worker(clientID,title,text)