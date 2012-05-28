from scrapy.spider import BaseSpider
from scrapy.selector import HtmlXPathSelector
from test.items import TestItem

class DmozSpider(BaseSpider):
    domain_name = 'dmoz.org'
    start_urls = [
        "http://www.dmoz.org/Computers/Programming/Languages/Python/Books/",
        "http://www.dmoz.org/Computers/Programming/Languages/Python/Resources/"
    ]

    def parse(self, response):
#        filename = response.url.split("/")[-2]
        #open(filename, 'wb').write(response.body)
        hxs = HtmlXPathSelector(response)
        sites = hxs.select('//ul[2]/li')
        items = []
        for site in sites:
            item = TestItem()
            item['title'] = site.select('a/text()').extract()
            item['link'] = site.select('a/@href').extract()
            item['desc'] = site.select('text()').extract()
            items.append(item)
        return items

SPIDER = DmozSpider()

