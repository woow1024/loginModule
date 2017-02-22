 # -*- coding:utf-8 -*-     
import pika    
class RabbitMQ:
    def __init__(self, 
                 host = '192.168.44.223',
                 port = 5672,
                 user = '',
                 pwd = '',
                 vhost ='/',
                 queue = None):
        self.host = host
        self.port = port
        self.user = user
        self.pwd = pwd
        self.queue= ''
        self.vhost = vhost
        self.prfetch_count = 2
     
        #FS.test111_13
    def connect_mq(self):
        try:
            auth = pika.PlainCredentials(self.user, self.pwd)
            parameters = pika.ConnectionParameters(host=self.host, 
                                               port=self.port,
                                               virtual_host=self.vhost,
                                               credentials=auth
                                               )
            self.conn = pika.BlockingConnection(parameters)  
            self.channel = self.conn.channel() 
            print 'connect rabbitmq success...'
        except Exception,e:
            raise "connect mq error %s" %e
        
    def get_channel(self):
        return self.channel
    
    def set_queue_count(self, count):
        self.prfetch_count = count
        
    def start_Consumer(self, exchange, queue, callback, serverLists):
        try:
            self.channel.queue_declare(queue=queue, durable=True) 
           
            for serverList in serverLists:
                #将队列绑定到不同的routingKey上
                self.channel.queue_bind(exchange=exchange,  
                                        queue=queue, routing_key=serverList) 
                
            self.channel.basic_qos(prefetch_count=self.prfetch_count)
            self.channel.basic_consume(callback, queue = queue,
                                               no_ack=False)
            print "start recvive msg..."
            self.channel.start_consuming()
        except Exception, e:
            print(e)     
            
    def start_producer(self, msg, exchange, routing_key='FSReplay'):   
        try:
            #json_str = json.dumps(msg)
            self.channel.exchange_declare(exchange=exchange,type='direct')
            self.channel.basic_publish(exchange=exchange,
                                   routing_key=routing_key,
                                   body=msg,
                                   properties=pika.BasicProperties(
                                       #delivery_mode = 2
                                   ))

        except Exception, e:
            print e
            #self.channel.close()
            
            print("%s %s",__file__,e)   
            
    def confirm(self, ch, method):
            ch.basic_ack(delivery_tag=method.delivery_tag)
        