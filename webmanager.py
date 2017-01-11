# -*- coding:utf-8 -*-  
from Queue import Queue
from mqworker import my_mq
import os
import threading
import json
from time import sleep
produce_queue = Queue()


def produce_func(args):
    #q = args
    global produce_queue
    produce_mq = my_mq(host='127.0.0.1', 
                       port=5672, 
                       user='admin', 
                       pwd='000000')
    produce_mq.connect_mq('my_queue1')     
    while True:
        if produce_queue.qsize()>0:
            #启动发送队列
            msg = produce_queue.get()
            try:
                # channel =  self.__connect_mq(queue_name)
                json_str = json.dumps(msg)
                self.channel.basic_publish(exchange='',
                               routing_key='my_queue1',
                               body=json_str,
                               properties=pika.BasicProperties(
                                   #delivery_mode = 2
                               ))	
            except Exception, e:
                print e
        else:
            sleep(0.001)
            
if __name__ == '__main__':  
    try:

        threads_handle = []
        p_thread = threading.Thread(target=produce_func, args=(1,))
        threads_handle.append(p_thread)
    
        p_thread.start()
        
        #c_thread = threading.Thread(target=consum_func, args=produce_queue)
        
        #启动接收队列
        mq = my_mq(host='127.0.0.1', 
                   port=5672, 
                   user='', 
                   pwd='',
                   queue=produce_queue)

        mq.initRedisPool(host='127.0.0.1', 
                         port=6379, 
                         db=0, 
                         pwd=None)

        print 'init redis pool success..'
        mq.connect_mq(queue_name='my_queue')
        mq.star_consuming(queue_name='my_queue')

    except Exception, error:
        print error  
    #start_multi_process(func=main, worknum=4)


