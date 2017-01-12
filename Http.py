# -*- coding:utf-8 -*-  
import gevent
import urllib2
import urllib
import json
import logging
from gevent import monkey
from xml.etree import ElementTree  
from public import send_to_mq
from public import user_exists

ini_url = 'http://www.baidu.com/apis/pc/yg/client/login?sid=3'
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
        
        send_to_mq(self.res_data)
        
    def decode_login_res(self, res=''):
        try:
            #decode xml
            root= ElementTree.fromstring(self.res_data)
            #decode json
            msg_json = root.find('msg').text
            result = json.loads(msg_json)
            if( result['ret'] != 0 ):
                return  False
            keyname = result['user']['username']  
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

    def normal_method(self, url):
        try:
            gevent.spawn(self.work, url)
        except Exception,e:
            print(e)	
        return None   
    
    def __compos_url(self,jsondata):
        try:
            json_data = json.loads(jsondata)
            print json_data
            new_url = ini_url + '&u=' + json_data['u'] + '&p=' + json_data['p']
            return new_url
        except Exception, e:
            print(e)     
            
    def work(self, msg):
        url = self.__compos_url(msg)
        self.send_login_request(url)
        self.decode_login_res();
        
    def check_result(self,):
        pass
        #res = .write_redis( keyname,'func',result['r'][0]['func'])