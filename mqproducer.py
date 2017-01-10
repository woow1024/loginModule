import pika  
import sys
import urllib2
import json

def connect_mq():
    auth = pika.PlainCredentials('', '')
    parameters = pika.ConnectionParameters('localhost', 5672, '/');
    try:
        host_url = 'localhost'
        connection = pika.BlockingConnection(parameters)  
        channel = connection.channel()  
        channel.queue_declare(queue='my_queue', durable=True)
    except Exception, e:
        print("error", ':',e)
        return None
    return channel



channel =  connect_mq()
message = "hello, world"
try:
        #message=("%d helloworld"%count)
        message={'u':'MTg5MTgxOTIzOTA=','p':'OTZlNzkyMTg5NjVlYjcyYzky'}
        json_str = json.dumps(message)
        channel.basic_publish(exchange='',
                      routing_key='my_queue',
                      body=json_str,
                      properties=pika.BasicProperties(
                          #delivery_mode = 2
                      ))
        #time.sleep(1)
except Exception, e:
    print(e)
finally:
    print('done')
    
    #print("send message:", message)