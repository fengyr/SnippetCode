#!/usr/bin/python
#encoding=utf8

import pygtk
pygtk.require('2.0')
import gtk, gtk.glade
import os, sys
import win, lyric
import unittest

class widget:
    def __init__(self):
        self.wid = gtk.glade.XML('./win.glade')
        for i in [win.win, win.FileSelect, lyric.lyric]:
            self.wid.signal_autoconnect(i.__dict__)
        
    def __del__(self):
        pass
    
    def __getitem__(self, key):
        return self.wid.get_widget(key)

    def run(self):
        gtk.main()

if __name__ == '__main__':
    wid = widget()
    for i in [win, lyric]:
        i.set_widget(wid)

    wid.run()

