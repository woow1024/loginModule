# -*- coding: utf-8 -*-
from igetui.template.igt_transmission_template import *
from igetui.template.igt_notification_template import *
from igetui.igt_target import *
from igt_push import *
from igetui.template import *
from  config import config
import Redis 
import logging


__all__ = [ 'pushMessageToList', 'NotificationTemplateDemo']

TITLE = u"如意期货通"
push = IGeTui(config['IGT_HOST'],config['ANDROID_APPKEY'],config['ANDROID_MASTERSECRET'])
class PushToAndroid(object):
    def __init__(self):
	self.cid = None
	self.logger = logging.getLogger("Push.push_getui")
        
   
    #单推
    def pushMessageToSingle(self,cid,text):
	template = self.NotificationTemplateDemo(TITLE,text)
	self.cid = cid
	message = IGtListMessage()
	message.data = template
	message.isOffline = True
	message.offlineExpireTime = 1000 * 3600 * 12
	message.pushNetWorkType = 0
    
	target = Target()
	target.appId = config['ANDROID_APPID']
	target.clientId = cid
	target.alias = config['ALIAS']
	arr = []
    
	arr.append(target)
	contentId = push.getContentId(message, u'List_alarm')
	try:
	    ret = push.pushMessageToSingle(message, target)
	    self.logger.info("android push succeed state:%s,clientID: %s",ret['status'],cid)
	except RequestException, e:
	    # 发生异常重新发送
	    requstId = e.getRequestId()
	    ret = push.pushMessageToSingle(message, target, requstId)
	    if ret['result'] != 'ok':
		self.logger.warn("android push failed %s",ret)
	    
    def NotificationTemplateDemo(self,title,text):
	template = NotificationTemplate()
	template.appId = config['ANDROID_APPID']
	template.appKey = config['ANDROID_APPKEY']
	template.transmissionType = 1
	template.transmissionContent = self.cid
	template.title = title
	template.text = text
	template.logo = "" 
	template.logoURL = config['LOGOURL']
	template.isRing = True
	template.isVibrate = True
	template.isClearable = True
	# begin = "2015-03-04 17:40:22"
	# end = "2015-03-04 17:47:24"
	# template.setDuration(begin, end)
	return template         
    
    def getUserStatus(self):
	print push.getClientIdStatus(config['ANDROID_APPID'], config['CID'])

    def pushMessageToList(self,clientId,title, text):
	
	template = self.NotificationTemplateDemo(title,text)
	message = IGtListMessage()
	message.data = template
	message.isOffline = True
	message.offlineExpireTime = 1000 * 3600 * 12
	message.pushNetWorkType = 0
    
	target1 = Target()
	target1.appId = config['ANDROID_APPID']
	target1.clientId = clientId
	target1.alias = config['ALIAS']
	arr = []
    
	arr.append(target1)
	contentId = pushA.getContentId(message, u'List_alarm')
	ret = pushA.pushMessageToList(contentId, arr)
	print ret
        

	
    def worker(self,clientId,text):
	#self.getUserStatus()
	self.pushMessageToSingle(clientId,text)
        #gevent.spawn(self.pushMessageToList,clientId,title,text)