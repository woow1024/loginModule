import pika  
 
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

auth = pika.PlainCredentials('cc', '123')
parameters = pika.ConnectionParameters(host='localhost', 
                                               virtual_host='test',
                                               credentials=auth
                                               )
connection = pika.BlockingConnection(parameters)  
#connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost', virtual_host='test'))  
channel = connection.channel()  
  
result = channel.queue_declare(exclusive=True)  
  
queue_name = result.method.queue  

  
channel.queue_bind(exchange='FSExchange1',  
                   queue=queue_name, routing_key='FSReplay')  
  
print ' [*] Waiting for logs. To exit press CTRL+C'  
  
def callback(ch, method, properties, body):  
    print " [x] %r" % (body,)  
  
channel.basic_consume(callback,  
                      queue=queue_name,  
                      no_ack=True)  
  
channel.start_consuming() 
