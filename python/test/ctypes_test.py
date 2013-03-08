#!/usr/bin/env python
#!encoding=utf8

from ctypes import *

#cdll.LoadLibrary(module_name)
#CDLL(module_name)
libc = cdll.LoadLibrary('libc.so.6')

#内置函数
#sizeof
#string_at
#memset
#memmove
#resize
#create_string_buffer
#addressof
#alignment
#get_errno
from ctypes.util import find_library
print find_library('c')
print find_library('SDL')
print find_library('ogg')
print find_library('ogg error')

#设置函数参数
#func.argtypes = [...]
printf = libc.printf
printf.argtypes = [c_char_p, c_char_p, c_int]
printf("this is %s %d\n", "num", 2)

#设置函数返回结果
#func.restype = [...]
strchr = libc.strchr
strchr.argtypes = [c_char_p, c_char]
res = strchr("abcdefg", 'd')
print res
strchr.restype = c_char_p
res = strchr("abcdefg", 'd')
print res

#指针引用
#byref(p)

#结构体
class POINT(Structure):
    _fields_ = [("x", c_int),
                ("y", c_int)
    ]

class POINT_16(Structure):
    _fields_ = [("x", c_int, 16),
                ("y", c_int, 16)
    ]

class MyStruct(Structure):
    _fields_ = [("a", c_int),
                ("b", c_float),
                ("point_array", POINT * 4),
                ("point_p", POINTER(POINT))
    ]

#指针数组
TenPointArrayType = POINT * 10
i = TenPointArrayType((1,1), (2,2), (3,3), (4,4), (5,5), (6,6), (7,7), (8,8), (9,9), (10,10))
for ii in i:
    print ii.x, ii.y

pi = pointer(i)
for ii in range(len(i)):
    print ii, pi[0][ii].x, pi[0][ii].y

#类型转换
#cast(to_type, from_type)
class Bar(Structure):
    _fields_ = [("count", c_int),
                ("values", POINTER(c_int))
    ]

bar = Bar()
bar.count = 4
val = (c_int*4)(1, 2, 3, 4)
bar.values = cast( (c_byte*4)(), POINTER(c_int) )
bar.values = val
print bar.count, bar.values[2]

#不完全类型
class cell(Structure):
    pass

cell._fields_ = [("name", c_char_p),
                 ("next", POINTER(cell))]

#回调函数定义
#func = CFUNCTYPE(type1, type2, ...)
def py_cmp_func(a, b):
    print a, b
    return 0

CMPFUNC = CFUNCTYPE(c_int, POINTER(c_int), POINTER(c_int))
cmpfunc = CMPFUNC(py_cmp_func)

