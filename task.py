from celery import Celery
import os

app = Celery('task', backend='amqp', 
             broker='amqp://guest@localhost//')

@app.task
def start():
    os.system('python main.py')