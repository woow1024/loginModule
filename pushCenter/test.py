# -*- coding: utf-8 -*-
from array import array

__author__ = 'wei'

import jsonpickle
from igt_push import *
from igetui.template import *
from igetui.template.igt_base_template import *
from igetui.template.igt_transmission_template import *
from igetui.template.igt_link_template import *
from igetui.template.igt_notification_template import *
from igetui.template.igt_notypopload_template import *
from igetui.template.igt_apn_template import *
from igetui.igt_message import *
from igetui.igt_target import *
from igetui.template import *
from BatchImpl import *
from payload.APNPayload import *
from igetui.utils.AppConditions import *

# toList接口每个用户返回用户状态开关,true：打开 false：关闭
os.environ['needDetails'] = 'true'
# APPKEY = ""
# APPID = ""
# MASTERSECRET = ""
# CID = ""
HOST = 'http://sdk.open.api.igexin.com/apiex.htm'
Alias = '请输入别名'
DEVICETOKEN = ""

APPKEY = "5Ppd2ltAfx7M4QbfkJ1vN9"
APPID = "kILp1JiBjv7vmuVTSGOc38"
MASTERSECRET = "zDnVTa1INk50HlV6Wy62p9"
CID = "3bbf411607d756ccb24d3bfd4215338e"

def pushMessageToList():
    push = IGeTui(HOST, APPKEY, MASTERSECRET)

    # 消息模版：
    # 1.TransmissionTemplate:透传功能模板
    # 2.LinkTemplate:通知打开链接功能模板
    # 3.NotificationTemplate：通知透传功能模板
    # 4.NotyPopLoadTemplate：通知弹框下载功能模板

    template = NotificationTemplateDemo()
    # template = LinkTemplateDemo()
    # template = TransmissionTemplateDemo()
    # template = NotyPopLoadTemplateDemo()

    message = IGtListMessage()
    message.data = template
    message.isOffline = True
    message.offlineExpireTime = 1000 * 3600 * 12
    message.pushNetWorkType = 0

    target1 = Target()
    target1.appId = APPID
    target1.clientId = CID
    target1.alias = Alias
    arr = []

    arr.append(target1)
    contentId = push.getContentId(message, 'ToList_任务别名_可为空')
    ret = push.pushMessageToList(contentId, arr)
    print ret


# 通知透传模板动作内容
def NotificationTemplateDemo():
    template = NotificationTemplate()
    template.appId = APPID
    template.appKey = APPKEY
    template.transmissionType = 1
    template.transmissionContent = u"hi"
    template.title = u"通知标题"
    template.text = u"通知测试"
    template.logo = "icon.png"
    template.logoURL = ""
    template.isRing = True
    template.isVibrate = True
    template.isClearable = True
    # begin = "2015-03-04 17:40:22"
    # end = "2015-03-04 17:47:24"
    # template.setDuration(begin, end)
    return template

# queryAppPushDataByDate()
# queryAppUserDataByDate()
# pushMessageToSingle()
# pushMessageToSingleBatch()
pushMessageToList()
# pushMessageToApp()

# IOS简化版推送接口
# pushAPN()

# 获取用户状态接口
# getUserStatus()

# 任务停止功能接口
# stopTask()

# 通过服务端设置用户标签
# setTag()

# 根据TaskID返回任务推送结果功能
# getPushResultTest()

# 根据ClientID查询标签功能
# getUserTagsTest()

# 获取appid可以操作的用户属性
# getPersonaTagsDemo()
