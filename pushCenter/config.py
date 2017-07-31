# -*- coding: utf-8 -*-
import os
import yaml
import logging
from plugins.Redis import  RedisDb

__all__ = ['LOG_FORMAT','LOGGER','redis','config']

#logging
LOG_FORMAT = ('%(levelname) -10s %(asctime)s %(name) -30s %(funcName) '
              '-35s %(lineno) -5d: %(message)s')
LOGGER = logging.getLogger(__name__)

config_path = os.environ.get('APP_SETTINGS')
if config_path is None:
    config = yaml.load(open('settings.yaml'))
else:
    config = yaml.load(open(config_path))
    
#load config	
redis = RedisDb(host=config['REDIS_URL'])
   
   


