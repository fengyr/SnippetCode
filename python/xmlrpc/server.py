#!/usr/bin/python
import datetime
from SimpleXMLRPCServer import SimpleXMLRPCServer
import xmlrpclib

def read_text():
	"""read 1.txt content
	"""
	f = open('1.txt')
	return xmlrpclib.Binary(f.read())
	f.close()

def sum(x, y):
	return x+y

def is_even(n):
	return n%2 == 0

def today():
	today = datetime.datetime.today()
	return xmlrpclib.DateTime(today)

server = SimpleXMLRPCServer(("192.168.100.73", 80000))
server.register_introspection_functions()

print "Listening on port 80000..."
server.register_function(read_text, 'read_text')
server.register_function(sum, 'sum')
server.register_function(is_even, 'is_even')
server.register_function(today, 'today')

server.serve_forever()
