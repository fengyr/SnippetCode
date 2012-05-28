#!encoding=utf8

from scrapy.spider import BaseSpider
from scrapy.selector import HtmlXPathSelector
from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.http import Request, FormRequest, Response
import config
from baidu_mp3.items import BaiduMp3Item
import codecs
import urllib2
from lxml import etree

class BaiduMp3Spider(BaseSpider):
    domain_name = 'baidu.com'
    start_urls = [ config.TOP100_URL ]

    def raw_unistr(self, unistr):
        return codecs.raw_unicode_escape_encode(unistr)[0].decode('gbk')

    def baidu_unistr(self, unistr):
        return ''.join([ hex(ord(i)).replace('0x', '%') for i in list(unistr.encode('gbk'))])

    def parse_songs_list(self, response):
        hxs = HtmlXPathSelector(response)
        songs_info = hxs.select('/html/body/div[2]/table[2]/tbody/tr[2]/td[2]/a/@href')
        
    def search_des_urls(self, list_url):
        #import pdb;pdb.set_trace()
        url_f = urllib2.urlopen(list_url)
        list_url_context = url_f.read()
        list_url_root = etree.HTML(list_url_context)

        url_tr_lists = list_url_root.xpath('//tr[position()>1]/td[2]/a/@href')
        song_sizes = list_url_root.xpath('//tr[position()>1]/td[8]/text()')
        des_url = ''
        for i in range(len(url_tr_lists)):
            try:
                if float(song_sizes[i].split()[0]) > 3:
                    des_url = url_tr_lists[i]
                    break
            except Exception, e:
                print 'search_des_urls: ', e

        return self.find_des_url(des_url.encode('gbk'))

    def find_des_url(self, down_url):
        if down_url == '':
            return down_url
        else:
            down_url = down_url.replace('%', '\%').replace(' ', '')
        
        import pdb;pdb.set_trace()
        url_f = urllib2.urlopen(down_url)
        down_url_context = url_f.read()
        down_url_root = etree.HTML(down_url_context)
        temp_down_url = down_url_root.xpath('//*[@id="urla"]/@href')

#        url_f = urllib2.urlopen(temp_down_url.replace('%', '\%'))
        #down_url_context = url_f.read()
        #down_url_root = etree.HTML(down_url_context)
        #final_down_url = down_url_root.xpath('//*[@id="urla"]')

        return final_down_url
        

    def parse(self, response):
        hxs = HtmlXPathSelector(response)
        items = []
        songs_list_url = ''
        #搜索搜有歌曲分类
        songs = hxs.select('//tr')
        #歌曲名,歌手名
        for song in songs[:2]:
            item = BaiduMp3Item()
            songname = ''
            author = ''
            try:
                item['songname'] = songname = self.raw_unistr(song.select('td[3]/a/text()').extract()[0])
                item['author'] = author = self.raw_unistr(song.select('td[4]/a/text()').extract()[0])
                songs_list_url = config.SONGS_LIST_URL % (self.baidu_unistr(songname), self.baidu_unistr(author))
                #import pdb;pdb.set_trace()
                item['link'] = self.search_des_urls(songs_list_url)
            except Exception, e:
                print '*************:', e
                
            items.append(item)

        return items

SPIDER = BaiduMp3Spider()

