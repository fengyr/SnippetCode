# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/topics/item-pipeline.html
import csv

class BaiduMp3Pipeline(object):
    def __init__(self):
        self.file = open('baidu_top100.txt', 'w')

    def process_item(self, domain, item):
        context = '%s\t%s\t%s' % (item['songname'].encode('utf8'), item['author'].encode('utf8'), item['link'].encode('utf8'))
        self.file.write(context)
        self.file.write('\n')
        return item
