#!/usr/bin/env python
# -*- coding: utf-8 -*-

from celery import Celery

CELERY_BROKER_URL = 'redis://localhost:6379'
def make_celery(broker):
    celery = Celery(name=APP_NAME, broker=broker)
    