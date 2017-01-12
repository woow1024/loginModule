from RabbitMq import RabbitMQ
from Redis import RedisDb

#redisConfig= {}
#mqConfig = {}

redis = RedisDb(host='192.168.94.230', port=6379, pwd='')
producer = RabbitMQ(host='127.0.0.1', port=5672, user='guest', pwd='guest')
consumer = RabbitMQ(host='192.168.94.230', port=5672, user='admin', pwd='000000')

def send_to_mq(msg):
    producer.start_producer(msg)

 
def write_redis(self, keyname,jsonAttr,jsonData):
    res = self.user_exists
    if not res:
        return False
    elif res == 'not exists':
        self.conn.hset(keyname,jsonAttr, jsonData)
        return 'success'
    elif res == 'exits':
        return 'kickout'
    

def user_exists(self,keyname):
    conn = redis.get_res_conn()
    if conn is None:
        logging.error("connect to redis failed : %s", str(error))
        return False
    try:
        len = conn.hlen(keyname)
        if len == 0:
            return 'not exists'
        else:
            return 'exists'
    except Exception,error:
        logging.error("connect to redis failed : %s", str(error))
        return False       
    
def delete_redis(self,keyname):
    conn = redis.get_res_conn()
    if conn is None:
        logging.error("connect to redis failed : %s", str(error))
        return False    
    for i in att_dict:
        conn.hdel(keyname,i)
    h_data = conn.hgetall(keyname)
            #print h_data 
