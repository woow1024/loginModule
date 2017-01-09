import pika  
import sys
import urllib2

def connect_mq():
    auth = pika.PlainCredentials('admin', '000000')
    parameters = pika.ConnectionParameters('192.168.94.230', 5672, '/',auth);
    try:
        host_url = '192.168.1.23'
        connection = pika.BlockingConnection(parameters)  
        channel = connection.channel()  
        channel.queue_declare(queue='my_queue', durable=True)
    except Exception, e:
        print("error", ':',e)
        return None
    return channel


count = 0
channel =  connect_mq()
message = "hello, world"
try:
    for i in range(1000):
        global count
        count += 1
        #message=("%d helloworld"%count)
        message="http://httpbin.org/get"
        channel.basic_publish(exchange='',
                      routing_key='my_queue',
                      body=message,
                      properties=pika.BasicProperties(
                          #delivery_mode = 2
                      ))
        #time.sleep(1)
except Exception, e:
    print(e)
finally:
    print('done')
    
    #print("send message:", message)