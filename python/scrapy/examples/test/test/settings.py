#!encoding=utf8
# Scrapy settings for test project
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

BOT_NAME = 'test'
BOT_VERSION = '1.0'

SPIDER_MODULES = ['test.spiders']
NEWSPIDER_MODULE = 'test.spiders'
DEFAULT_ITEM_CLASS = 'test.items.TestItem'
USER_AGENT = '%s/%s' % (BOT_NAME, BOT_VERSION)

#加入pipeline配置
ITEM_PIPELINES = ['test.pipelines.CsvPipeline']

