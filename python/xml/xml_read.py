#!/usr/bin/env python
#!encoding=utf8

from xml.dom import minidom

doc = minidom.parse('./test.xml')
print doc.toxml()
