 # -*- coding:utf-8 -*-     
import pika    
class RabbitMQ:
    def __init__(self, 
                 host = 'localhost',
                 port = 5672,
                 user = 'guest',
                 pwd = 'guest',
                 queue = None):
        self.host = host
        self.port = port
        self.user = user
        self.pwd = pwd
        self.queue=''
        self.prfetch_count = 2
    
    def connect_mq(self, queue='', vhost='/'):
        try:
            auth = pika.PlainCredentials(self.user, self.pwd)
            parameters = pika.ConnectionParameters(host=self.host, 
                                               port=self.port,
                                               virtual_host=vhost,
                                               credentials=auth
                                               );    
            self.conn = pika.BlockingConnection(parameters)  
            self.channel = self.conn.channel()  
            self.channel.queue_declare(queue=queue, durable=True)  
            print 'connect rabbitmq success...'
        except Exception,e:
            print "connect mq error %s" %e
            return None
    
    def get_channel(self):
        return self.channel
    
    def set_queue_count(self, count):
        self.prfetch_count = count
        
    def start_Consumer(self, callback):
        try:
            #设置最多分给worker2个任务，多余的分配给其他worker
            self.channel.basic_qos(prefetch_count=self.prfetch_count)
            self.channel.basic_consume(callback, queue = self.queue,
                                               no_ack=False)
            print "start recvive msg..."
            self.channel.start_consuming()
        except Exception, e:
            print(e)     
            
    def start_producer(self, msg, queue_name=''):   
        try:
            self.queue = queue_name
            #json_str = json.dumps(msg)
            self.channel.basic_publish(exchange='',
                                   routing_key=self.queue,
                                   body=msg,
                                   properties=pika.BasicProperties(
                                       #delivery_mode = 2
                                   ))	
        except Exception, e:
            print e
            #print("%s %s",__file__,e)   
            
    def confirm(self, ch, method):
            ch.basic_ack(delivery_tag=method.delivery_tag)
        