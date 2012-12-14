#!/usr/bin/python
#!encoding=utf8

'''
@file: c.py
@author: Zenki here
@brief: simple demo ok
'''

class Abc(object):
    """docstring for Abc"""
    def __init__(self, arg):
        super(Abc, self).__init__()
        self.arg = arg
    def say(self):
        """docstring for fname"""
        print self.arg

if __name__ == '__main__':
    a = Abc('hello world')
    a.say()
