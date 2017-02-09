# -*- coding:utf-8 -*-  
import gevent
import urllib2
import urllib
import json
import logging
from gevent import monkey
from xml.etree import ElementTree  
from public import send_to_mq
from public import *
from public import write_redis

ini_url = 'http://test.user-api.yinrui99.com/apis/pc/yg/client/login?sid=3'
class Http:
    def __init__(self, ):
        pass
    
    def send_login_request(self, url):
        value = {}
        data = urllib.urlencode(value)
        req=urllib2.Request(url)
        try:
            resp = urllib2.urlopen(req)
        except Exception, e:
            logging.info("send login request error[%s]"%e)	
            return False
        self.res_data = resp.read()
        print(self.res_data) 
        
        #send_to_mq(self.res_data)
    def return_json_res(self, iniStr):
        return json.loads(iniStr)
    
    def decode_login_res(self, res=''):
        try:
            #decode xml
            root= ElementTree.fromstring(self.res_data)
            #decode json
            msg_json = root.find('msg').text
            result = json.loads(msg_json)
            return result
            
           # keyname = result['user']['username']  
        except Exception, e:
            print(e)

    def pool_method(self, url):
        try:
            g = pool.Pool()
            g.spawn(self.send_login_request, url)
                #time.sleep(1)
        except Exception,e:
            print(e)	
        return None

    def get_url_from_json(self,body):
        return None
        
    def normal_method(self, body):
        #url = self.get_url_from_json(body)
        try:
            gevent.spawn(self.work, body)
        except Exception,e:
            print(e)	
        return None   
    
    def __compos_url(self,jsonDdata):
        try:
            global ini_url
            new_url = ini_url + '&u=' + jsonDdata['usr'] + '&p=' + jsonDdata['pwd']
            return new_url
        except Exception, e:
            print(e)  
            
            
    def prepare_send_to_mq(self,iniJsonData, serverData):
        
        
        #构造返回报文
        resData = {}
        resData['cmd'] = iniJsonData['cmd']
        resData['from'] = iniJsonData['from']
        resData['seq'] = iniJsonData['seq'] 
        resData['ret'] = serverData['ret']
        resData['msg'] = serverData['msg']
        resData['success'] = serverData['success']
        if resData['success'] == 'true':
            #检查redis
            tjPriv = serverData['r'][0][u'func']
            res = write_redis(iniJsonData['usr'], 'func',tjPriv )            
            resData['permission'] = serverData['r'][0][u'permission']
            resData['func'] = tjPriv
            resData['action'] = res
       
        print resData
        send_to_mq(json.dumps(resData))
        
    def work(self, msg):
        jsonData = self.return_json_res(msg)
        url = self.__compos_url(jsonData)
        self.send_login_request(url)
        res = self.decode_login_res()
        self.prepare_send_to_mq(jsonData,res)
        
        
   