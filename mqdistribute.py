import pika  
import sys
import urllib2
import funclib

host_url = 'localhost'
connection = pika.BlockingConnection(pika.ConnectionParameters(host=host_url))  
channel = connection.channel()  


print ' [*] Waiting for messages.'  
    
#'http://user-api.yinrui99.com/apis/pc/yg/client/login?sid=3&u=MTg5MTgxOTIzOTA=&p=OTZlNzkyMTg5NjVlYjcyYzky'
def yield_distribute(content):
    call_http_method(content)
    print " [x] Received %r" % (body,)  
    
    
def callback(ch, method, properties, body):  
    yield_distribute(body)
   


que_name = sys.argv[1]

channel.basic_consume(callback,  
                      queue=que_name,  
                      no_ack=True)  

channel.start_consuming()  