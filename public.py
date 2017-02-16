from RabbitMq import RabbitMQ
from Redis import RedisDb
import time
import logging

HOST = '192.168.44.223'
PORT_MQ = 5672
PORT_REDIS = 6379
USER_MQ = 'cc'
PASSWD_MQ = '123'
PASSWD_REDIS =''
VHOST = 'test'

att_dict={'func'}
redis = RedisDb(host=HOST, 
                port=PORT_REDIS, 
                pwd=PASSWD_REDIS
                )

producer = RabbitMQ(host=HOST, 
                    port=PORT_MQ, 
                    user=USER_MQ, 
                    pwd=PASSWD_MQ,
                    vhost=VHOST)

consumer = RabbitMQ(host=HOST, 
                    port=PORT_MQ, 
                    user=USER_MQ, 
                    pwd=PASSWD_MQ,
                    vhost=VHOST)


def send_to_mq(msg):
    try:
        producer.connect_mq()
        producer.start_producer(msg=msg, 
                            exchange='FSExchange1', 
                            routing_key='FSReplay')
        
    except Exception,e:
        raise "reconnet error:" ,e
          
    logging.info("send %s" ,msg)
    print "send [%s]" %msg
    

 
def write_redis(keyname,jsonAttr,jsonData):
    conn = redis.get_res_conn()
    if conn is None:
        logging.error("res_conn none : %s", str(error))
        return False    
    res = user_exists(keyname)
    if not res:
        return 'false'
    elif res == 'not exists':
        conn.hset(keyname,jsonAttr, jsonData)
        return 'login'
    elif res == 'exists':
        return 'kickout'
    
def user_exists(keyname):
    conn = redis.get_res_conn()
    if conn is None:
        logging.error("res_conn none : %s", str(error))
        return False
    try:
        len = conn.hlen(keyname)
        if len == 0:
            return 'not exists'
        else:
            return 'exists'
    except Exception,error:
        logging.error("check user_exists failed : %s", str(error))
        return False       
    
def delete_redis(keyname):
    global att_dict
    conn = redis.get_res_conn()
    if conn is None:
        logging.error("res_conn none : %s", str(error))
        return False    
    
    for i in att_dict:
        conn.hdel(keyname,i)
    h_data = conn.hgetall(keyname)
    #print h_data 

