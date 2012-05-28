# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/topics/item-pipeline.html
import csv

class TestPipeline(object):
    def process_item(self, domain, item):
        return item

class CsvPipeline(object):
    def __init__(self):
        self.csvwriter = csv.writer(open('item.csv', 'wb'))

    def process_item(self, domain, item):
        for i in range(len(item['title'])):
            self.csvwriter.writerow([item['title'][i], item['link'][i], item['desc'][i]])
        return item

