#!/usr/bin/env python
#!encoding=utf8

import re
from config import *

FILE_NOT_FOUND = '找不到歌词文件'
FILE_ENCODING_ERROR = '文件编码错误'


def set_widget(w):
    global widget

    widget = w


def get_lyric(filename):
    # import pdb;pdb.set_trace()
    tv = widget['lyric']
    fn = filename[:-3] + 'lrc'

    try:
        f = open(fn)
        try:
            alltext = f.read()
            try:
                text = ''.join(re.split('\[.*\]', alltext))
                rule = re.split('\r\n', alltext)
                tv.get_buffer().set_text(text.decode('gbk'))
            except UnicodeDecodeError:
                try:
                    tv.get_buffer().set_text(text.decode('utf8'))
                except UnicodeDecodeError:
                    tv.get_buffer().set_text(FILE_ENCODING_ERROR)
        finally:
            f.close()
    except:
        tv.get_buffer().set_text(FILE_NOT_FOUND)


class lyric:
    def on_songtext_delete_event(obj, event):
        win = widget['songtext']
        win.hide()
        print 'lyric closed'

        return True
