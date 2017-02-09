from RabbitMq import RabbitMQ
from Redis import RedisDb


redisServerInfo={'host':'192.168.44.223',
                  'port':6379,
                  'pwd':''}

mqServerInfo={'host':'192.168.44.223',
           'port':5672,
           'user':'cc',
           'pwd':'123',
           'vhost':'test'}


redis = RedisDb(host=redisServerInfo['host'], 
                port=redisServerInfo['port'], 
                pwd=redisServerInfo['pwd']
                )

producer = RabbitMQ(host=mqServerInfo['host'], 
                    port=mqServerInfo['port'], 
                    user=mqServerInfo['user'], 
                    pwd=mqServerInfo['pwd'],
                    vhost=mqServerInfo['vhost'])

consumer = RabbitMQ(host=mqServerInfo['host'], 
                    port=mqServerInfo['port'], 
                    user=mqServerInfo['user'], 
                    pwd=mqServerInfo['pwd'],
                    vhost=mqServerInfo['vhost'])


def send_to_mq(msg):
    producer.start_producer(msg=msg, 
                            exchange='FSExchange1', 
                            routing_key='FSReplay')

 
def write_redis(keyname,jsonAttr,jsonData):
    global redis
    conn = redis.get_res_conn()
    if conn is None:
        logging.error("res_conn none : %s", str(error))
        return 'false'    
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
    conn = redis.get_res_conn()
    if conn is None:
        logging.error("connect to redis failed : %s", str(error))
        return False    
    for i in att_dict:
        conn.hdel(keyname,i)
    h_data = conn.hgetall(keyname)
            #print h_data 

