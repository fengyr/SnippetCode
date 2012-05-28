#!/usr/bin/python
#!encoding=utf8
import os
import traceback


try:
    open('abcde')
except Exception:
    #traceback.print_exc()
    print traceback.extract_stack()
    print 'debug end'
    
