#!/usr/bin/env python

import os
os.environ.setdefault('SCRAPY_SETTINGS_MODULE', 'baidu_mp3.settings')

from scrapy.command.cmdline import execute
execute()
