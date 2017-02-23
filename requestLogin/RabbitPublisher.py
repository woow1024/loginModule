# -*- coding: utf-8 -*-

import logging
import pika
import time
LOG_FORMAT = ('%(levelname) -10s %(asctime)s %(name) -30s %(funcName) '
              '-35s %(lineno) -5d: %(message)s')
LOGGER = logging.getLogger(__name__)


class RabbitPublisher(object):
    _CONN_FLAG = True
    _RECONNECT_TIME = 0
    def __init__(self, amqp_url):
        self._connection = None
        self._channel = None
        self._closing = False
        self._consumer_tag = None
        self._url = amqp_url
        
    def __init__(self,
                 host = '192.168.44.223',
                 port = 5672,
                 user = '',
                 pwd = '',
                 vhost ='/',
                 queue = None):
        
        self._url =  'amqp://%s:%s@%s:%d/%s' %(user, pwd, host, port, vhost)
        self._connection = None
        self._channel = None
        self._closing = False
        self._consumer_tag = None    
    #设置作为消费者的 exchange， queue routingkey
    def set_args(self,exchange, type,  queue, routing_key, on_msg_callcack=None):
        self.EXCHANGE = exchange
        self.EXCHANGE_TYPE = type
        self.QUEUE = queue
        self.ROUTING_KEY = routing_key
        self.CALLBACK = on_msg_callcack
    def add_time_out(self, seconds, func):
        #指定秒数之后执行函数
        self._connection.add_timeout(deadline=time, callback_method=func)
        
    def connect(self):
        """
        :rtype: pika.SelectConnection
        连接到RabbitMQ, 连接成功建立后，会触发on_connection_open函数
        """
        LOGGER.info('Connecting to %s', self._url)
        return pika.SelectConnection(pika.URLParameters(self._url),
                                     self.on_connection_open,
                                     stop_ioloop_on_close=False,
                                     on_open_error_callback=self.on_connect_open_err)
    def on_connect_open_err(self, err, reason):
        self._CONN_FLAG = False
        LOGGER.info('connect open err...'  + reason)
        try:
            self._connection.ioloop.stop()
        except Exception, e:
            LOGGER.info('stop io loop err')   
            
        
    def on_connection_open(self, unused_connection):
        """
        :type unused_connection: pika.SelectConnection
        """
        self._CONN_FLAG = True
        LOGGER.info('Connection opened')
        self._connection.add_on_close_callback(self.on_connection_closed)
        self.open_channel()

    def on_connection_closed(self, connection, reply_code, reply_text):
        self._CONN_FLAG = False
        self._channel = None
        
        #if False == self._closing:
         
        LOGGER.info('connection close..........')
                     
        try:
            self._connection.ioloop.stop()
        except Exception, e:
            LOGGER.info('stop io loop err')           
        
            #self._connection.add_timeout(5, self.reconnect)

    def reconnect(self):
        """
        no use for now
        """
        # This is the old connection IOLoop instance, stop its ioloop
        self._connection.ioloop.stop()

        if not self._closing:

            # Create a new connection
            self._connection = self.connect()

            # There is now a new connection, needs a new ioloop to run
            self._connection.ioloop.start()

    def open_channel(self):
        LOGGER.info('Creating a new channel')
        self._connection.channel(on_open_callback=self.on_channel_open)

    def on_channel_open(self, channel):
        """
        """
        LOGGER.info('Channel opened')
        self._channel = channel
        self._channel.add_on_close_callback(self.on_channel_closed)
        #self.setup_exchange(self.EXCHANGE)
        self.setup_queue(self.QUEUE)

    def on_channel_closed(self, channel, reply_code, reply_text):
        """
        """
        LOGGER.warning('Channel %i was closed: (%s) %s',
                       channel, reply_code, reply_text)
        self._connection.close()

    def setup_exchange(self, exchange_name):
        """
        """
        LOGGER.info('Declaring exchange %s', exchange_name)
        try:
            self._channel.exchange_declare(self.on_exchange_declareok,
                                       exchange_name,
                                       self.EXCHANGE_TYPE)
        except Exception, e:
            print ('declaring exchange failed' + e)

    def on_exchange_declareok(self, unused_frame):
        """
        """
        LOGGER.info('Exchange declared')
        self.setup_queue(self.QUEUE)

    def setup_queue(self, queue_name):
        """
        """
        LOGGER.info('Declaring queue %s', queue_name)
        self._channel.queue_declare(self.on_queue_declareok, queue_name)

    def on_queue_declareok(self, method_frame):
        """
        """
        LOGGER.info('Binding %s to %s with %s',
                    self.EXCHANGE, self.QUEUE, self.ROUTING_KEY)
        self._channel.queue_bind(self.on_bindok, self.QUEUE,
                                 self.EXCHANGE, self.ROUTING_KEY)

    def on_bindok(self, unused_frame):
        """I
        """
        LOGGER.info('Queue bound')
        self.start_consuming()

    def start_consuming(self):
        """
        """
        LOGGER.info('Issuing consumer related RPC commands')
        self._channel.add_on_cancel_callback(self.on_consumer_cancelled)
        self._channel.basic_qos(prefetch_count = 2)     
        #self._consumer_tag = self._channel.basic_consume(self.CALLBACK,self.QUEUE, no_ack=False)
    
    def publish(self, routing_key, msg):
        try:
            #self._channel.exchange_declare(exchange=exchange,type='direct')
            self._channel.basic_publish(exchange=self.EXCHANGE,
                                       routing_key=routing_key,
                                       body=msg,
                                       properties=pika.BasicProperties(
                                           #delivery_mode = 2
                                       ))   
        except Exception, e:
            print e
        
    def on_consumer_cancelled(self, method_frame):
        """Invoked by pika when RabbitMQ sends a Basic.Cancel for a consumer
        receiving messages.

        """
        LOGGER.info('Consumer was cancelled remotely, shutting down: %r',
                    method_frame)
        if self._channel:
            self._channel.close()

    def on_message(self, unused_channel, basic_deliver, properties, body):
        """
        """
        LOGGER.info('Received message # %s from %s: %s',
                    basic_deliver.delivery_tag, properties.app_id, body)
        self.acknowledge_message(basic_deliver.delivery_tag)

    def acknowledge_message(self, delivery_tag):
        """Acknowledge the message delivery from RabbitMQ by sending a
        Basic.Ack RPC method for the delivery tag.

        :param int delivery_tag: The delivery tag from the Basic.Deliver frame

        """
        LOGGER.info('Acknowledging message %s', delivery_tag)
        self._channel.basic_ack(delivery_tag)

    def stop_consuming(self):
        """
        """
        if self._channel:
            LOGGER.info('Sending a Basic.Cancel RPC command to RabbitMQ')
            self._channel.basic_cancel(self.on_cancelok, self._consumer_tag)

    def on_cancelok(self, unused_frame):
        """
        """
        LOGGER.info('RabbitMQ acknowledged the cancellation of the consumer')
        self.close_channel()

    def close_channel(self):
        """
        """
        LOGGER.info('Closing the channel')
        self._channel.close()
        
    def run(self):
        """Run and  connecting to RabbitMQ and then
        starting the IOLoop to block and allow the SelectConnection to operate.
        """
        while True:
            try:
                self._CONN_FLAG = True
                self._connection = self.connect()
                if self._CONN_FLAG is not False:
                    self._connection.ioloop.start()
            except Exception, e:
                LOGGER.info('Closing the channel')
                LOGGER.info('%s, reopening in 3 seconds...', e)
            finally:
                time.sleep(3)

    def stop(self):
        """
        """
        LOGGER.info('Stopping')
        self._closing = True
        self.stop_consuming()
        self._connection.ioloop.start()
        LOGGER.info('Stopped')

    def close_connection(self):
        """This method closes the connection to RabbitMQ."""
        LOGGER.info('Closing connection')
        self._connection.close()
