import pika  
import sys
import urllib2
import json

def connect_mq(host_url):
    auth = pika.PlainCredentials('cc', '123')
    parameters = pika.ConnectionParameters(host_url, 5672, 'test',auth);
    try:
        connection = pika.BlockingConnection(parameters)  
        channel = connection.channel()  
       # channel.exchange_declare(exchange='FSexchange',type='direct')
    except Exception, e:
        print("error",e)
        return None
    return channel


#channel =  connect_mq('192.168.44.223')
channel =  connect_mq('127.0.0.1')
try:
        #message=("%d helloworld"%count)
        message={'u':'MTc3MDM3MTQ5MDE=','p':'MTc3MDM3MTQ5MDE='}
        json_str = json.dumps(message)
        channel.basic_publish(exchange='FSExchange1',
                      routing_key='FSReplay',
                      body=json_str
                      )
        print("done")
        #time.sleep(1)
except Exception, e:
    print(e)

    
    #print("send message:", message)
