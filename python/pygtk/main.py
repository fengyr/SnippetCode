#!/usr/bin/python

import pygtk
pygtk.require('2.0')
import gtk, gtk.glade
import os, sys
import win1

class widget:
	def __init__(self):
		self.wid = gtk.glade.XML('./test.glade')
		for i in [win1.win]:
			self.wid.signal_autoconnect(i.__dict__)
			print i.__dict__
# 		self.wid.signal_autoconnect(locals())

	def run(self):
		gtk.main()

if __name__ == '__main__':
	wid = widget()
	wid.run()
		
