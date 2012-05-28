# Scrapy settings for baidu_mp3 project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/topics/settings.html
#
# Or you can copy and paste them from where they're defined in Scrapy:
# 
#     scrapy/conf/default_settings.py
#

BOT_NAME = 'baidu_mp3'
BOT_VERSION = '1.0'

SPIDER_MODULES = ['baidu_mp3.spiders']
NEWSPIDER_MODULE = 'baidu_mp3.spiders'
DEFAULT_ITEM_CLASS = 'baidu_mp3.items.BaiduMp3Item'
USER_AGENT = '%s/%s' % (BOT_NAME, BOT_VERSION)
#LOG_ENABLED = False

ITEM_PIPELINES = ['baidu_mp3.pipelines.BaiduMp3Pipeline']
