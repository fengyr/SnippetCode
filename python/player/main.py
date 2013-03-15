#!/usr/bin/python
# encoding=utf8

# pygtk需要的库
import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade

import win
import lyric

class widget:
    def __init__(self):
        # 获取gtk.glade.XML对象，该对象作为GTK的Context
        self.wid = gtk.glade.XML('./win.glade')
        for i in [win.win, win.FileSelect, lyric.lyric]:
            self.wid.signal_autoconnect(i.__dict__)

    def __del__(self):
        pass

    # 定义该方法，可使用下标[widget_name]访问gtkWidget对象
    def __getitem__(self, key):
        return self.wid.get_widget(key)

    # 进入gtk事件循环
    def run(self):
        gtk.main()

if __name__ == '__main__':
    wid = widget()

    # 将widget对象传递给其他类，其他类可通过[widget_name]获取gtkWidget对象
    for i in [win, lyric]:
        i.set_widget(wid)

    wid.run()
