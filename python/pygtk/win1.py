import pygtk
pygtk.require('2.0')
import gtk, gtk.glade
import os, sys

class win:
	def on_button1_clicked(button):
		print "hello world"
		
	def on_window1_destroy_event(obj, event):
		print "exit"
		
	def on_window1_delete_event(obj, event):
		gtk.main_quit()