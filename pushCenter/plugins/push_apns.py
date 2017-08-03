#!/usr/bin/env python
#coding: utf-8

import OpenSSL
OpenSSL.SSL.SSLv3_METHOD = OpenSSL.SSL.TLSv1_METHOD

import sys
import json
import urllib
import urllib2
import logging
from apnsclient import *
import redis
from config import config
import logging

from OpenSSL import crypto
import OpenSSL
OpenSSL.SSL.SSLv3_METHOD = OpenSSL.SSL.TLSv1_METHOD

########################################################################
class  pushAPNs:
    """"""

    def __init__(self,  environment, timeout=15):
        self.factory = None
        self.environment = environment
        self.cert_file = config['P12_FILENAME']
        self.raw_mode = False
	self.pwd = config['PWD']
        self.key = None
	self.pem = None
	self.logger = logging.getLogger("Push.push_apns")
    
    def load_cert(self,pwd):
	try:
	    cert = crypto.load_pkcs12(open(self.cert_file, 'rb').read(), pwd)
		# PEM formatted private key
	    self.key = crypto.dump_privatekey(crypto.FILETYPE_PEM, cert.get_privatekey())
		# PEM formatted certificate
	    self.pem =  crypto.dump_certificate(crypto.FILETYPE_PEM, cert.get_certificate())
	    self.logger.info("load cert successfully")
	except Exception, e:
	    self.logger.warn("error in loading cert ")
	    raise e
        
    def connect(self,environ,pwd):
	try:
	    self.load_cert(pwd)
	    session = Session()
	    return session.get_connection(environ, cert_string=self.pem, key_string=self.key)
	except Exception,e:
	    self.logger.warn("error in connecting to apns server ")
	    raise e 
        
    def push(self, device, notifications):
	
	con = self.connect(self.environment,self.pwd)
	message = Message([ device], alert=notifications, badge=10)
	rcv = APNs(con)
	res = rcv.send(message)
	for token, reason in res.failed.items():
		code, errmsg = reason
		self.logger.warn("failed DeviceToken: %s, reason: %s ",token,errmsg)
		#print res.retry()
	

    
    
    
    
    
        
        
    
    