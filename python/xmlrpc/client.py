#!/usr/bin/env python
#!encoding=utf8

import xmlrpclib

proxy = xmlrpclib.ServerProxy("http://localhost:80000/")

print proxy.read_text()
print proxy.sum(100, 1)
print proxy.system.listMethods()
