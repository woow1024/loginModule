# -*- coding: utf-8 -*-
from igetui.template.igt_transmission_template import *
from igetui.template.igt_notification_template import *
from igetui.igt_target import *
from igt_push import *
from igetui.template import *
from config import config
import gevent
import gevent.monkey
gevent.monkey.patch_all()

__all__ = [ 'pushMessageToList', 'NotificationTemplateDemo']

class PushToAndroid(object):
    def __init__(self,
                 host = config['IGT_HOST'],
                 appkey = config['ANDROID_APPKEY'],
                 mastersecret = config['ANDROID_MASTERSECRET']):
        self.host = config['IGT_HOST']
        self.appkey = appkey
        self.mastersecret = mastersecret
        
    # 通知透传模板动作内容
    def NotificationTemplateDemo(self,title,text):
        template = NotificationTemplate()
        template.appId = config['ANDROID_APPID']
        template.appKey = config['ANDROID_APPKEY']
        template.transmissionType = 1
        template.transmissionContent = config["CID"]
        template.title = title
        template.text = text
        template.logo = "icon.png"
        template.logoURL = "./"
        template.isRing = True
        template.isVibrate = True
        template.isClearable = True
        # begin = "2015-03-04 17:40:22"
        # end = "2015-03-04 17:47:24"
        # template.setDuration(begin, end)
        return template                
    
    def pushMessageToList(self,clientId,title, text):
        
        push = IGeTui(self.host, self.appkey,self.mastersecret )

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
        contentId = push.getContentId(message, u'ToList_测试')
        ret = push.pushMessageToList(contentId, arr)
        print ret
        
        
    def worker(self,clientId,title,text):
        gevent.spawn(self.pushMessageToList,clientId,title,text)