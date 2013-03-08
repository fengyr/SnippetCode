#!/usr/bin/env python
#!encoding=utf8

from ctypes import *

libc = cdll.LoadLibrary('libc.so.6')


class cell(Structure):
    _fields_ = [('wYear', c_ushort),
                ('wMonth', c_ushort),
                ('wDay', c_ushort),
                ('wHour', c_ushort),
                ('wMin', c_ushort),
                ('wSec', c_ushort),
                ]

temp_cell = cell()
temp_cell.wYear = 2009
temp_cell.wMonth = 1
temp_cell.wDay = 8
temp_cell.wHour = 12
temp_cell.wMin = 12
temp_cell.wSec = 20

print temp_cell, byref(temp_cell), temp_cell.wYear


class timeval(Structure):
    _fields_ = [('tv_sec', c_int),
                ('tv_usec', c_int),
                ]

#class timezone(Structure):
    #_fields_ = [('tz_minuteswest', c_int),
                #('tz_dsttime', c_int),
    #]

tv = timeval()
tv.tv_sec = 10000
tv.tv_usec = 12000

print libc.settimeofday(byref(tv), None)
