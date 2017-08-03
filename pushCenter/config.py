# -*- coding: utf-8 -*-
import os
import yaml
import logging

from plugins.Redis import  RedisDb

__all__ = ['redis','config']



config_path = os.environ.get('APP_SETTINGS')
if config_path is None:
    config = yaml.load(open('settings.yaml'))
else:
    config = yaml.load(open(config_path))
    
#load config	
redis = RedisDb(host=config['REDIS_URL'])
   





